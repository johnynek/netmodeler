#include <netmodeler.h>
#include <fstream>
#include <cmath>

using namespace Starsky;
using namespace std;
#define ADDR_MAX 65536

void printInfo(map<int, pair<double, double> > result) {
	ofstream myfile("output1");
	myfile << "#nodes: " << "\t" << "hit rate" << "\t" << "ave msgs" << "\t" << "hops/hit_rate" << endl;
	map<int,pair<double, double> >::iterator it;
	for (it=result.begin(); it!=result.end(); it++) {
		double hops_ps = it->second.second / it->second.first;
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
    double alpha = 1;
    //int max_node = 101;
    Ran1Random ran_no;
    //Random r;
    //set the multicasting range (randge0, range1)
    unsigned long int rg_start, rg_end; 
    map<int, pair<double, double> > result;
    for (int nodes = 100; nodes <= max_node; nodes = nodes*10) {
	//cout << "nodes:\t" << nodes << endl;
	//cqsize determines how many rows or columns to multicast.
	//cqsize = sqrt(B) / sqrt(N), where B is total space m*m
	int cqsize = (int) (ADDR_MAX / (int)sqrt( nodes ) * alpha);
        //cout << "-------------------------------------------" << endl;
	//cout << "cqsize" << cqsize << endl;	
	//Make DeetooNetwork for cache.
	//DeetooNetwork* cacheNet = new DeetooNetwork(nodes, ran_no);
	auto_ptr<DeetooNetwork> cacheNet_ptr( new DeetooNetwork(nodes, ran_no) );
	//cout << "++++++++++++++++++++++++++++++++++" << endl;
	cacheNet_ptr->create(nodes);
	DeetooNetwork* cacheNet = cacheNet_ptr.get();
        //Insert k items from k randomly selceted nodes into the network.
	int k = 100;
	//generate k items.
	//cout << "++++++++++++++++++++++++++++++++++" << endl;
	std::set<std::string> items = rstringGenerator(k, 10, ran_no );
        std::set<std::string>::iterator item_it; 
	for (item_it = items.begin(); item_it != items.end(); item_it++)
	{
	    //pick a random node to insert an item.
	    //cout << "Let's insert items" << endl;
	    UniformNodeSelector item_src(ran_no);
	    item_src.selectFrom(cacheNet);
	    AddressedNode* item_source = dynamic_cast<AddressedNode*> (item_src.select() );
            //cout << "item_source's addr: " << item_source->getAddress(true) << endl;	
            //insert the item to item_source node
	    item_source->insertItem(*item_it );
	    //decide cache range
	    //calculate starting column
	    //cout << " **start range setup start " << endl;
	    int start_col = ( (int)(item_source->addr_i) - (int)(cqsize/2) );
	    //cout << "start column:\t" << start_col << endl;
	    if ( start_col < 0 ) {  //if starting node is in the first column
		start_col = 0;
                rg_start = 0;       // start in the first column
	    }
	    else
	    {
		  //otherwise start in the cqsize/2 left column from the column with initiating node.
	          rg_start = (unsigned long int)( start_col)*ADDR_MAX;
            }
	    //rg_start = (unsigned long int)(query_start->addr_j - 2/cqsize)*ADDR_MAX;
	    //cout << " ----end range setup start " << endl;
	    //cout << "rg_start:\t" << rg_start << endl;
	    //int end_col = ( (int)(rg_start / ADDR_MAX) + cqsize);
	    int end_col =  (int)(start_col + cqsize -1 );
	    //cout << "end column \t" << end_col << endl;
	    if ( end_col >= (ADDR_MAX-1) )
	    {
		// If the upper range is greater than column size
		// set the upper range to the last column
		//cout << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ " << endl;
	        rg_end = 4294967295;
		//cout << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ " << endl;
	        rg_start = (unsigned long int)(  (ADDR_MAX-cqsize) * ADDR_MAX );
	    }
	    else
	    {
		//cout << " =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;    
	        //rg_end = (unsigned long int)( ( ( start_col + cqsize - 1) * ADDR_MAX) + ADDR_MAX -1);
	        rg_end = (unsigned long int)( ( end_col * ADDR_MAX) + ADDR_MAX -1);
	    }
	    //cout << "range start: " << rg_start << "range end: " << rg_end << endl;
	    //local broadcasting for cache
	    //cout << "start range and end range:\t" << rg_start << "\t" << rg_end << endl;
	    //DeetooMessage* cache_m = new DeetooMessage(*item_it, rg_start, rg_end, true);
	    auto_ptr<DeetooMessage> cache_m ( new DeetooMessage(*item_it, rg_start, rg_end, true) );
	    //seg fault in visit
	    //DeetooNetwork* cached_net = cache_m->cacheItems(item_source, *cacheNet);
	    cache_m->cacheItems(item_source, cacheNet);
	    //delete cache_m;
	}
	//We finished caching all items into the network.
        //Now make another DeetooNetwork for query.
        //DeetooNetwork* queryNet = new DeetooNetwork(ran_no);
	//cout << "++++++++++++++++++++++++++++++++++" << endl;
	//queryNet->createQueryNet(cacheNet->node_map);
	//cout << "++++++++++++++++++++++++++++++++++" << endl;
        //Query each item 100 times
	//Record # of times each message is copied
	//Check hit_rate (count hit)
	//local broadcasting for query.
	
	//int f_reached = 0;
	//int f_ 
	int total_hits = 0;
	int total_msgs = 0;
	int max_it = 100;
	for (item_it = items.begin(); item_it != items.end(); item_it++)
	{
	    int no_msg = 0;
	    int reached=0;
	    int sum_no_msg = 0;
            for ( int it_no = 0; it_no < max_it; it_no++) {
		//DeetooNetwork* queryNet = new DeetooNetwork(ran_no);
		auto_ptr<DeetooNetwork> queryNet_ptr ( new DeetooNetwork(ran_no) );
		queryNet_ptr->createQueryNet(cacheNet->node_map);
	        //set starting point
		//cout << "number of iteration: \t " << it_no << endl;
		DeetooNetwork* queryNet = queryNet_ptr.get();
	        UniformNodeSelector uns_start(ran_no);
	        uns_start.selectFrom(queryNet);
	        AddressedNode* query_start = dynamic_cast<AddressedNode*> (uns_start.select() );
		//
	        int start_row = ( (int)(query_start->addr_j) - (int)(cqsize/2) );
	        if ( start_row < 0 ) {
		  start_row = 0;
		  rg_start = 0;
	        }
	    	else
	    	{
	          rg_start = (unsigned long int)( start_row)*ADDR_MAX;
                
	    	}
	        //rg_start = (unsigned long int)(query_start->addr_j - 2/cqsize)*ADDR_MAX;
		int end_row =  (int)(start_row + cqsize -1 );
		if ( end_row >= (ADDR_MAX-1) )
		{
		    //rg_end = (ADDR_MAX-1)*ADDR_MAX + (ADDR_MAX-1);
		    rg_end = 4294967295;
		    rg_start = (unsigned long int)(  (ADDR_MAX-cqsize) * ADDR_MAX );
		}
		else
		{
		    //rg_end = (unsigned long int)( ( ( start_row + cqsize - 1) * ADDR_MAX) + ADDR_MAX -1);
		    rg_end = (unsigned long int)( ( end_row * ADDR_MAX) + ADDR_MAX -1);
		}
	        //which item do you want to retrieve?
		std::string query = *item_it;

	        //DeetooNetwork* visited_net = queryNet->newNetwork();
		//cout << " rg_start,  rg_end " << rg_start << ",\t" << rg_end << endl;
		//cout << " current node's address:\t " << query_start->getAddress(false) << endl;
	        //DeetooMessage* query_msg = new DeetooMessage(query, rg_start, rg_end, false);
	        auto_ptr<DeetooMessage> query_msg ( new DeetooMessage(query, rg_start, rg_end, false) );
		//cout << "*********************************************" << endl;
		//DeetooNetwork* visited_net = query_msg->visit(query_start, *queryNet);
		auto_ptr<DeetooNetwork> visited_net( query_msg->visit(query_start, *queryNet) );
		//cout << "-----------------***********************************" << endl;
	        if (query_msg->hit) 
		{ 
		    reached = reached + 1; 
		    //cout << "hit\t hit\t hit\t" << endl;
		}
	        no_msg = visited_net->getNodeSize();
	        sum_no_msg = sum_no_msg + no_msg;
		//delete queryNet;
                //delete visited_net;
                //delete query_msg;
                //delete visited_net;
	    }
	    total_hits = total_hits + reached;
	    total_msgs = total_msgs + sum_no_msg;
            //double hit_rate = (double)reached / (double)max_it;
            //double msgCount = (double)sum_no_msg / (double)max_it;
            //result[*item_it] = make_pair (hit_rate , msgCount);
	 
        }
        int item_size = items.size();
	cout << "total hits: \t" << total_hits << endl;
	double hit_rate = (double)total_hits / (double)(max_it * item_size);
        double ave_msgs = (double)total_msgs / (double)(max_it * item_size);
	cout << "hit rate:\t" << hit_rate << endl;
	cout << "ave_msgs:\t" << ave_msgs << endl;
	//double hit_rate = (double)total_hits / (double)(100 * item_size);
        //double ave_msgs = (double)total_msgs / (double)(100 * item_size);
        result[nodes] = make_pair(hit_rate, ave_msgs);	
        //delete cacheNet;
	//result_map[nodes]=
	items.clear();
    }
    printInfo(result);
    cout << "---------------End of Process-----------------------" << endl;
}    


	
    
