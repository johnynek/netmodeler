#ifndef starsky__emailnetwork_h
#define starsky__emailnetwork_h

#include <map>
#include <set>
#include <iostream>

#include "network.h"
#include "directedweightednetwork.h"
#include "node.h"
#include "edge.h"

/**
 * This class is specifically designed to process the network from the paper "Scale-free topology of
 * email network" by Ebel et.al..
 */

namespace Starsky {
	
class EmailNetwork : public DirectedWeightedNetwork {

    public:
  
	EmailNetwork();
	/**
	 * @param in the input file to construct the directed weighted network.  
	 */
	EmailNetwork(std::istream& in);

        //int getInternalSize(NodePSet& nodes);
	int getInternalSize();

	int getInternalOutSize();

	std::map<int, double> getInternalOutDegtoAverageStrengthMap() const;

        double getInternalOutDegStrengthCoefficient();

	double getInternalOutDegAverageStrengthCoefficient();

	std::map<double, int> getInternalNodeOutStrengthDist();

	void removeSmallComponents();
       
	void printInternalOutStrengthDist(std::ostream& out);

	void printOutDegStrengthMap(std::ostream& out);

	void removeSinkNodes();

	void setInternalNodes(NodePSet& nodes);
	void setInternalNodes();

	void setInternalOutNodes(NodePSet& nodes);
	void setInternalOutNodes();

 protected:
	NodePSet internal_node_set;
	NodePSet internal_out_node_set;

};
	
}

#endif

