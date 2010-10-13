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

#ifndef starsky__sapartitioner_h
#define starsky__sapartitioner_h

#include <network.h>
#include <inetworkpartitioner.h>
#include <saopt.h>
#include <infocom.h>
#include <map>
#include <uniformnodeselector.h>
#include <uniformedgeselector.h>
#include <shuffleedgeselector.h>
#include <shufflenodeselector.h>

namespace Starsky {

class PNIterator : public Iterator<cnt_ptr<NetworkPartition> > {

  public:
    PNIterator(Random& rand, IEdgeSelector& es, INodeSelector& ns,
               cnt_ptr<NetworkPartition> part);
    Iterator<cnt_ptr<NetworkPartition> >* clone();
    const cnt_ptr<NetworkPartition>& current();
    bool moveNext(); 
    void reset();
  private:
    Random& _rand;
    cnt_ptr<NetworkPartition> _current;
    cnt_ptr<NetworkPartition> _part;
    cnt_ptr<std::map<Node*, cnt_ptr<Network> > > _comp_map;
    IEdgeSelector& _es;
    INodeSelector& _ns;
    
};

class PartNeighbor : public SANeighborGenerator<cnt_ptr<NetworkPartition> > {
  public:
    PartNeighbor(Random& r, const Network& orig);
    Iterator<cnt_ptr<NetworkPartition> >*
      neighborsOf(const cnt_ptr<NetworkPartition>& p);
  private:
    Random& _rand;
    ShuffleEdgeSelector _es;
    ShuffleNodeSelector _ns;
    const Network& _orig;
};

class ModularityEnergy : public SAEnergy<cnt_ptr<NetworkPartition> > {
  public:
    double energyOf(const cnt_ptr<NetworkPartition>& arg) {
      //We want high modularity, so use negative as energy
      double mod = arg->getModularity();
      //std::cout << "mod e: " << mod << std::endl;
      return -mod;
    }
};

class ComInfoEnergy : public SAEnergy<cnt_ptr<NetworkPartition> > {
  public:
    double energyOf(const cnt_ptr<NetworkPartition>& arg) {
      //We want high modularity, so use negative as energy
      double mod = arg->getComInformation();
      //std::cout << "mod e: " << mod << std::endl;
      return -mod;
    }
};

class InformationEnergy : public SAEnergy<cnt_ptr<NetworkPartition> > {
  public:
    double energyOf(const cnt_ptr<NetworkPartition>& arg) {
      double info = InfoCom::informationOf(arg.get());
      //Want high info, so negative this:
      return -info;
    }
};

/**
 * A representation of a partitioning a network
 */
class SAPartitioner : public INetworkPartitioner {
  public:
    SAPartitioner(Random& rand,
                  SAEnergy<cnt_ptr<NetworkPartition> >& energy,
                  int max_steps=-1);
   /**
    * The caller of this function should delete the result
    * when done.
    * This vector is sorted from largest to smallest ([0] is the biggest)
    */
    NetworkPartition* partition(const Network& input);
  
  private:
    Random& _rand;
    SAEnergy<cnt_ptr<NetworkPartition> >& _energy;
    const int _max_steps;
};
	
}

#endif
