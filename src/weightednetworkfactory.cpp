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

#include <weightednetworkfactory.h>

//#define DEBUG(msg) std::cout << msg << std::endl;
#define DEBUG(msg)

using namespace std;
using namespace Starsky;

WeightedNetworkFactory::WeightedNetworkFactory() :
	NetworkFactory(new NodeFactory(), new WeightedEdgeFactory())
{

}

WeightedNetworkFactory::WeightedNetworkFactory(NodeFactory* nf, EdgeFactory* ef) : NetworkFactory(nf,ef)
{

}

Network* WeightedNetworkFactory::create(istream& in)
{
  Network* net = new WeightedNetwork();
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
    DEBUG("Added" << first->toString() );
    if( result.size() == 1 ) { //There was no neighbors
      continue;
    }
    double weight = 1.0;
    if( result.size() > 2 ) {
      weight = atof( result[2].c_str() );
    }
    DEBUG("weight:" << weight );
    //There is a list of second nodes:
    vector<SuperString> nodes = result[1].split(" ");
    vector<SuperString>::iterator sit;
    for(sit = nodes.begin();
	sit != nodes.end();
	sit++) {
      second = 0;
      second = _nf->create( *sit );
      net->add( second );
      //Find any existing edge for this Node:
      
      Edge* existing = net->getEdge(first, second);
      
      if( existing != 0 ) {
        weight += existing->getWeight();
        DEBUG("existing:" << existing->toString() );
        net->remove(existing);
      }
      //Delete the existing edge:
      Edge* e = 0;
      stringstream attr;
      attr << weight;
      e = _ef->create(first, second, attr.str() );
      DEBUG("Made new edge: " << e->toString());
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
