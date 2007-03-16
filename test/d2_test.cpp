#include <netmodeler.h>
#include <iostream>
#include <sstream>
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
            int rand_no = (int) (r.getDouble01() * 122);
	    if ( rand_no < 65 ) { rand_no = 65 + rand_no % 56;}
	    if ( (rand_no > 90) && (rand_no < 97) ) { rand_no += 6; }
	    item += (char)rand_no;		  
	}
	items.insert(item);
    }
    return items;
}

int main(int argc, char *argv[]) 
{
    //int max_node = 100000;
    if (argc < 2) {
	    cerr << "Usage: " << argv[0] << ", number of nodes:" << endl;
    }
    int nodes = atoi(argv[1]);
    double alpha = 1;
    Ran1Random ran_no;
    //set the multicasting range (randge0, range1)
    unsigned long int rg_start, rg_end; 
    map<int, pair<double, double> > result;
    //for (int nodes = 100; nodes <= max_node; nodes = nodes*10) {
	//cqsize determines how many rows or columns to multicast.
	//cqsize = sqrt(B) / sqrt(N), where B is total space m*m
	double cqsize = (double) (ADDR_MAX / (double)sqrt( nodes ) * alpha);
	cout << "cqsize: " << cqsize << endl;
	//Make DeetooNetwork for cache.
	auto_ptr<DeetooNetwork> cacheNet_ptr( new DeetooNetwork(nodes, ran_no) );
	cacheNet_ptr->create(nodes);
	cout << "--------" << cacheNet_ptr->getNodeSize() << "\t" << cacheNet_ptr->getEdgeSize() << endl;
	//DeetooNetwork* cacheNet = cacheNet_ptr.get();
        //Insert k items from k randomly selceted nodes into the network.
	
	int k = 100;
	//generate k items.
	std::set<std::string> items = rstringGenerator(k, 10, ran_no );
        std::set<std::string>::iterator item_it;
    	UniformNodeSelector item_src(ran_no);
        item_src.selectFrom(cacheNet_ptr.get() );
	int sum_c_nodes=0, sum_c_edges=0;
	for (item_it = items.begin(); item_it != items.end(); item_it++)
	{
	    //pick a random node to insert an item.
	    AddressedNode* item_source = dynamic_cast<AddressedNode*> (item_src.select() );
            //cout << "item_source's addr: " << item_source->getAddress(true) << endl;	
            //insert the item to item_source node
	    item_source->insertItem(*item_it );
	    //decide cache range
	    //calculate starting column
	    std::pair<unsigned long int, unsigned long int> c_ranges = cacheNet_ptr->getRange(cqsize);
	    unsigned long int rg_start = c_ranges.first, rg_end = c_ranges.second;
	    //cout << "range start: " << rg_start << "\trange end: " << rg_end << endl;
	    //local broadcasting for cache
	    //DeetooMessage* cache_m = new DeetooMessage(*item_it, rg_start, rg_end, true);
	    DeetooNetwork* cacheNet = cacheNet_ptr.get();
	    auto_ptr<DeetooMessage> cache_m ( new DeetooMessage(rg_start, rg_end, true) );
	    auto_ptr<DeetooNetwork> cached_net( cache_m->visit(item_source, *cacheNet) );
	    //cout << "cached net's size: " << cached_net->getNodeSize()  << endl;
	    int c_nodes = cached_net->getNodeSize();
	    sum_c_nodes += c_nodes;
	    int c_edges = cached_net->getEdgeSize();
	    sum_c_edges += c_edges;
	    cout << "c_nodes, c_edges, sumnodes, sumedges: " << c_nodes << "\t" << c_edges << "\t" << sum_c_nodes << "\t" << sum_c_edges << endl;
	    //DeetooNetwork* cached_net = cache_m->visit(item_source, cacheNet_ptr.get() );
	    // Cache this item to all node in the cached_net.
	    auto_ptr<NodeIterator> ni_insert (cached_net->getNodeIterator() );
	    while (ni_insert->moveNext() ) {
		//cout << " in while--------------" << endl;
		AddressedNode* inNode = dynamic_cast<AddressedNode*> (ni_insert->current() );
		inNode->insertItem(*item_it);
		//cout << " in while 2222222--------------" << endl;
	    }
            //cout << " out of while --------------" << endl;
	}
	cout << "sum_c_nodes and sum_c_edges: " << sum_c_nodes << "\t" << sum_c_edges << endl;
	double avg_c_nodes = (double) sum_c_nodes / (double) k;
	double avg_c_edges = (double) sum_c_edges / (double) k;
	/**
	auto_ptr<NodeIterator> ni (cacheNet_ptr->getNodeIterator() );
	while (ni->moveNext() ) {
		AddressedNode* cNode = dynamic_cast<AddressedNode*> ( ni->current() );
		cout << " current node's item size: " << cNode->getItem().size() << endl;
	}
	**/
	//We finished caching all items into the network.
        //Now make another DeetooNetwork for queries.
        //Query each item 100 times
	//Record # of times each message is copied
	//Check hit_rate (count hit)
	//local broadcasting for query.
	
	int total_hits = 0;
	int total_msgs = 0;
	int total_qEdgeSize = 0, hit_nodes_sum;
	int max_it = 10;
	int i = 0;
        auto_ptr<DeetooNetwork> queryNet_ptr ( new DeetooNetwork(ran_no) );
	//queryNet_ptr->createQueryNet( cacheNet->node_map);
	queryNet_ptr->createQueryNet( (cacheNet_ptr.get() )->node_map);
	/**
	auto_ptr<NodeIterator> nic (cacheNet_ptr->getNodeIterator() );
	while (nic->moveNext() ) {
		AddressedNode* cNode = dynamic_cast<AddressedNode*> ( nic->current() );
		cout << " In query net::::current node's item size: " << cNode->getItem().size() << endl;
	}
	**/
	//cout << "number of iteration: \t " << it_no << endl;
	DeetooNetwork* queryNet = queryNet_ptr.get();
	//queryNet->printNetInfo(false);
	cout << "--------" << queryNet_ptr->getNodeSize() << "\t" << queryNet_ptr->getEdgeSize() << endl;
        UniformNodeSelector uns_start(ran_no);
	uns_start.selectFrom(queryNet_ptr.get() );
	for (item_it = items.begin(); item_it != items.end(); item_it++)
	{
	    int no_msg = 0, no_edges = 0;
	    int sum_no_msg = 0, sum_qEdge = 0, sum_hits=0;
	    int reached = 0;
	    cout << i << "th item" << endl;
            for ( int it_no = 0; it_no < max_it; it_no++) {
	        int hits = 0;
		cout << "number of iteration: \t " << it_no << endl;
	        //set starting point
	        AddressedNode* query_start = dynamic_cast<AddressedNode*> (uns_start.select() );
	        std::pair<unsigned long int, unsigned long int> q_ranges = cacheNet_ptr->getRange(cqsize);
	        unsigned long int rg_start = q_ranges.first, rg_end = q_ranges.second;
	        //cout << "range start: " << rg_start << "\trange end: " << rg_end << endl;
	        //which item do you want to retrieve?
		std::string query = *item_it;

	        //DeetooNetwork* visited_net = queryNet->newNetwork();
		//cout << " rg_start,  rg_end      " << rg_start << ",\t" << rg_end << endl;
		//cout << " current node's address:\t " << query_start->getAddress(false) << endl;
	        //DeetooMessage* query_msg = new DeetooMessage(query, rg_start, rg_end, false);
	        auto_ptr<DeetooMessage> query_msg ( new DeetooMessage(rg_start, rg_end, false) );
		//DeetooNetwork* visited_net = query_msg->visit(query_start, *queryNet);
		auto_ptr<DeetooNetwork> visited_net( query_msg->visit(query_start, *queryNet) );
	        // Send a query for this item to all node in the visited_net.
		
	        auto_ptr<NodeIterator> niq (visited_net->getNodeIterator() );
	        while (niq->moveNext() ) {
		  AddressedNode* cNode = dynamic_cast<AddressedNode*> ( niq->current() );
		  //sum_hits = hits + cNode->searchItem(*item_it);
		  hits += cNode->searchItem(*item_it);
	        }
		cout << "hit? " << hits << endl;
		sum_hits += hits;
	        if (hits != 0) 
		{ 
		    reached = reached + 1; 
		    cout << "hit\t hit\t hit\t" << endl;
		}
	        no_msg = visited_net->getNodeSize();
		no_edges = visited_net->getEdgeSize();
		cout << "No. Msgs: \t" << no_msg << endl;
		cout << "no. edges: \t" << no_edges;
	        sum_no_msg = sum_no_msg + no_msg;
	        sum_qEdge = sum_qEdge + no_edges;
	    }
	    cout << "reached: " << reached << "\tsum_hits: " << sum_hits << endl;
	    hit_nodes_sum = hit_nodes_sum + sum_hits; 
	    total_hits = total_hits + reached;
	    total_msgs = total_msgs + sum_no_msg;
	    total_qEdgeSize = total_qEdgeSize + sum_qEdge;
            //double hit_rate = (double)reached / (double)max_it;
            //double msgCount = (double)sum_no_msg / (double)max_it;
            //result[*item_it] = make_pair (hit_rate , msgCount);
	 
        }
        int item_size = items.size();
	//cout << "total hits: \t" << total_hits << endl;
	double hit_rate = (double)total_hits / (double)(max_it * item_size);
	double ave_hit_nodes = (double)hit_nodes_sum / (double)(total_hits);
        double ave_msgs = (double)total_msgs / (double)(max_it * item_size);
        double ave_qEdges = (double)total_qEdgeSize / (double)(max_it * item_size);
        double hops_hit = ave_msgs / hit_rate;
        result[nodes] = make_pair(hit_rate, ave_msgs);	
        //delete cacheNet;
	items.clear();
        cout << "#nodes:\t" << "hit rate:\t" << "ave msgs:\t" << "ave_qEdge:\t" << "hops/hit_rate:\t" << "hitNodes:\t" << "cNodes:\t" << "cEdges:" << endl
		<< nodes << "\t" << hit_rate << "\t" << ave_msgs << "\t" << ave_qEdges << "\t" << hops_hit << "\t" << ave_hit_nodes << "\t" << avg_c_nodes << "\t" << avg_c_edges << endl;
	//delete queryNet;
    //}
    //printInfo(result);
    //cout << "---------------End of Process-----------------------" << endl;

}    


	
    
