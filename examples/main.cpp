#include "network.h"
#include "debiandependencynetwork.h"
#include "node.h"
#include "edge.h"

#include "ran1random.h"
#include "randomnetwork.h"
#include "prefatnetwork.h"
#include "doubleprefatnetwork.h"
#include "randomdirectednetwork.h"
#include "callawaynetwork.h"
#include "polypreference.h"
#include "exppreference.h"
#include "simkinnetwork.h"
#include "vazquezdirectednetwork.h"
#include "contentnetwork.h"
#include "contentnode.h"
#include "powerlawprobabilityfunction.h"
#include "degreelawrandomnetwork.h"
#include "prefdelcompnetwork.h"

#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

	//Verify the assortativity calculation on a known case:
	/**
	 * the callaway network should have assortativity p/(1+2p)
	 */
	DynamicNetwork* my_net = 0;

	Ran1Random my_rand;
	double p = 0.0;
	cout << "#c <k> clustercoeff assort n" << endl;
	while( p < 1.0 ) {
	    //my_net = new CallawayNetwork(10000, p, my_rand);
	    my_net = new PrefDelCompNetwork(RandomNetwork(20, 0.5, my_rand),
			          my_rand,
				  p,
				  1,
				  0.75); //Only restore connections with q=0.75
	   
	    while( my_net->getNodes().size() < 20000) {
                my_net->incrementTime(1);
	    }
	    cout << p << " "
	         << my_net->getAverageDegree() << " "
		 << my_net->getClusterCoefficient() << " "
		 << my_net->getAssortativity() << " "
	         << my_net->getNodes().size() << endl;
	    my_net->clear();
	    delete my_net;
            p += 0.1;
	}
	
  return 1;
}
