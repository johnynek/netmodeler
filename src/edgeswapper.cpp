#include "edgeswapper.h"
#include <algorithm>

//This is pretty handy:
#define FOREACH(it, col) for(it = col.begin(); it != col.end(); it++)

//#define DEBUG(msg) cout << msg << endl;
#define DEBUG(msg) 

using namespace std;
using namespace Starsky;

EdgeSwapper::EdgeSwapper(EdgeFactory& ef, Random& rand, double p, Method meth) : _rand(rand),
								_p(p),
								_method(meth),
								_ef(ef)
{
  
}

void EdgeSwapper::map(Network* net)
{
  DEBUG("About to look for edges to swap");
  vector<Edge*> edges;
  Network::EdgeSet::const_iterator eit;
  FOREACH(eit, net->getEdges()) {
    if ( _rand.getBool(_p) ) {
      edges.push_back(*eit);
    }
  }
  //This is in the STL algorithms
  random_shuffle(edges.begin(), edges.end());
  //Now take pairs and delete them:
  DEBUG("About to swap");
  vector<Edge*>::iterator dit;
  FOREACH(dit, edges) {
    Edge* one = *dit;
    dit++;
    if( dit != edges.end() ) {
      DEBUG("Not at end");
      Edge* two = *dit;
      DEBUG("About to call contains on: " << one << " " << two );
      DEBUG("One: " << one->toString() << " Two: " << two->toString());
      if( one->contains( two->first ) ||
	  one->contains( two->second ) ) {
        //It is meaningless to swap edges when they share
	//a node
	DEBUG("Edges share a node");
      }
      else {
	DEBUG("Edges don't share a node");
        Method m = _method;
	if( m == RandomSwap ) {
          if( _rand.getBool( 0.5 ) ) {
            m = HighToHigh;
	  }
	  else {
            m = HighToLow;
	  }
	}
	DEBUG("Sorting nodes");
        Node *high_1, *high_2, *low_1, *low_2;
	if( net->getDegree(one->first) > net->getDegree(one->second) ) {
          high_1 = one->first;
	  low_1 = one->second;
	}
	else {
          high_1 = one->second;
	  low_1 = one->first;
	}
	if( net->getDegree(two->first) > net->getDegree(two->second) ) {
          high_2 = two->first;
	  low_2 = two->second;
	}
	else {
          high_2 = two->second;
	  low_2 = two->first;
	}
        Edge* ea;
	Edge* eb;
	DEBUG("Creating edges");
	if( m == HighToHigh ) {
          ea = _ef.create(high_1, high_2);
	  eb = _ef.create(low_1, low_2);
	}
	else if( m == HighToLow ) {
          ea = _ef.create(high_1, low_2);
	  eb = _ef.create(high_2, low_1);
	}
	if( net->has(*ea) || net->has(*eb) ) {
	  DEBUG("Could not add");
          //Don't do anything if either of these new edges are not new.
	  delete ea;
	  delete eb;
	}
	else {
	  DEBUG("Adding the edges");
          net->add(ea);
	  net->add(eb);
	  DEBUG("deleting the old edges");
	  net->remove(one);
	  net->remove(two);
	}
      }
    }
    else {
      //That's it
      DEBUG("break out");
      break;
    }
  }
}
