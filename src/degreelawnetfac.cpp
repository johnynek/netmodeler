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
  int n_with_deg = 0;
  int this_deg = _dpf.minDegree() - 1; //We will increment this later
  for(int i = 0; i < _nodes; i++) {
    Node* this_node = _nf->create();
    net->add( this_node );
    int degs = 0;
    if( _indep ) {
      //Assign degree independently:
      degs = _dpf.getRandomDegree( _rand.getDouble01() );
    }
    else {
      //Each degree gets a fixed fraction of nodes in the network:
      while( (n_with_deg == 0) && (this_deg < _dpf.maxDegree()) ) {
        this_deg++;
        n_with_deg = (int)round( _nodes * _dpf.getProbabilityOf( this_deg ) );
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
    while( degs-- > 0 ) {
      stubs.push_back( this_node );
    }
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

  
  //Now pair up the nodes:
  int ssize = stubs.size();
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
      do {
        n2 = stubs[j];
        if ( (n2 != 0) && (n1 != n2) && (net->getEdge(n1, n2) == 0) ) {
          //This is a valid pair!
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
      } while( done == false );
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
  //We have the network, return it:
  return net;
}
