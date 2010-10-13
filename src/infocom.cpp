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

#include "infocom.h"

using namespace std;
using namespace Starsky;

InfoCom::InfoCom() : AgglomPart() {
  reset();
}

double InfoCom::getNextJoin(const Network& net,
                               const std::map<Node*, int>& node_community,
		               const std::map<int, std::map<int, double> >& e_ij,
			       const vector< double >& a_i,
			       int& join1, int& join2)
{
    if( _node_cnt.empty() ) {
      //This is the first run:
      //Each community has one node:
      _node_cnt.resize(net.getNodeSize(), 1);  
      _edge_cnt = net.getEdgeSize();
    }
    double delta_q, tmp_delta, tmp_info, max_info = -1.0, eij_join;
    Edge* edge_join = 0;
    //It is important to initialize the communities with
    //sane values.  If (for some reason) there are no edges
    //between these communities,
    if( node_community.size() > 1 ) {
      map<Node*, int>::const_iterator tmpit;
      tmpit = node_community.begin();
      join1 = tmpit->second;
      tmpit++;
      join2 = tmpit->second;
      //Make sure join1 <= join2:
      if( join2 < join1 ) { int tmp = join1; join1 = join2; join2 = tmp; }
    }
    else {
      join1 = node_community.begin()->second;
      join2 = join1;
    }
    delta_q = 0.0;
    int com1, com2;
    map<Node*, int>::const_iterator com1_it, com2_it;
    auto_ptr<EdgeIterator> ei( net.getEdgeIterator() );
    while( ei->moveNext() ) {
      Edge* this_edge = ei->current();
      com1_it = node_community.find( this_edge->first );
      com2_it = node_community.find( this_edge->second );
      com1 = com1_it->second;
      com2 = com2_it->second;
      if( com1 != com2 ) {
	std::map<int, std::map<int, double> >::const_iterator mit = e_ij.find(com1);
	std::map<int, double>::const_iterator mit2 = mit->second.find(com2);
	double eij = mit2->second;
        tmp_delta = 2 * (eij - a_i[com1] * a_i[com2] );
        //Compute the information after this join
        tmp_info = computeInfo(com1, com2, eij);
        /*
         * The first join always increases the mutual information the
         * same amount for all edge choices, so we always use the second
         * criterion (below) to prefer more highly clustered edges in that
         * case (this is an example of where the greedy joining is suboptimal
         */
        if( (0 == edge_join) || ( tmp_info > max_info ) ) {
          //Make sure join1 <= join2:
          if( com1 < com2 ) { join1 = com1; join2 = com2; }
          else { join1 = com2; join2 = com1; }
          delta_q = tmp_delta;
          eij_join = eij;
	  edge_join = this_edge;
          max_info = tmp_info;
        }
        if(  tmp_info == max_info ) {
          //Fall back to choosing the edge with the most triangles:
          int this_tri = net.getTriangles(this_edge);
          if( this_tri > net.getTriangles(edge_join) ) {
            //Make sure join1 <= join2:
            if( com1 < com2 ) { join1 = com1; join2 = com2; }
            else { join1 = com2; join2 = com1; }
            delta_q = tmp_delta;
            eij_join = eij;
	    edge_join = this_edge;
            max_info = tmp_info;
          }
        }
      }
    }
    //Update our internal state: 
    if( edge_join != 0 ) {
      //It is possible there is no edge join here that can merge more communities
      //If that is the case, edge_join == 0, so if we find a joinable community
      //Add it here:
      double pe;
      getProbs(join1, join2, eij_join, pe, _p_c, _p_cge, _p_cgebar); 
      _node_cnt[join1] = _node_cnt[join1] + _node_cnt[join2];
      _node_cnt[join2] = 0;
      _info_at_join.push_back(max_info);
      //std::cout << edge_join << " info: " << max_info << endl;
    }
    else {
      //Signal there is no good join:
      join1 = 0;
      join2 = 0;
      _info_at_join.push_back( _info_at_join.back() );
    }
    return delta_q;
}

double InfoCom::h2(double p) {
  if ( p <= 0.0 ) { return 0.0; }
  if ( p >= 1.0 ) { return 0.0; }
  return - p * log(p)/log(2.0) - (1.0 - p) * log(1.0 - p)/log(2.0);
}

double InfoCom::informationOf(NetworkPartition* part) {
  /*
   * We need to know p_c|e, p_c|\bar{e} and p_e
   */
  const Network& orig = part->getOriginal();
  double E = (double)orig.getEdgeSize();
  double N = (double)orig.getNodeSize();
  double Ebar = N*(N-1.0)/2.0 - E;

  auto_ptr<Iterator<cnt_ptr<Network> > > comps(part->getComponents());
  
  double p_c = 0.0;
  double p_cge = 0.0;
  double p_cgebar = 0.0;
  while(comps->moveNext()) {
    double ni = (double)comps->current()->getNodeSize();
    //degrees!! in this component
    double eii = 2.0 * (double)comps->current()->getEdgeSize();
    p_c += ni * (ni - 1.0);
    p_cge += eii;
    p_cgebar += (ni * (ni - 1.0)) - eii;
  }
  //Renormalize:
  p_c /= (N*(N-1.0));
  p_cge /= (2.0 * E);
  p_cgebar /= (2.0 * Ebar);
  double p_e = 2 * E/(N*(N-1.0)); 
  double p_ebar = 1.0 - p_e; 
  //We should have: p_c = p_e * p_cge + p_ebar * p_cgebar
  if( abs(p_c - (p_e * p_cge + p_ebar * p_cgebar)) > 0.0001 ) {
    cout << "p_c(" << p_c << ") != "
         << "p_e * p_cge + p_ebar * p_cgebar" << p_e * p_cge + p_ebar * p_cgebar <<endl;
  }
  return h2(p_c) - p_e * h2(p_cge) - p_ebar * h2(p_cgebar);
}

void InfoCom::getProbs(int ci, int cj, double eij, double& pe,
                       double& pc, double& pce, double& pceb) const {
  double ni = (double)_node_cnt[ci];
  double nj = (double)_node_cnt[cj];
  double n = (double)_node_cnt.size();
  double e = (double) _edge_cnt;
 
  //eij is a fraction of edge ends in each community, we want ends:
  eij *= 2*e;
  double all_pairs = n*(n-1.0)/2.0;
  double empty_edge = all_pairs - e;
  pe = e / all_pairs;
  pc = _p_c +  ni*nj/all_pairs;
  pce = _p_cge + eij/e;
  pceb = _p_cgebar + (ni*nj - eij)/empty_edge;
#if 0
  std::cout << "pc = " << pc
            << "pe * pce + (1-pe)*pceb " << (pe * pce + (1-pe)*pceb) << std::endl
            << "err = " << (pc - (pe * pce + (1-pe)*pceb)) << std::endl;
#endif
}

double InfoCom::computeInfo(int clusteri, int clusterj, double eij) const {
  double pe, pc, pce, pceb;
  getProbs(clusteri, clusterj, eij, pe, pc, pce, pceb); 
#if 0
  std::cout << "h2(pc)=" << h2(pc) << " pce=" << pce << " pceb= " << pceb 
            << " pe=" << pe << " h2(pce)=" << h2(pce) << " h2(pceb)=" << h2(pceb) << std::endl;
#endif
  return h2(pc) - pe * h2(pce) - (1.0 - pe) * h2(pceb);
}


void InfoCom::reset() {
  _info_at_join.clear();
  _info_at_join.push_back(0.0); //No information before joining
  _node_cnt.clear();
  _p_c = 0.0;
  _p_cge = 0.0;
  _p_cgebar = 0.0;
  _edge_cnt = 0;
}
