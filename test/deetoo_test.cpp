#include <netmodeler.h>
#include <fstream>
#include <cmath>

using namespace Starsky;
using namespace std;
#define ADDR_MAX 65536

void printInfo(map<int, pair<double, double> > result) {
	ofstream myfile("output");
	myfile << "#nodes: " << "\t" << "#ave_pathlength" << "\t" << "hit rate" << "\t" << endl;
	map<int,pair<double, double> >::iterator it;
	for (it=result.begin(); it!=result.end(); it++) {
		double hops_ps = it->second.first / it->second.second;
		myfile << it->first << "\t" << it->second.first << "\t" << it->second.second << "\t" << hops_ps << endl;
	}
}
//random string generator
std::set<std::string> rstringGenerator ( int howmany, int length, Ran1Random& r )
{
    std::set<std::string> items;
    int k = howmany;
    for (int no=0; no < k; no++)
    {
	std::string item;
	
	for (int i = 0; i < length; i++)
	{
            //int rand_no = r.getInt(122,0);
            int rand_no = (int) (r.getDouble01() * 122);
	    if ( rand_no < 65 ) { rand_no = 65 + rand_no % 56;}
	    if ( (rand_no > 90) && (rand_no < 97) ) { rand_no += 6; }
	    item += (char)rand_no;		  
	}
	items.insert(item);
    }
    return items;
}

int main(void)
{
    int max_node = 100000;
    Ran1Random ran_no;
    //Random r;
    //set the multicasting range (randge0, range1)
    unsigned long int rg_start, rg_end; 
    map<std::string, pair<double, double> > result;
    for (int nodes = 100; nodes <= max_node; nodes = nodes*2) {
	//cqsize determines how many rows or columns to multicast.
	//cqsize = sqrt(B) / sqrt(N), where B is total space m*m
	int cqsize = (int) ADDR_MAX / (int)sqrt( nodes );
        //cout << "-------------------------------------------" << endl;
	cout << "cqsize" << cqsize << endl;	
	//Make DeetooNetwork for cache.
	DeetooNetwork* cacheNet = new DeetooNetwork(nodes, ran_no);
	//cout << "++++++++++++++++++++++++++++++++++" << endl;
	cacheNet->create(nodes);
        //Insert k items from k randomly selceted nodes into the network.
	int k = 100;
	//generate k items.
	//cout << "++++++++++++++++++++++++++++++++++" << endl;
	std::set<std::string> items = rstringGenerator(k, 10, ran_no );
        std::set<std::string>::iterator item_it; 
	for (item_it = items.begin(); item_it != items.end(); item_it++)
	{
	    //pick a random node to insert an item.
	    UniformNodeSelector item_src(ran_no);
	    item_src.selectFrom(cacheNet);
	    AddressedNode* item_source = dynamic_cast<AddressedNode*> (item_src.select() );
            cout << "item_source's addr: " << item_source->getAddress(true) << endl;	
            //insert the item to item_source node
	    item_source->insertItem(*item_it );
	    //decide cache range
	    //calculate starting column
	    int start_cl = ( (int)(item_source->addr_i) - (int)(cqsize/2) );
	    if ( start_cl < 0 ) {
		rg_start = 0;
	    }
	    else
	    {
	        rg_start = (unsigned long int)( start_cl)*ADDR_MAX;
                
	    }
	    rg_end = (unsigned long int) ( ( ( (int)(rg_start / ADDR_MAX) + cqsize) * ADDR_MAX) + ADDR_MAX -1);
	    cout << "range start: " << rg_start << "range end: " << rg_end << endl;
	    //local broadcasting for cache
	    DeetooMessage* cache_m = new DeetooMessage(*item_it, rg_start, rg_end, true);
	    cout << "++++++++++++++++++++++++++++++++++" << endl;
	    DeetooNetwork* cached_net = cache_m->visit(item_source, *cacheNet);
	    cout << "++++++++++++++++++++++++++++++++++" << endl;
	}
	cout << "++++++++++++++++++++++++++++++++++" << endl;
	//We finished caching all items into the network.
        //Now make another DeetooNetwork for query.
        DeetooNetwork* queryNet = new DeetooNetwork(ran_no);
	queryNet->createQueryNet(cacheNet->node_map);
        //Query each item 100 times
	//Record # of times each message is copied
	//Check hit_rate (count hit)
	//local broadcasting for query.
	
	//int f_reached = 0;
	//int f_ 
	for (item_it = items.begin(); item_it != items.end(); item_it++)
	{
	    int max_it = 100;
	    int no_msg = 0;
	    int reached;
	    int sum_no_msg = 0;
            for ( int it_no = 0; it_no < max_it; it_no++) {
	        //set starting point
	        UniformNodeSelector uns_start(ran_no);
	        uns_start.selectFrom(queryNet);
	        AddressedNode* query_start = dynamic_cast<AddressedNode*> (uns_start.select() );
	        rg_start = (unsigned long int)(query_start->addr_j - 2/cqsize)*ADDR_MAX;
	        rg_end = (unsigned long int)( ( ( rg_start / ADDR_MAX) + cqsize) * ADDR_MAX + ADDR_MAX -1);
	        //which item do you want to retrieve?
		std::string query = *item_it;

	        //DeetooNetwork* visited_net = queryNet->newNetwork();
	        DeetooMessage* query_msg = new DeetooMessage(query, rg_start, rg_end);
		DeetooNetwork* visited_net = query_msg->visit(query_start, *queryNet);
	        if (query_msg->hit) { reached = reached + 1; }
	        no_msg = visited_net->getNodeSize();
	        sum_no_msg = sum_no_msg + no_msg;
                delete visited_net;
                delete query_msg;
                //delete visited_net;
	    }
            double hit_rate = (double)reached / (double)max_it;
            double msgCount = (double)sum_no_msg / (double)max_it;
            result[*item_it] = make_pair (hit_rate , msgCount);
        }
        int item_size = items.size();
        delete cacheNet;
        delete queryNet;	
	//result_map[nodes]=
    }
}    


	
    
