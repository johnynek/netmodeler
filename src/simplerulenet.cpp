#include "simplerulenet.h"

using namespace Starsky;
using namespace std;


void SimpleRuleNet::incrementTime(int steps)
{
  while(steps-- > 0) {
    Node* new_node = new Node();
    add( new_node );
    //Make the connections:
    int connections = getConnectionCount(new_node);
    while(connections-- > 0) {
      Node* partner = findPartnerFor(new_node);
      //if we can't find a partner, partner == 0)
      if(partner) {
        add( Edge(new_node, partner) );
      }
    }
    //Now find the nodes to remove:
    NodePSet rem = getRemoveNodes();
    //The set of nodes which at the end will be compensated
    NodePSet comped;
    NodePSet::iterator it;
    for( it = rem.begin();
         it != rem.end();
	 it++)
    {
      //First we remove self loops, which should not
      //be there, but are not compensated for:

      remove( Edge(*it, *it) );
	    
      EdgeSet::const_iterator e_it;
      //Find the nodes that will need compensation
      for(e_it = getEdges( *it ).begin();
	  e_it != getEdges( *it ).end();
	  e_it++)
      {
        Node* other = (*e_it)->getOtherNode( *it );
	if( needsCompensation(other, *e_it) ) {
          comped.insert( other );
	}
      }
      //Now remove the node in question
      remove( *it );
      //Make sure this node is not in our set to be compensated:
      comped.erase( *it );
    }
    //Now compensate the nodes that need it:
    NodePSet::iterator n_it;
    for(n_it = comped.begin();
	  n_it != comped.end();
	  n_it++)
    {
      Node* partner = findPartnerFor( *n_it );
      if( partner ) {
        //cerr << "compensating: (" << *n_it << ", " << partner << ")" << endl;
        if( partner == *n_it) { cerr<<"findPartner found self" <<endl;}
        add( Edge(*n_it, partner) );
      }
    }
    //Now we call the post node addition function:
    postNodeAdd(new_node);
  }
}
