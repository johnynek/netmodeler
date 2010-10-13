#include "sapartitioner.h"

using namespace Starsky;
using namespace std; 

PNIterator::PNIterator(Random& rand, IEdgeSelector& es, INodeSelector& ns,
                       const cnt_ptr<NetworkPartition> part)
  : _rand(rand), _part(part), _es(es), _ns(ns)
{
  _current = _part; 
  _comp_map = _part->asMap();
}

Iterator<cnt_ptr<NetworkPartition> >* PNIterator::clone() {
  return 0;
}

const cnt_ptr<NetworkPartition>& PNIterator::current() {
  return _current;
}

bool PNIterator::moveNext() {
  //Select a random edge:
  bool moved = false;
  while(!moved) {
    Edge* rand_edge = _es.select();
    Node* node1 = rand_edge->first;
    Node* node2 = rand_edge->second;
    cnt_ptr<Network> n1( (*_comp_map)[node1] );
    cnt_ptr<Network> n2( (*_comp_map)[node2] );
    if( n1 != n2 ) {
      //Randomly transfer:
      cnt_ptr<Network> to_trans( _rand.getBool(0.5) ? n1 : n2 );
      _current = _part->transfer(to_trans, rand_edge);
      moved = true;
  /*
      cout << "transfer: " << rand_edge->toString() << endl;
      cout << "init parts: " << _part->size() << endl;
      cout << "final parts: " << _current->size() << endl;
   */
    }
    else {
      //select a random node and isolate it:
      Node* rand_node = _ns.select();
      NetworkPartition* attempt = _part->isolate(rand_node);
      if( attempt != 0 ) {
        moved = true;
        _current = attempt;
     /*
        cout << "isolate" << endl;
        cout << "init parts: " << _part->size() << endl;
        cout << "final parts: " << _current->size() << endl;
*/
      } 
    }
  }
  return true;
}

void PNIterator::reset() {

}

PartNeighbor::PartNeighbor(Random& r, const Network& orig)
: _rand(r), _orig(orig), _es(r), _ns(r)
{
  //orig should never change, but selectors don't know that, and need to watch for changes
  //bad design maybe, to circumvent, we cast away const:
  _es.selectFrom((Network*)&orig);
  _ns.selectFrom((Network*)&orig);
}

Iterator<cnt_ptr<NetworkPartition> >*
PartNeighbor::neighborsOf(const cnt_ptr<NetworkPartition>& part) {
  return new PNIterator(_rand, _es, _ns, part);
}

SAPartitioner::SAPartitioner(Random& rand,
               SAEnergy<cnt_ptr<NetworkPartition> >& energy, int max_steps)
: _rand(rand), _energy(energy), _max_steps(max_steps)
{

}

NetworkPartition* SAPartitioner::partition(const Network& input)
{
  PartNeighbor pt(_rand, input);
  vector<cnt_ptr<Network> >* init_part = new vector<cnt_ptr<Network> >();
  auto_ptr<Iterator<Node*> > node_i(input.getNodeIterator());
  while(node_i->moveNext()) {
    //Put each in a separate cluster:
    cnt_ptr<Network> tmp_net( input.newNetwork() );
    tmp_net->add(node_i->current());
    init_part->push_back(tmp_net); 
  }
  //sort it!
  ptr_gt<cnt_ptr<Network> > ngt;
  sort(init_part->begin(), init_part->end(), ngt);
  //Here is the initial partition:
  cnt_ptr<NetworkPartition> init(new NetworkPartition(input, init_part));

  double init_e = _energy.energyOf(init);
  int N = input.getNodeSize();
  double t0 = 2.0 * init_e/log((double)N); //1/\sqrt{N} chance of moving
  ExpTempSched ts(100, 0.99);
  //Run at most N^2 steps without improving:
  int msteps = _max_steps == -1 ? (10*N*N) : _max_steps;
  SAOpt<cnt_ptr<NetworkPartition> > saopt(_rand, init, pt, _energy, ts, msteps);
  int steps = 0;
  while(saopt.moveNext()) {
    //std::cout << "step: " << steps++ << " mod: " << saopt.current().second << std::endl;
  }
  pair<cnt_ptr<NetworkPartition>, double> result = saopt.getBest();
  std::cout << "best energy: " << result.second << std::endl;
  NetworkPartition* result_np = result.first.get();
  //Now tell the result to stop counting;
  result.first.disown();
  return result_np;
}
