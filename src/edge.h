#ifndef starsky__edge_h
#define starsky__edge_h

#include "node.h"

namespace Starsky {

  /**
   * Simple object to store pointers to two nodes
   * @see Starsky::Node
   * @see Starsky::Network
   * @see Starsky::DirectedEdge
   */
  class Edge {
  public:
    ///the Node* with the lowest memory address 
    Starsky::Node * first;
    ///the Node* with the lowest memory address
    Starsky::Node * second;

    /**
     * creates an edge between start and end.  For undirected nodes, order does
     * not matter.  @see Starsky::DirectedNode
     */
    Edge (Node * start = 0, Node * end = 0, double a_weight=1.0);
    /**
     * Make an edge with the given attributes.  This
     * edge only supports a weight (double in the string)
     */
    Edge(Node* start, Node* end, const std::string& attrs);

    virtual ~Edge() { };
    /**
     * @param start
     * @param end
     * @return true if this edge has both start and end
     */
    virtual bool connects (Node * start, Node * end) const;
    /**
     * @return first if passed second, second if passed first, and 0 otherwise
     */
    Node * getOtherNode (Node * start) const;
    /**
     * Returns the attributes of this edge.  Useful in EdgeFactories
     */
    virtual std::string getAttributes() const;
    /**
     * @return true of this Node is first or second
     */
    bool hasNode (Node *) const;
    virtual std::string toString() const;

    double getWeight() const;
    void setWeight(double a_weight);
			
    virtual bool operator<(const Edge& a) const;
    virtual bool operator==(const Edge& a) const;
	protected:
		///the weight of the edge
		double weight;
	
  };

}
#endif
