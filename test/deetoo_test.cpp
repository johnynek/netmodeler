#include <netmodeler.h>
#include <fstream>

using namespace Starsky;
using namespace std;

void printInfo(map<int, pair<double, double> > result) {
	ofstream myfile("output");
	myfile << "#nodes: " << "\t" << "#ave_pathlength" << "\t" << "hit rate" << "\t" << endl;
	map<int,pair<double, double> >::iterator it;
	for (it=result.begin(); it!=result.end(); it++) {
		double hops_ps = it->second.first / it->second.second;
		myfile << it->first << "\t" << it->second.first << "\t" << it->second.second << "\t" << hops_ps << endl;
	}
}

int main(void)
{
    int max_node = 100000;
    Ran1Random ran_no;
    map<int, pair<double, double> > result;
    for (int nodes = 100; nodes <= max_node; nodes = nodes*2) {
	//Make DeetooNetwork for cache.
	DeetooNetwork* cacheNet = new DeetooNetwork(ran_no);
	cacheNet->create(nodes);
        
	//insert items
	//insert an item to a randomly chosen node in cacheNet
	//item_node->insertItem(content);
	//local broadcasting for cache
	DeetooNetwork* cachedNet = cacheNet->newNetwork();
	//DeetooMessage* cache_msg = new Deetoomessage();
	//cache_msg->
	
        //Make DeetooNetwork for query.
        DeetooNetwork* queryNet = new DeetooNetwork(ran_no);
	queryNet->createQueryNet(cacheNet->node_map);
        
	//local broadcasting for query.
	int max_it = 100;
	int no_msg = 0;
	int reached;
	double sum_no_msg = 0;
        for ( int it_no = 0; it_no < max_it; it_no++) {
	    //set starting point
	    UniformNodeSelcetor uns_source1(ran_no);
	    uns_source1.selectFrom(queryNet);
	    //which item do you want to retrieve?
	    std::string query;
	    int r0, r1;
	    DeetooMessage* query_msg = new Deetoomessage(query, r0, r1);
	    DeetooNetwork* visited_net = queryNet->newNetwork();
	    
	    query_msg->visit();
            if ( visited_net->has(target) ) {
	        reached = reached+1;
	    }
	    no_msg = visited_net.getNodeize();
	    sum_no_msg = sum_no_msg + no_msg;
            //delete nets;
            //delete msg;
            //delete visited_net;
	}
        double hit_rate = (double)reached / (double)max_it;
        double msgCount = (double)sum_no_msg / (double)max_it;
        result[nodes] = make_pair (hit_rate , msgCount);
    }
}    


	
    
