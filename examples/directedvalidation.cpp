#include "directednetwork.h"
#include "node.h"
#include "directededge.h"
#include <vector>
#include <map>
#include <iostream>

using namespace std;
using namespace Starsky;

///This demonstrates the directed graph extension to the Netmodeler
// From the examples directory this will compile the example
// 
// g++ -c directedvalidation.cpp -I ../
// g++ -o ditest directedvalidation.o ../netmodlib.a
// ./ditest

int main(int argc, char* argv[]) 
{
	DirectedNetwork digraph;
	Node* star_center = new Node;
	digraph.add(star_center);
	
	vector<Node*> v_arms;
	int num_of_arms = 10;
	Node* t_node = 0;
	while (num_of_arms-- > 0)
	{
		t_node = new Node;
		if (0 != t_node)
		{
			digraph.add(t_node);
			digraph.add(DirectedEdge( t_node,star_center));
			
		}
	}
	
	map<int,int> in_deg_dist = digraph.getInDegreeDist();
	map<int, int>::const_iterator in_deg_dist_it;
	cout <<"Since the network is a star with all edges pointing to toward the hub,"<<endl;
	cout <<"the in-degree should be zero for all nodes except one which will "<<endl;
	cout <<"have an in-degree of N-1, where N is the number of nodes."<<endl;
	cout <<endl;
	cout <<"In-degree distribution:"<<endl;
	cout <<"in-degree"<<"\t"<<"population"<<endl;
	for(in_deg_dist_it = in_deg_dist.begin();in_deg_dist_it !=  in_deg_dist.end();  in_deg_dist_it++) 
	{
      cout << in_deg_dist_it->first << "\t" << in_deg_dist_it->second << endl;
	}
	
	cout <<endl;
	cout <<"Out-degree distribution:"<<endl;
	cout <<"out-degree"<<"\t"<<"population"<<endl;	
	map<int,int> out_deg_dist = digraph.getOutDegreeDist();
	map<int, int>::const_iterator out_deg_dist_it;
	for(out_deg_dist_it = out_deg_dist.begin();out_deg_dist_it !=  out_deg_dist.end();  out_deg_dist_it++) 
	{
      cout << out_deg_dist_it->first << "\t" << out_deg_dist_it->second << endl;
	}
  cout <<endl;
	cout <<"Average in-degree"<<endl;
	cout << digraph.getInDegreeMoment(1)<<endl;
	cout <<"Average out-degree"<<endl;
	cout << digraph.getOutDegreeMoment(1)<<endl;

	set<Node*> center_in_neighbors = digraph.getInNeighbors(star_center);
	set<Node*>::const_iterator in_neigh_it = center_in_neighbors.begin();
	cout<<endl;
	cout <<"Memory addresses of nodes that share a directed edge that terminates at"<<endl;
	cout <<"the center of the star."<<endl;
	while (in_neigh_it !=center_in_neighbors.end() )
	{
		cout << *in_neigh_it <<endl;
		in_neigh_it++;
  }
	cout<<"----------"<<endl;
	
	set<Node*> center_out_neighbors = digraph.getOutNeighbors(star_center);
	set<Node*>::const_iterator out_neigh_it = center_out_neighbors.begin();
	cout <<endl;
	cout <<"Memory addresses of nodes that share a directed edge that originates at the"<<endl; 
	cout <<"center of the star."<<endl;
	while (out_neigh_it !=center_out_neighbors.end() )
	{
		cout << *out_neigh_it <<endl;
		out_neigh_it++;
	}
	cout<<"----------"<<endl;
	
	return 0;

}
