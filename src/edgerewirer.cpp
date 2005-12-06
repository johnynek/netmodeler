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

#include <edgerewirer.h>
#include <uniformnodeselector.h>

#define FOREACH(it, col) for(it = col.begin(); it != col.end(); it++)

using namespace Starsky;
using namespace std;

EdgeRewirer::EdgeRewirer(EdgeFactory& ef, double p, Random& rand): _ef(ef),
								   _prob(p),
								   _rand(rand)
{
  //Make the default Node selectors:
  _startsel = new UniformNodeSelector(_rand);
  _endsel = _startsel;
}

EdgeRewirer::EdgeRewirer(EdgeFactory& ef, double p, Random& rand,
		INodeSelector* startsel,
		INodeSelector* endsel) : _ef(ef), _prob(p), _rand(rand)
{
  _startsel = startsel;
  _endsel = endsel;
  if( _endsel == 0) {
    _endsel = startsel;
  }
}

EdgeRewirer::~EdgeRewirer()
{
  if( _startsel != _endsel ) {
    delete _endsel;
    _endsel = 0;
  }
  delete _startsel;
  _startsel = 0;
}

void EdgeRewirer::map(Network* net)
{
  RandomEdgeFilterator rei( net->getEdgeIterator(), _rand, _prob);
  map( net, &rei );
}

void EdgeRewirer::map(Network* net, Iterator<Edge*>* it)
{
  //Make a list of edges to rewire:
  vector<Edge*> edges_to_rewire;
  it->pushInto(edges_to_rewire);
  
  if( _endsel != _startsel ) {
    _endsel->selectFrom( net );
  }
  _startsel->selectFrom( net );

  vector<Edge*>::iterator reit;
  FOREACH(reit, edges_to_rewire)
  {
    Edge* this_edge = *reit;
    string attrs = this_edge->getAttributes();
    net->remove( this_edge );
    Node* start = _startsel->select();
    Node* end = _endsel->select(start);
  //  cout << "(" << start << ", " << end << ")" << endl;
    Edge* new_edge = _ef.create(start, end, attrs);
    net->add(new_edge); 
  }
}
