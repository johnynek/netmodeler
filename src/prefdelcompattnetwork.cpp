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

  if( _rand.getBool( _del_p ) && (getNodeSize() > 1000)) {
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
	    if( _rand.getBool( _uniform_del_p ) && (getNodeSize() > 2000)) 
	    {
		 node = _rand.getInt(getNodeSize() - 1);
		 NodeIterator ni = getNodeIterator();
		 ni.moveNext();
		 while( node-- > 0) { ni.moveNext(); }
		 removenodes.insert(ni.current());
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
