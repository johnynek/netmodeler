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

#include "degreelawrandomnetwork.h"

using namespace Starsky;
using namespace std;

DegreeLawRandomNetwork::DegreeLawRandomNetwork(int nodes,
		                               DegreeProbabilityFunction& dpf,
					       Random& r,
					       bool node_deg_indep) : Network(),
					                       _dpf(dpf), _rand(r)
{
    //First create the nodes:
    int i = 0;
    while( i++ < nodes ) {
        add(new Node());
    }

    //Holds a Node* to for each "stub", or degree, in the network.
    vector<Node*> stub_set;
    int degs = 0;
 if(node_deg_indep) {
    double r_d = 0.0;
    //Randomly generate degrees for each node
    NodeIterator ni = getNodeIterator();
    while( ni.moveNext() ) {
	r_d = _rand.getDouble01();
        degs = _dpf.getRandomDegree( r_d );
	while( degs-- > 0 ) {
            stub_set.push_back( ni.current() );
	}
    }
  }
  else {
    //Give each node a degree from the degree function:
    int max_deg = _dpf.maxDegree();
    NodeIterator ni = getNodeIterator();
    ni.moveNext();
    int nodes_to_assign = nodes;
    int temp_n;
    int stub = 0;
    double n_frac, n_rem = 0.0;
    //We loop through because due to rounding errors we might not
    //assign all the nodes we need to:
    while(nodes_to_assign > 0 ) {
      nodes = nodes_to_assign;
      for(degs = _dpf.minDegree(); degs <= max_deg; degs++ ) {
	//Stop looping through the degrees once we have assigned all the nodes
	if( nodes_to_assign == 0) { break; }
	//Add the fraction of a node(n_rem) that we didn't use last time:
	n_frac = ((double)nodes) * _dpf.getProbabilityOf( degs ) + n_rem;
	//Round:
        //temp_n = (int)floor(n_frac + 0.5);
	//Floor:
        temp_n = (int)(n_frac);
	n_rem = n_frac - (double)temp_n;
	/*
	cout << "frac n: " << ((double)nodes) * _dpf.getProbabilityOf( degs ) << " ";
	cout << "temp_n: " << temp_n
	     << " nodes_to_assign: " << nodes_to_assign
	     << " degs: " << degs << endl;
	*/
	if( temp_n > nodes_to_assign) {
          temp_n = nodes_to_assign;
	}
	nodes_to_assign -= temp_n;
	while(temp_n-- > 0) {
	  for(int i = 0; i < degs; i++) {
            stub_set.push_back( ni.current() );
	    //cout << "Made stub: " << stub++ << " nodes to assign" << nodes_to_assign << endl;
	  }
	  ni.moveNext();
	}
      }
    }
  }
  //cout << "Made all stubs" << endl;
    //Now we have a set of stubs and we just connect
    //them randomly with the exception that no two nodes
    //are connected twice, and no node is connected to itself.
    vector<Node*>::iterator node_a, node_b;
  
    //Randomize the order of the stubs:
    node_a = stub_set.begin();
    node_b = stub_set.end();
#if 1
    //Use the STL to randomize the stubs:
    //The below is from STL <algorithm>.  Applies a random permutation
    //However, I we don't know the algorithm, so we will assume this
    //did nothing.  We will still connect pairs at random below:
    random_shuffle(node_a, node_b);
#endif
     
    //Now we connect random pairs of nodes:
    bool skip_stub = false;
    Node *n_a = 0;
    Node *n_b = 0;
    int tries = 0;
    int temp_size = stub_set.size();
    int pos = 0;
    int pos_b = 0;
    int e=0;
    
    degs = temp_size;
    while( temp_size >= 2 ) {
	do {
	  n_a = stub_set[pos % degs];
	  pos++;
	} while( n_a == 0);
	//Zero out this position:
	stub_set[(pos-1) % degs] = 0;
	temp_size--;
	//Select a random node after the n_a:
	pos_b = pos + _rand.getInt( temp_size - 1 );
	do {
	  n_b = stub_set[ pos_b %degs ];
	  pos_b++;
	} while( (n_b == 0) || (n_a == n_b));
	skip_stub = false;
	tries = 0;
	while( has( Edge(n_a, n_b) ) )
	{
	    pos_b = pos + _rand.getInt( temp_size - 1 );
	    do {
	      n_b = stub_set[ pos_b %degs ];
	      pos_b++;
	    } while( (n_b == 0) || (n_a == n_b) );
	    tries++;
	    //cout << "Tries: " << tries << endl;
	    if( tries == temp_size ) {
		skip_stub = true;
		//cerr << "had to skip a degree" << endl;
		//We have wrapped around, and can't find any more nodes.
                break;
	    }
	}
	if( !skip_stub ) {
          //cout << "Added Edge(" << n_a << ", " << *node_b << ")" << endl;
	  //cout << "Edges: " << e++ << endl;
          add( Edge( n_a, n_b) );
	  //Zero out this position:
	  stub_set[(pos_b - 1) % degs ] = 0;
	  temp_size--;
	}
    }
    //We have added all the edges we need to add.
} 
