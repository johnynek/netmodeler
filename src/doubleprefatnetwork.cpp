#include "doubleprefatnetwork.h"

using namespace Starsky;
using namespace std;

DoublePrefAtNetwork::DoublePrefAtNetwork(const Network& seed,
			                 Random& r,
			                 int c) : PrefAtNetwork(seed,r,c)
{
}
										 

DoublePrefAtNetwork::DoublePrefAtNetwork(const Network& seed,
			                 Random& r,
					 const DegreePreferenceFunction& dp,
			                 int c) : PrefAtNetwork(seed,r,dp,c)
{
}

void DoublePrefAtNetwork::postNodeAdd(Node* node)
{
  //Select two nodes preferentially, and add an edge between them
  Node* first = findPartnerFor(0);
  Node* second = findPartnerFor(first);
  if( first && second ) {
    Network::add( Edge(first,second) );
  }
  if( first == second ) {
    cerr << "first == second" << endl;
  }
}
