/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2010  P. Oscar Boykin <boykin@pobox.com>, University of Florida

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
#include <fstream>

#include "optionparser.h"

using namespace std;
using namespace Starsky;


class energy_func : public SAEnergy<double> {
    double energyOf(const double& arg) {
      return (arg*arg - 2)*(arg*arg - 2);
    }
};

class RandIt : public Iterator<double> {
  public:
  RandIt(Random& r, double here, double w) : _rand(r), _here(here), _width(w) { }
  Iterator<double>* clone() {
    return new RandIt(_rand, _here, _width);
  }
  const double& current() {
    return _here;
  }  

  bool moveNext() {
    _here = _here + _rand.getDouble(_width/2.0, -_width/2.0);
    return true; 
  }
  void reset() { }

  double _here;
  double _width;
  Random& _rand;
};

class neigh_func : public SANeighborGenerator<double> {
  public:
    neigh_func(Random& r) : _rand(r) { }
    Iterator<double>* neighborsOf(const double& arg) {
      return new RandIt(_rand, arg, 1.0);
    }

    Random& _rand;
};

int main(int argc, char* argv[]) {

  vector<string> reqs;
  reqs.push_back("steps");
  vector<string> optionals;
  OptionParser op(reqs, optionals);
  try {
    op.parse(argc, argv);
  }
  catch (exception x) {
    cout << "usage: " << argv[0] << op.getUsageString() << endl;
    return -1;
  }
  int steps = op.getIntOpt("steps", -1);
  Ran1Random rand(-1);
  neigh_func nf(rand);
  energy_func ef;  
  ExpTempSched ts(100, 0.99);
  SAOpt<double> sa(rand, 0.0, nf, ef, ts);
  int step = 0;
  while(sa.moveNext() && step < steps) {
    std::pair<double, double> here = sa.current();
    std::cout << step << " " << here.first << " " << here.second << std::endl;
    step += 1; 
  } 
  std::cout << "#best: " << std::endl
            <<  sa.getBest().first << " " << sa.getBest().second << std::endl;
  return 0;
}
