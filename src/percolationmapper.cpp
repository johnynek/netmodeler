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

#include "percolationmapper.h"

using namespace Starsky;
using namespace std;

PercolationMapper::PercolationMapper(Random& rand,
		                     double bp,
				     double sp) : _rand(rand),
						  _bond_p(bp),
						  _site_p(sp)
{

}

double PercolationMapper::getExpectedThreshold(const Network* net)
{
  IntStats ns;
  ns.collect(net, &Network::getDegree);
  double k1 = ns.getAverage();
  double k2 = ns.getMoment2();
  return (k1/(k2 - k1));
}

void PercolationMapper::map(Network* net)
{
  PercFilter pf(_rand);
  if( _site_p < 1.0 ) {
    pf.setProb(_site_p);
    FilteredIterator<PercFilter, Node*> nfi(net->getNodeIterator(), &pf,
		                            &PercFilter::removeNode);
    net->remove(&nfi);
  }
  if( _bond_p < 1.0 ) {
    pf.setProb(_bond_p);
    FilteredIterator<PercFilter, Edge*> efi(net->getEdgeIterator(), &pf,
		                            &PercFilter::removeEdge);
    net->remove(&efi);
  }
}

PercolationMapper::PercFilter::PercFilter(Random& r) : _rand(r) { }
void PercolationMapper::PercFilter::setProb(double p) { _prob = p; }
bool PercolationMapper::PercFilter::removeEdge(Edge* e) { return !_rand.getBool(_prob); }
bool PercolationMapper::PercFilter::removeNode(Node* n) { return !_rand.getBool(_prob); }
