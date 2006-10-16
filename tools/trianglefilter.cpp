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

#include <netmodeler.h>
#include <cstdio>
#include <list>
#include "optionparser.h"

using namespace Starsky;
/**
 * This is a very simple tool.  It applies the TriangleMapper on a network
 * and prints the network back out.
 */

class Remover {

  public:
    Network* _net;
    double _th;
    Remover(Network* net, double th) {
      _net = net;
      _th = th;
    }

    //Remove edges which have low clustering coefficient which are not degree 1
    bool check(Edge* e) {
      if( _net->getEdgeCC(e) <= _th && 
	  ( (_net->getDegree( e->first ) > 1) && (_net->getDegree(e->second) > 1) ) ) {
        return true;
      }
      else {
        return false;
      }
    }
    
};


int main(int argc, char* argv[]) {

  std::vector<std::string> reqs;
  std::vector<std::string> opts;
  opts.push_back("input");
  opts.push_back("edgecc_thresh");
  OptionParser op(reqs, opts);
  try {
    op.parse(argc, argv);
  }
  catch (std::exception x) {
    std::cout << "usage: " << argv[0] << op.getUsageString() << std::endl;
    return -1;
  }
  Network* net = 0;
  NetworkFactory nf;
  if( op.getStringOpt("input","-") == "-" ) {
    net = nf.create(std::cin);
  }
  else {
    std::ifstream in( op.getStringOpt("input","-").c_str() );
    net = nf.create(in);
  }
  EdgeIterator* ei = net->getEdgeIterator();
  double thresh = op.getDoubleOpt("edgecc_thresh", 0.0);
  Remover r(net, thresh);
  ClassFilterator<Remover, Edge*> fi(ei, &r, &Remover::check);
  net->remove(&fi);
  net->printTo(std::cout);
  delete net;
}
