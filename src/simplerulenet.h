#ifndef starsky__simplerulenet
#define starsky__simplerulenet

#include "network.h"
#include "incrementable.h"

namespace Starsky {

 /**
  * This abstract class is used to model networks which have
  * growing graphs.
  *
  * The general rule is the following:
  * At each time step, a new node is created and added to
  * the network.  The node is then connected to some number
  * of nodes @see SimpleRuleNet::getConnectionCount.
  * 
  * Then a set of nodes are selected to remove (@see
  * SimpleRuleNet::getRemoveNodes).  If the average of that
  * function is note less than 1, the network will not be
  * growing in size.
  *
  * Then for each of the above nodes, check to see if the
  * their deleted edges will need compensation (@see
  * SimpleRuleNet::needsCompensation).
  *
  * The function that describes the above general algorithm
  * is SimpleRuleNet::incrementTime.  That function IS NOT
  * VIRTUAL.  All subclasses share the same above approach.
  * The only changes in the algorithm are made to how the
  * various selection functions are implemented.
  * 
  */
	
class SimpleRuleNet : public Network,
                      public Incrementable {

    public:
	/**
	 * step forward in time.  This is function models the evolution of
	 * a network.  This function is common to all
	 * subclasses
	 * @param steps how many steps into the future to take.  Default = 1.
	 */
        void incrementTime(int steps = 1);

	/**
	 * In a rule based network, often times, a node is created
	 * and then one or more connections are made according to
	 * some rule.  This method implements that rule.  Note
	 * that the method is constant, hence, once the partner
	 * is found, and Edge must still be added, and if the
	 * parameter node (start) is new, it must also be added.
	 * @param start the Node which is looking for a new partner in an edge
	 * @return the node which was selected to be the partner.
	 */
        virtual Node* findPartnerFor(Node* start) = 0;

	/**
	 * How many connections should a node make when
	 * joining the network.  Could be any function
	 * (including a random one).  By default it is 1
	 */
	virtual int getConnectionCount(Node* node) {return 1;}

        /**
	 * Get a set of nodes to remove.  All of their edges
	 * will be checked to see if they need to be
	 * compensated for.  If so, the findPartnetFor
	 * function will be called for each node needing
	 * compensation.  By default, no nodes are deleted.
	 */
	virtual const NodePSet getRemoveNodes()
	             {return Network::_empty_nodeset;}
	
	/**
	 * Method which allows us a general way to deal
	 * with edge compensation when a node leaves.
	 * 
	 * @param node the node in the network that may
	 * compensate for a lost edge
	 * @param edge the edge in question
	 * @returns true if the edge would be compensated if
	 * lost.
	 * By default, this is always false.
	 */
	virtual bool needsCompensation(Node* node, Edge* edge) {return false;} 
	/**
	 * This is a catchall method called at the end of
	 * each step.  We pass the node, which may or may
	 * not be of use.  By default, nothing happens.
	 */
	virtual void postNodeAdd(Node* node) { }
};
	
}

#endif
