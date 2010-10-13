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

#ifndef starsky__saopt_h
#define starsky__saopt_h

#include <functional>
#include <memory>
#include "random.h"

namespace Starsky {

/**
 * Implement Simulated Annealing
 */
template<typename T>
class SANeighborGenerator {
  public:
    virtual Iterator<T>* neighborsOf(const T& point) = 0;
};

template<typename T>
class SAEnergy {
  public:
    virtual double energyOf(const T& point) = 0;
};

template<typename T>
class SAOpt : public Iterator<std::pair<T, double> > {

  public:
    SAOpt(Random& rand, const T& init,
          SANeighborGenerator<T>& neighborfunc,
          SAEnergy<T>& energy_func,
          Iterator<double>& temp_schedule, int max_neighbor_check = -1)
      : _rand(rand), _neigh(neighborfunc),
        _energy(energy_func), _temp(temp_schedule), _max_n(max_neighbor_check) {
       double einit = _energy.energyOf(init);
      _current = std::pair<T, double>(init, einit);
      _best = _current;
    }

    SAOpt<T>* clone() {
      return 0;
    }
  
    bool moveNext() {
      const T& here = _current.first;
      double energy = _current.second;
      _temp.moveNext();
      double temp = _temp.current();
      
      std::auto_ptr<Iterator<T> > nit( _neigh.neighborsOf(here) );
      bool move = false;
      double this_e;
      int neighbor_moves = 0;
      while((!move) && nit->moveNext() && ((neighbor_moves < _max_n) || (_max_n == -1))) {
        neighbor_moves += 1;
        const T& this_case = nit->current();
        this_e = _energy.energyOf(this_case);
        double d = (this_e - energy);
        //std::cout << "delta e: " << d << std::endl;
        if( d < 0 ) {
          //Always accept a move to a better state:
          move = true;
        }
        else if( temp <= 0 ) {
          //when temp <= 0, only accept improvements:
          move = false;
        }
        else if( exp( -d/temp ) > _rand.getDouble01() ) {
          //temp > 0, we got a good trade
          move = true;
        }
        //std::cout << "move: " << move << std::endl;
      }
      //When we get here, we move to 
      if( move ) {
        _current = std::pair<T, double>(nit->current(), this_e);
        if( this_e < _best.second ) {
           //We have a new best:
           _best = _current;
        }
      }
      //We only stop if t == 0, and move == false
      return move;
    }

    const std::pair<T,double>& current() {
      return _current; 
    }
    const std::pair<T,double>& getBest() { return _best; }
    void reset() { }
    
  protected:
    Random& _rand;
    SANeighborGenerator<T>& _neigh;
    SAEnergy<T>& _energy;
    Iterator<double>& _temp;
    const int _max_n;
    std::pair<T,double> _current;
    std::pair<T,double> _best;
};

class ExpTempSched : public Iterator<double> {

  public:
    ExpTempSched(double t0, double alpha) : _current(t0), _alpha(alpha) {
      
    }

    ExpTempSched(const ExpTempSched& copy) : _current(copy._current),
                                             _alpha(copy._alpha) { }

    //This has to be a const ref type to match Iterator 
    const double& current() {
      return _current;
    }
    ExpTempSched* clone() {
      return new ExpTempSched(_current, _alpha);
    }

    bool moveNext() {
      _current = _alpha * _current;
      return true;
    }
    void reset() { }

  protected:
    double _current;
    double _alpha;
};

}
#endif
