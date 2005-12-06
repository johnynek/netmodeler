/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
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

#include "clusterpart.h"

using namespace Starsky;

ClusterPart::ClusterPart(double m): _min_cc(m) { }

std::vector<Network*>* ClusterPart::partition(const Network& input) {
  MinCCEdgeTester test(&input, _min_cc);
  std::auto_ptr<Network> net( input.clone() );
  ClassFilterator<MinCCEdgeTester, Edge*> fi(input.getEdgeIterator(),
		                             &test, &MinCCEdgeTester::shouldRemove);
  //Remove the edges that match.
  net->remove(&fi); 
  ComponentPart cp;
  return cp.partition(*net);
}

ClusterPart::MinCCEdgeTester::MinCCEdgeTester(const Network* net, double m): _net(net),
								       _m(m) { }

bool ClusterPart::MinCCEdgeTester::shouldRemove(Edge* e) {
#if 0
  int min_deg = std::min( _net->getDegree( e->first ), _net->getDegree(e->second) );
  return ( (_net->getEdgeCC(e) <= _m) && (min_deg > 1) );
#else
  /*
   * Here we make sure to leave every node at least one edge.
   */
  //Keep an edge if it is the node with the highest clustering
  //coefficient *or* it is above the threshold
  double my_cc = _net->getEdgeCC(e);
  if ( my_cc > _m ) { return false; }
  int min_deg = std::min( _net->getDegree( e->first ), _net->getDegree(e->second) );
  if( 1 == min_deg ) { return false; }
  //If we get here, it must not be above the threshold
  bool is_max = true;
  std::auto_ptr<EdgeIterator> ei( _net->getEdgeIterator(e->first) );
  while( ei->moveNext() && is_max ) {
    if( ei->current() == e ) { continue; }
    double this_cc = _net->getEdgeCC( ei->current() );
    is_max = (my_cc > this_cc);
  }
  bool is_max2 = true;
  std::auto_ptr<EdgeIterator> ej( _net->getEdgeIterator(e->second) );
  while( ej->moveNext() && is_max2 ) {
    if( ej->current() == e ) { continue; }
    double this_cc = _net->getEdgeCC( ej->current() );
    is_max2 = (my_cc > this_cc);
  }
  is_max = is_max || is_max2;
  return ( !is_max );
#endif
}
