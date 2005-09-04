/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "degreelawnetfac.h"

using namespace std;
using namespace Starsky;

DegreeLawNetFac::DegreeLawNetFac(int nodes, DegreeProbabilityFunction& dpf,
		                 Random& ran, bool indep) : NetworkFactory(),
                                                            _nodes(nodes),
                                                            _dpf(dpf),
                                                            _rand(ran),
                                                            _indep(indep)
{

}

DegreeLawNetFac::DegreeLawNetFac(int nodes, DegreeProbabilityFunction& dpf,
		                 Random& ran,
                                 NodeFactory* nf, EdgeFactory* ef,
                                 bool indep) : NetworkFactory(nf, ef),
                                                            _nodes(nodes),
                                                            _dpf(dpf),
                                                            _rand(ran),
                                                            _indep(indep)
{

}


Network* DegreeLawNetFac::create()
{
  /**
   * The algorithm here is the following:
   * 1) Create each node and assign it a degree.
   * 2) For each degree create a "stub".
   * 3) Randomly shuffle the stubs.
   * 4) Start at the first stub, go through the list until we find a match.
   * 5) Repeat at the next position.
   */
  //Create the nodes:
  Network* net = new Network();
  vector<Node*> stubs;
  //This variable is used for the "indep = false" case, where
  //a fixed fraction of the nodes have a given degree.
  int max_degree = 0;
  int n_with_deg = 0;
  double surplus_nodes = 0.0; //Used in the indep case
  int this_deg = _dpf.minDegree() - 1; //We will increment this later
  for(int i = 0; i < _nodes; i++) {
    Node* this_node = _nf->create();
    //cout << "Nodefactory created: " << this_node << endl;
    net->add( this_node );
    int degs = 0;
    if( _indep ) {
      //Assign degree independently:
      //cout << "getRandomDegree" << endl;
      degs = _dpf.getRandomDegree( _rand.getDouble01() );
      //cout << "got: " << degs << endl;
    }
    else {
      //Each degree gets a fixed fraction of nodes in the network:
      while( (n_with_deg == 0) && (this_deg < _dpf.maxDegree()) ) {
        this_deg++;
	double tmp_n = ( (double) _nodes) * _dpf.getProbabilityOf( this_deg )
		       + surplus_nodes;
	n_with_deg = (int)floor(tmp_n);
	surplus_nodes = tmp_n - (double)n_with_deg;
      }
      //This is one more node with this degree:
      if( this_deg <= _dpf.maxDegree() ) {
        degs = this_deg;
        n_with_deg--;
      }
      else {
        //All the rest get zero degree:
        degs = 0;
      }
    }
    //cout << "Adding node: " << this_node << " degree: " << degs << endl;
    if( max_degree < degs ) {
      //Keep track of the max degree
      max_degree = degs;
    }
    while( degs-- > 0 ) {
      stubs.push_back( this_node );
    }
    //cout << "Next loop" << endl;
  }
  //We have all the nodes and stubs.
  _rand.shuffle( stubs );
  
  /*
   * Lets make super-double-sure
   */
#if 0
    //Since we can't control this randomization (with a seed)
    //it is not repeatable, but if you want to turn it on,
    //go ahead:

    //Randomize the order of the stubs:
    vector<Node*>::iterator node_a = stubs.begin();
    vector<Node*>::iterator node_b = stubs.end();
    //Use the STL to randomize the stubs:
    //The below is from STL <algorithm>.  Applies a random permutation
    //However, I we don't know the algorithm, so we will assume this
    //did nothing.  We will still connect pairs at random below:
    random_shuffle(node_a, node_b);
#endif

  /*
   * There are two algorithms here.
   * One uses a vector, which is fast to shuffle, but
   * slow to delete from.  We use the null (0) pointer
   * to delete nodes, but later we have to skip over them.
   * When there are a lot of nodes to skip, this may not be 
   * efficient. 
   *
   * This algorithm appears to be about twice as fast as the
   * list based one below (possibly because skipping the
   * same node does not happen very often, and the there is
   * a big time penalty to copy the vector into the list.
   */
  //if( max_degree < _nodes/4 ) {
  if( true ) {
    //Now pair up the nodes:
    int ssize = stubs.size();
//#define CHECK_SKIPS
#ifdef CHECK_SKIPS
  int skips = 0;
#endif
    for(int i = 0; i < ssize; i++)
    {
      Node* n1 = stubs[i];
      Node* n2;
      if( n1 != 0 ) {
        //This node is not yet assigned:
        int j = i+1;
        bool done = ( j >= ssize );
        while( done == false ) {
          n2 = stubs.at(j);
          if ( (n2 != 0) && (n1 != n2) && (net->getEdge(n1, n2) == 0) ) {
            //This is a valid pair
            //cout << "Adding edge: (" << n1 << ", " << n2 << ")" << endl;
            net->add( _ef->create(n1, n2) );
            stubs[j] = 0;
            done = true;
          }
          else {
            //Can't match: n1:
  #ifdef CHECK_SKIPS
            if( n2 == 0 ) {
              //We are skipping a slot
              skips++;
            }
  #endif
          }
          j = j + 1;
          done = done || ( j >= ssize );
        }
      }
      else {
        //This position has already been handled:
      }
      /*
       * If it is possible to pair n1, we have done so,
       * if not, we just move on.
       */
      stubs[i] = 0;
    }
  #ifdef CHECK_SKIPS
    //The below was used for checking how often we had to skip
    //to see if a list would be better (it does not appear to be better)
    cout << "Skipped: " << skips << endl;
  #endif
    stubs.clear();
  }
  else {
    /*
     * Here is the list based algorithm.  A list is slow
     * to shuffle, but fast to erase from, so there is no
     * skipping over "deleted" nodes later in the algorithm.
     *
     * It appears (in limited testing) that this algorithm
     * is about half as fast as the one above.
     */
    list<Node*> stub_list;
    stub_list.insert( stub_list.begin(), stubs.begin(), stubs.end());
    stubs.clear();
    list<Node*>::iterator lit1, lit2;
    while(stub_list.size() > 1) {
      Node* n1 = stub_list.front();
      //cout << "Node 1:" << n1 << endl; 
      //Remove the first item:
      stub_list.pop_front();
      lit2 = stub_list.begin();
      while( lit2 != stub_list.end() ) {
        Node* n2 = *lit2;
        if ( (n2 != 0) && (n1 != n2) && (net->getEdge(n1, n2) == 0) ) {
          //This is a valid pair
          //cout << "Adding edge: (" << n1 << ", " << n2 << ")" << endl;
          net->add( _ef->create(n1, n2) );
          //Now we take lit2 out of the list:
          stub_list.erase(lit2);
          //this causes us to exit the loop
          lit2 = stub_list.end();
        }
        else {
          //Go on to the next one:
          //We may eventually run out, and then this node will be skipped.
          lit2++;
        }
      }
    }
  }
  //We have the network, return it:
  return net;
}
