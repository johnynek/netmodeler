#include <netmodeler.h>

using namespace Starsky;

class NodeLeaveAction : public Action {
  public:
    NodeLeaveAction(EventScheduler& sched, Network& n, Node* me)
      : _sched(sched), _net(n), _me(me)
    {

    }
    void Execute() {
      //schedule a time to add a new node:
      _net.remove(_me);
      std::cout << _sched.getCurrentTime() << "\t"
                << _net.getNodeSize() << "\t"
                << _net.getEdgeSize()
                << std::endl;
    }
  protected:
    EventScheduler& _sched;
    Network& _net;
    Node* _me;
};


/**
 * Represents a node joining a network
 * When we execute, we select two random nodes and make edges to them,
 * and schedule a time to leave.
 */
class NodeJoinAction : public Action {
  public:
    NodeJoinAction(EventScheduler& sched, Random& r, Network& n, INodeSelector& ns)
      : _sched(sched), _r(r), _net(n), _ns(ns)
    {

    }

    void Execute() {
      _ns.selectFrom(&_net);
      Node* me = new Node();
      Node* neighbor0 = _ns.select();
      Node* neighbor1 = _ns.select(neighbor0);
      if( neighbor0 != 0 ) {
        _net.add(Edge(me, neighbor0));
      }
      //Avoid that one if possible
      if( neighbor1 != 0 ) {
        _net.add(Edge(me, neighbor1));
      }
      //Make sure I get added no matter what
      _net.add(me);
      //Plan to leave:
      double lifetime = 3600.0 * _r.getDouble01();
      Action* leave = new NodeLeaveAction(_sched, _net, me);
      _sched.after(lifetime, leave);
      //Plan to rejoin
      double sleeptime = 3600.0 * _r.getDouble01();
      Action* rejoin = new NodeJoinAction(_sched, _r, _net, _ns);
      _sched.after(lifetime + sleeptime, rejoin);
      //Print out results:
      std::cout << _sched.getCurrentTime() << "\t"
                << _net.getNodeSize() << "\t"
                << _net.getEdgeSize()
                << std::endl;
    }
  protected:
    EventScheduler& _sched;
    Random& _r;
    Network& _net;
    INodeSelector& _ns;
};

int main(int argc, char* argv[])
{

  EventScheduler sched;
  double time = 0;
  double interval = 0.5;

  Ran1Random rand(-1);
  Network net;
  UniformNodeSelector ns(rand); 

  //Add 1000 nodes, and just ru
  for(int i = 0; i < 1000; i++) {
    Action* a = new NodeJoinAction(sched, rand, net, ns);
    //Keep in mind, sched will delete a after Execute is called.
    sched.at(time, a); 
    time += interval;
  }
  //Run for 360,000 seconds (100 hours) of simulated time
  Action* stop = new StopAction(sched);
  sched.at(360000, stop);
  std::cout << "#About to start" << std::endl;
  sched.start();
  std::cout << "#Finished" << std::endl;
  return 0;
}
