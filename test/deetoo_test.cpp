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
    map<int, pair<double, double> > result;
    for (int nodes = 100; nodes <= max_node; nodes = nodes*2) {
	//Make smsnetwork for cache.
	Smsnetwork* cacheNet = new Smsnetwork();
	cacheNet->create();
        
	//insert items
	cacheNet->insertContent();
	//local broadcasting for cache
	Smsnetwork* cachedNet = cacheNet->newNetwork();
	Smsmessage* cache_msg = new Smsmessage();
	cache_msg->
	
        //Make smsnetwork for query.
        Smsnetwork* queryNet = new Smsnetwork();
	queryNet->createQueryNet(cacheNet.node_map);
        
	//local broadcasting for query.
	int max_it = 100;
	int no_msg = 0;
	double sum_no_msg = 0;
        for ( int it_no = 0; it_no < max_it; it_no++) {
	    //set starting point
	    uniformNodeSelcetor usn_source1(rand);
	    uns_source1.selectFrom(queryNet)
	    SmsMessage* query_msg = new Smsmessage();
	    Smsnetwork* visited_net = queryNet->newNetwork();
	    
	    query_msg->visit();
            if ( visited_net->has(target) ) {
	        reached = reached+1;
	    }
	    no_msg = visited_net.size();
	    sum_no_msg = sum_no_msg + no_msg;
            delete nets;
            delete msg;
            delete visited_net;
	}
        double hit_rate = (double)reached / (double)max_it;
        double msgCount = (double)sum_no_msg / (double)max_it;
        result[nodes] = make_pair (hit_rate , msgCount);
    }
}    


	
    
