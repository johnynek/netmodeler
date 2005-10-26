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

#include <networkfactory.h>

using namespace std;
using namespace Starsky;

NetworkFactory::NetworkFactory() {

  _nf = new NodeFactory();
  _ef = new EdgeFactory();
}

NetworkFactory::NetworkFactory(NodeFactory* nf, EdgeFactory* ef) : _nf(nf), _ef(ef) 
{

}

NetworkFactory::~NetworkFactory()
{
  delete _nf;
  _nf = 0;
  delete _ef;
  _ef = 0;
}

Network* NetworkFactory::create(istream& in)
{
  Network* net = new Network();
  SuperString line;
  while( getline(in, line, '\n') ) {
    if( line[0] == '#' ) { //This is a comment
      continue;
    }
    vector<SuperString> result = line.split(" : ");
    Node* first = 0;
    Node* second = 0;
    first = _nf->create(result[0]);
    net->add( first ); 
    if( result.size() == 1 ) { //There were no neighbors
      continue;
    }
    //There is a list of second nodes:
    auto_ptr< Iterator<SuperString> > si( result[1].spliterator(" ") );
    while( si->moveNext() ) {
      second = 0;
      second = _nf->create( si->current() );
      net->add( second );
      Edge* e = 0;
      if( result.size() > 2 ) {
        e = _ef->create(first, second, result[2]);
      }
      else {
        e = _ef->create(first, second);
      }
      //Add the edge:
      if( e ) {
	/**
	 * Remember: we have to add references to edges, 
	 * not pointers.  This is because we don't want to allow
	 * multiple edges between nodes (for now).
	 */
        net->add( *e );
      }
      delete e;
    }
  }
  return net;
}
