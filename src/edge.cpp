#include "edge.h"
using namespace Starsky;

Edge::Edge (Node * start, Node * end, double a_weight)
: weight(a_weight)
{
  if( start < end ) {
    first = start;
    second = end;
  }
  else {
    first = end;
    second = start;
  }
}

bool Edge::connects (Node * start, Node * end) const
{
  return ((first == start) && (second == end)) ||
    ((first == end) && (second == start));
}

Node* Edge::getOtherNode (Node * start) const
{
  if (start == first) {
    return second;
  }
  else if (start == second) {
    return first;
  }
  else {
    return 0;
  }
}

bool Edge::hasNode (Node * node) const
{
  return (first == node) || (second == node);
}

double Edge::getWeight()
{
  return weight;
}
		
void Edge::setWeight(double a_weight)
{
  weight = a_weight;
}

std::string Edge::toString() const {
  return first->toString() + " : " + second->toString();
}

bool Edge::operator<(const Edge& b) const
{
  if( first == b.first) {
    return (second < b.second);
  }
  else {
    return (first < b.first);
  }
}

bool Edge::operator==(const Edge& a) const
{
  return (first == a.first) && (second == a.second);
}
