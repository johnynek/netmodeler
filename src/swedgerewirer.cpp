/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida
Copyright (C) 2005  Tae Woong Choi <twchoi@ufl.edu>, University of Florida

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

#include <swedgerewirer.h>
#include <uniformnodeselector.h>
#include <swnodeselector.h>

#define FOREACH(it, col) for(it = col.begin(); it != col.end(); it++)

using namespace Starsky;
using namespace std;

SWEdgeRewirer::SWEdgeRewirer(EdgeFactory& ef, double p, Ran1Random& rand): _ef(ef), _prob(p), _rand(rand)
{
  //Make the default Node selectors:
  _startsel = new UniformNodeSelector(_rand);
  _endsel = _startsel;
}

SWEdgeRewirer::SWEdgeRewirer(EdgeFactory& ef, double p, Ran1Random& rand,
		INodeSelector* startsel,
		INodeSelector* endsel) : _ef(ef), _prob(p), _rand(rand)
{
  _startsel = startsel;
  _endsel = endsel;
  if( _endsel == 0) {
    _endsel = startsel;
  }
}

SWEdgeRewirer::~SWEdgeRewirer()
{
  if( _startsel != _endsel ) {
    delete _endsel;
    _endsel = 0;
  }
  delete _startsel;
  _startsel = 0;
}
/**
void SWEdgeRewirer::notToRewire(SWNetwork* net, SWNetwork* rewire_net)
{
  auto_ptr<EdgeIterator> ei(rewire_net->getEdgeIterator() );
  while(ei->moveNext() ) {
    net->remove(ei->current() );
  }
  //return net;
}
*/
/**
void SWEdgeRewirer::ToRewire(SWNetwork* net)
{
  //set<Edge*> edges_to_rewire;
  //SWNetwork* toRewireNet = net->newNetwork();
  //toRewireNet = net->newNetwork();
  auto_ptr<EdgeIterator> ei( net->getEdgeIterator() );
  while( ei->moveNext() ) {
    if( _rand.getBool(_prob) ) {
      //Rewire this edge:
      edges_to_rewire.insert( ei->current() );
      //toRewireNet->add(ei->current() );
    }
  }
  //return edges_to_rewire;
  //return toRewireNet;
}
*/
//SWNetwork* SWEdgeRewirer::map(SWNetwork* net, SWNetwork* toRewireNet, int m)
//SWNetwork* SWEdgeRewirer::map(SWNetwork* net, std::set<Edge*> edges_to_rewire, int m)
SWNetwork* SWEdgeRewirer::map(SWNetwork* net, int m)
{
  if(_endsel != _startsel) {
    _endsel-> selectFrom( net);
  }
  _startsel->selectFrom( net );
  set<SWEdge*> edges_to_rewire;
  auto_ptr<EdgeIterator> ei( net->getEdgeIterator() );
  while( ei->moveNext() ) {
    if( _rand.getBool(_prob) ) {
	//Rewire this edge:
	edges_to_rewire.insert( dynamic_cast<SWEdge*>(ei -> current() ) );
	//edges_to_rewire.insert( ei -> current()  );
    }
  }
  set<SWEdge*>::iterator reit;
  FOREACH(reit, edges_to_rewire)
  {
      RandAddrNode *start, *end; 
      std::string attr;
      do {
        //Method m = _method;
        if (m == 1) {  //rewire both ends to random
          //_startsel->selectFrom( net );
          start = dynamic_cast<RandAddrNode*> (_startsel->select());
          end = dynamic_cast<RandAddrNode*>(_endsel->select(start));
	  attr=(*reit)->getAttributes();
        }  
        else if ( m == 2) {  //rewire one end to random
          if (_rand.getBool(0.5)) {
            start = dynamic_cast<RandAddrNode*> ( (*reit) -> first);
          }
          else {
            start = dynamic_cast<RandAddrNode*> ( (*reit) -> second);
          }	
          end = dynamic_cast<RandAddrNode*> (_endsel->select(start));
        }
        else if ( m == 3) {  //rewire one end to shortcut
          if (_rand.getBool(0.5)) {
            start = dynamic_cast<RandAddrNode*> ( (*reit) -> first);
          }
          else {
            start = dynamic_cast<RandAddrNode*> ( (*reit) -> second);
          }	
          double x = _rand.getDouble01();
          int net_size = net->getNodeSize();
          int k = int(pow(net_size, x));
          int shortcut_addr = (start->getAddress() + k) % net_size;
          end = net->node_vec[shortcut_addr];
        }
      } while(net->getEdge(start,end)!=0);
      //auto_ptr<Edge> new_edge ( _ef.create(start, end ) );
      //auto_ptr<SWEdge> new_edge ( _ef.create(start, end ) );
      //auto_ptr<SWEdge> new_edge( new SWEdge(start, end, (*reit)->getAttributes() ) );
      //new_edge = new SWEdge(start, end, (*reit)->getAttributes() );
      //cout << "edge attr: " << (*reit)->printAttributes() << endl;
    //}
    RandAddrNode* orig_first=dynamic_cast<RandAddrNode*>( (*reit)->first);
    RandAddrNode* orig_second=dynamic_cast<RandAddrNode*>( (*reit)->second);
    //RandAddrNode* new_first=dynamic_cast<RandAddrNode*>(new_edge->first);
    //RandAddrNode* new_second=dynamic_cast<RandAddrNode*>(new_edge->second);
    //cout << "original: (" << orig_first->getAddress() <<"," << orig_second->getAddress() <<"), rewiered: ("<< rew_first->getAddress() <<","<<rew_second->getAddress()<<")" << endl;
    //cout << "original: (" << orig_first->getAddress() <<"," << orig_second->getAddress() << "," << (*reit)->printAttributes() << "), rewiered: ("<< start->getAddress() <<","<<end->getAddress()<<")" << endl;
    //SWEdge* orig_edge = dynamic_cast<SWEdge*>(*reit);
    //cout << orig_edge->printAttributes() << endl;
    //while( !net->add( *(new_edge ) ) );
    //net->add( *(new_edge ) );
    //net->add( *(new_edge.get() ) );
    net->add( SWEdge(start,end,attr ) );
    //cout << "new edge's attr: " << new_edge->printAttributes() << endl;
      //The above adds a *copy* of the node IF there is no such node already in the network.
    net->remove(*reit );
    //delete new_edge;
    //string attrs = (*reit) -> getAttribues();   
    //net->remove( *reit );
  //  cout << "(" << start << ", " << end << ")" << endl;
    //Edge* new_edge = _ef.create(start, end, attrs);
    //Edge* new_edge = _ef.create(start, end);
    //net->add(new_edge); 
  }
  return net;
}
