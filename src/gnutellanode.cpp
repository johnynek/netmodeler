#include "gnutellanode.h"

using namespace Starsky;
using namespace std;

GnutellaNode::GnutellaNode(const string& address,
		           const string& vendor,
			   bool is_ultra,
			   int distance) : _address(address),
			                   _vendor(vendor),
					   _is_ultrapeer(is_ultra),
					   _distance(distance) {

}

string GnutellaNode::toString() const {
  return _address;
}
