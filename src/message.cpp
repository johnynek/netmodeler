#include "message.h"

using namespace Starsky;
using namespace std;

void Message::forgetVisitedNodes() {
  _visited_nodes.clear();
  _crossed_edges = 0;
}

const set<Node*>& Message::getVisitedNodes() const {
  return _visited_nodes;
}
