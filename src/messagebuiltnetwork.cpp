#include "messagebuiltnetwork.h"

using namespace Starsky;
using namespace std;

MessageBuiltNetwork::MessageBuiltNetwork(int nodes,
		                         Message& aMessage,
					 Random& aRand,
		                         int iterations)
                                         :
					   _message(aMessage),
					   _rand(aRand),
					   _iter(iterations)
{
    int i;
    Node* this_node = 0;
    //Here is the completely connected seed network:
    for(i = 0; i < 3; i++) {
        this_node = new Node();
	Network::add(this_node);
	NodePSet::const_iterator nit;
	for( nit = node_set.begin();
	     nit != node_set.end();
	     nit++)
	{
	  //We have to make sure not to add loops:
	  if(this_node != *nit) {
            Network::add( Edge(this_node, *nit) );
	  }
	}
    }
    
    incrementTime(nodes - 3);
}

MessageBuiltNetwork::MessageBuiltNetwork(const Network& seed,
		                         Message& aMessage,
					 Random& aRand,
		                         int iterations)
                                         : 
					   _message(aMessage),
					   _rand(aRand),
					   _iter(iterations)
{
  Network::operator=(seed);
}

Node* MessageBuiltNetwork::findPartnerFor(Node* start)
{
  //begin is the node we start the message at
  Node* begin = start;
  NodePSet::const_iterator nit;
  int rn;
  if( getNeighbors(start).size() == 0 ) {
    //start does not have any neighbors, so start at random
    nit = node_set.begin();
    rn = _rand.getInt( node_set.size() - 1);
    if( has(start) ) { rn--; }
    while( rn-- > 0 ) { nit++; }
    if( *nit == start ) { nit++; }
    
    //Now we change our start node:
    begin = *nit;
  }
  //Start at the start, and visit the nodes.
  _message.forgetVisitedNodes();
  _message.visit(begin, *this );
  //Count how many of the visited nodes we are not connected to:
  int num = 0;
  for(nit = _message.getVisitedNodes().begin();
      nit != _message.getVisitedNodes().end();
      nit++)
  {
      //If this node is not where we started or one of our neighbors,
      //increment the count.
      if( (*nit != start) && (getNeighbors(start).count(*nit) == 0) )
      {
        num++;
      }
  }
  //Now we know how many of the visited nodes we are not already
  //connected to.

  //If there are no nodes to connect to, return NULL
  if( num == 0) { return 0; }
  //Select one of the nodes we are not already connected to:
  rn = _rand.getInt(num-1);
  nit = _message.getVisitedNodes().begin();
  //If this is one of the nodes we should not connect to, go on
  while( (*nit == start) || (getNeighbors(start).count(*nit) != 0) )
  {
    nit++;
  }
  //Select a random node:
  while( rn-- > 0)
  {
    nit++;
    //Skip all the nodes that we are connected to:
    while( (*nit == start) || (getNeighbors(start).count(*nit) != 0) )
    {
      nit++;
    }
  }
  return *nit;
}

#if 0
//This needs to be done carefully

const Network::NodePSet MessageBuiltNetwork::getRemoveNodes()
{
  if( _rand.getBool(0.5) ) {
    NodePSet rem;
    NodePSet::const_iterator nit = node_set.begin();
    if( _rand.getBool( 0.0 ) ) {
      //Remove the highest degree node
      Node* max = *nit;
      while( nit != node_set.end() ) {
        if( getDegree(max) < getDegree(*nit) ) {
          max = *nit;
        }
        nit++;
      }
      rem.insert(max);
    }
    else {
      //Return a random node:
      int rn = _rand.getInt( node_set.size() - 1);
      while( rn-- > 0 ) {
        nit++;
      }
    }

    rem.insert(*nit);
    return rem;
  }
  return _empty_nodeset;
}
#endif
