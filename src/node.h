#ifndef starsky__node_h
#define starsky__node_h

#include <string>
#include <sstream>

namespace Starsky {

  /**
   * Base node class.  Subclass this to make different types of nodes.
   * We usually work with pointers to nodes, so we don't have to store O(N^2)
   * copies of the data associated with each node.
   * @see Starsky::Network
   * @see Starsky::Edge
   */
  class Node {
  public:
      Node();
      Node(const std::string& s) { _name = s; }
      virtual ~Node() { };
      /**
       * @return a string suitable for printing
       */
      virtual std::string toString() const {
        if( _name != "" ) {
          return _name;
	}
	std::stringstream s;
	s << "n" << this;
        return s.str();
      }
  protected:
      static int _node_count;
      std::string _name;
  };

}
#endif
