#include "directededge.h"

using namespace Starsky;
using namespace std;

DirectedEdge::DirectedEdge(Node* start, Node* end) {
    if( start < end) {
        first = start;
	second = end;
	_dir_first_to_second = true;
    }
    else {
        first = end;
	second = start;
	_dir_first_to_second = false;
    }
}

bool DirectedEdge::connects(Node* start, Node* end) const {
  if( _dir_first_to_second ) {
    return (start == first) && (end == second);
  }
  else {
    return (start == second) && (end == first);
  }
}

Node* DirectedEdge::getStartNode() const{
    if( _dir_first_to_second == true) {
      return first;
    }
    else {
      return second;
    }
}

Node* DirectedEdge::getEndNode() const{
    if( _dir_first_to_second == true) {
      return second;
    }
    else {
      return first;
    }
}

void DirectedEdge::reverse() {
    //Switch the direction:
    if( _dir_first_to_second == true) {
      _dir_first_to_second = false;
    }
    else {
      _dir_first_to_second = true;
    }
}

std::string DirectedEdge::toString() const {
  if( _dir_first_to_second ) {
     return "from " + first->toString() + " to " + second->toString();
  }
  else {
     return "from " + second->toString() + " to " + first->toString();
  }  
}

bool DirectedEdge::operator<(const Edge& a) const {

    try {
      //See if this is a directed edge
      const DirectedEdge& d_a = dynamic_cast<const DirectedEdge&>(a);
    
      bool first_eq = (first == d_a.first);

      if( first_eq ) {
	bool second_eq = (second == d_a.second);
        if( second_eq ) {
           return (!_dir_first_to_second) && d_a.pointsFirstToSecond();
	}
	else {
           return (second < d_a.second);
	}
      }
      else {
        return (first < d_a.first);
      }
    }
    catch(std::bad_cast x) {
      //If it is not a subclass of DirectedEdge, it is less than directed edge.
      return true; 
    }
}

bool DirectedEdge::operator==(const Edge& a) const {
  try {
    const DirectedEdge& d_a = dynamic_cast<const DirectedEdge&>(a);
    if ( _dir_first_to_second ) {
      return d_a.connects(first, second);
    }
    else {
      return d_a.connects(second, first);
    }
  }
  catch(std::bad_cast x) {
    //If it is not a subclass of DirectedEdge, it is not equal
    return false;
  }
}
