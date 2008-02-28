/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2008  P. Oscar Boykin <boykin@pobox.com>, University of Florida

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

#ifndef starsky__event_h
#define starsky__event_h

//For priority queue
#include <queue>

namespace Starsky {

///Represents an action taken at some point in time
/**
 * Subclass this to make your own Action objects for the
 * EventScheduler.  After Execute runs, the EventScheduler
 * will delete the object, bear that in mind
 */
class Action {

  public:
  ///Causes the action to run.
  virtual void Execute() = 0;

};

/**
 * This holds an action and the time to run the action
 * Used internally in EventScheduler
 */
struct Event {
  Event(double at, Action* a) : RunAt(at), ToDo(a) {
  }
  double RunAt;
  Action* ToDo;
  /**
   * This is for use in the priority queue, which puts
   * larger items first.  But we want earlier times to
   * go first, so an Event is less than, if the time is
   * later
   */
  bool operator<(const Event& e) const {
    return e.RunAt < RunAt;
  }
};

/**
 * Allows you to schedule events to happen at given
 * times or after a given time.
 */
class EventScheduler {

  public:
    EventScheduler();
    ///Deletes any unexecuted Actions in the queue
    ~EventScheduler();
    ///After a given interval of time run the given action
    void after(double interval, Action* a) {
      double time = getCurrentTime() + interval;
      at(time, a);
    }
    ///At a given time, execute a given Action, delete the Action after
    void at(double time, Action* a);
    ///get the current time
    double getCurrentTime() const { return _time; }

    ///Start running the queue.
    /**
     * Returns when the queue is empty, or it has
     * been stopped (by some Action)
     */
    void start();
    /**
     * Temporarily stop running the queue.  Can be called from an Action
     * that is executing.  Causes the start method to return with some items
     * still left in the queue.  The next call to start will pick up where
     * the queue left off
     */
    void stop();
  protected:
    std::priority_queue<Event> _event_queue;
    double _time;
    bool _stop;
};

///Stops the EventScheduler when executed.
/**
 * Allows us to schedule a stop at some point in the future
 */
class StopAction : public Action {

  public:
    StopAction(EventScheduler& sched) : _sched(sched) { }
    void Execute() { _sched.stop(); }
  protected:
    EventScheduler& _sched;
};


}

#endif
