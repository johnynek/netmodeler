#include "prefdelcompattnetwork.h"

using namespace Starsky;
using namespace std;
#define _totaldelete 10

PrefDelCompAttNetwork::PrefDelCompAttNetwork(const Network& seed,
		                       Random& r,
				       double p,
				       int connections,
				       double q,
				       double dpp
				      ) : PrefAtNetwork(seed,
					                         r,
							         connections),
				                                 _del_p(p),
								 _restore_q(q),
								 _uniform_del_p(dpp)
{

}

PrefDelCompAttNetwork::PrefDelCompAttNetwork(const Network& seed,
		                       Random& r,
				       const DegreePreferenceFunction& dp,
				       double p,
				       int connections,
				       double q,
				       double dpp) : PrefAtNetwork(seed,
					                         r,
								 dp,
								 connections),
				                                 _del_p(p),
								 _restore_q(q),
								 _uniform_del_p(dpp)
{

}	

const Network::NodePSet PrefDelCompAttNetwork::getRemoveNodes()
{
  NodePSet removenodes;
  bool flag=false;

  if( _rand.getBool( _del_p ) && (node_set.size() > 1000)) {
      Node * nit;
      nit = findPartnerFor(0); 
	if (nit){
	//	cout<<"ir came to here"<<_del_p<<endl;
		removenodes.insert(nit);
		flag=true;
		    	}
  }
  //cout<<"it is here working"<<_uniform_del_p<<endl;
 
 
    
   NodePSet::const_iterator nit;
//	cout<<"ir came to here"<<_uniform_del_p<<endl;


    int node;// = _rand.getInt(node_set.size() - 1);
    for (int jj=0;jj<_totaldelete;jj++){
	    if( _rand.getBool( _uniform_del_p ) && (node_set.size() > 2000)) 
	    {
		 node = _rand.getInt(node_set.size() - 1);
		 nit = node_set.begin();
		 while( node-- > 0) { nit++; }
		 removenodes.insert(*nit);
		 flag=true;
	    }
    }
	//	cout<<"it is working"<<_uniform_del_p<<endl;


  

  if (flag)
		return removenodes;
  
  return _empty_nodeset;
}

bool PrefDelCompAttNetwork::needsCompensation(Node* n, Edge* e)
{
  return _rand.getBool( _restore_q );
}
