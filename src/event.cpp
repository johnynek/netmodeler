#include "event.h"
#include <memory> //for std::auto_ptr
using namespace Starsky;

EventScheduler::EventScheduler() : _time(0.0), _stop(false)
{

}

EventScheduler::~EventScheduler() {
  while( false == _event_queue.empty() ) {
    const Event& e = _event_queue.top();
    //Make sure all the actions are deleted
    delete e.ToDo;
    _event_queue.pop();
  }
}

void EventScheduler::at(double t, Action* a) {
  Event e(t, a);
  _event_queue.push(e);
}

void EventScheduler::start() {
  _stop = false;
  while( (false == _event_queue.empty()) && (false == _stop)) {
    const Event& e = _event_queue.top();
    _time = e.RunAt;
    //This makes sure we delete no matter what:
    std::auto_ptr<Action> ap(e.ToDo);
    //We are done with this one
    _event_queue.pop();
    //Now actually execute after we've removed the old
    //item from the queue
    ap->Execute();
  }
}

void EventScheduler::stop() {
  _stop = true; 
}
