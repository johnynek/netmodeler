/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <netmodeler.h>
#include <ctime>

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

     //Here we consider a network for a P2P topology
     Ran1Random r1(time(NULL)), r2(-1000), r3(-1000000), r4(-5678);
     ContentNetwork* my_cnet = 0;
     Node* a_node = 0;
     ContentNode* c_node = 0;
     vector<Node*> node_vector;
     
     //Here is all the content
     vector<ContentNode*> content_vector;
     int cont_count = 10; 
     //Look for each content 10 times:
     int runs = 10*cont_count;
     content_vector.reserve(cont_count);
     for(int i = 0; i < cont_count; i++) {
         content_vector.push_back(new ContentNode);
     }
     
     Network* my_net = 0;
     //my_net = new PrefDelCompNetwork(RandomNetwork(10,1.0,r1),r1,0.75,1,0.6666);
     //my_net = new DoublePrefAtNetwork( RandomNetwork(10,0.6,r1), r1, 1 );
     //my_net = new PrefAtNetwork( RandomNetwork(2,1.0,r1), r1, 1 );
     //ifstream input("internal.dat");
     //my_net = new Network(input);
     /*while(my_net->getNodes().size() < 40000) {
       dynamic_cast<Incrementable*>(my_net)->incrementTime();
       }*/
     //The exponent -2.236 is what we fit to DPA network of the same size
     PowerLawDRV pl(r1, -2.3,2,1000);
     DegreeLawNetFac nf(40000, pl, r1, true);
     my_net = nf.create();
		    
     //my_net = new DegreeLawRandomNetwork(nodes, *pl, r1);
     //my_net = new GnutellaNetwork(argv[1],"limecrawler");
     //my_net = new GnutellaNetwork(argv[1],"ripeanu");

     Network *net = 0;
     int choice = 2;
     if(choice == 1){
       ComponentPart cp;
       set<Network*>* net_set = cp.partition(*my_net);
       cout << "The number of components of this network is: " << net_set->size() << endl;
       set<Network*>::const_iterator comp_it;
       cout << "The components have the following sizes: ";
       for(comp_it = net_set->begin(); comp_it != net_set->end(); comp_it++){
	 net = *(comp_it);
	 cout << net->getNodeSize() << " ";
       }
       //Free the memory here:
       cp.deletePartition(net_set);
       net = 0;
     }
     else{
       //The exponent -1.81 is what we want for the email network
       int nodes = 10000;
       PowerLawDRV pl(r1,-2.1,2,(int)sqrt(nodes));
       DegreeLawNetFac my_fact(nodes, pl, r1, true);
       net = my_fact.create();
     }
     cout << endl;
     //use the biggest connected component;
     NodeIntStats ns;
     ns.collect(net, &Network::getDegree);
     cout << "#nodes: " << net->getNodeSize() << endl 
          << "#edges: " << net->getEdgeSize() << endl
	  << "#<k>: " << ns.getAverage() << endl
	  << "#<k^2>: " << ns.getMoment2() << endl;
     cout << "#ttl p hit_rate edges_crossed nodes_reached" << endl;
     //Put the nodes into a vector so we can randomly select them easier:
     node_vector.clear();
     auto_ptr<NodeIterator> ni( net->getNodeIterator() );
     while( ni->moveNext() ) {
       node_vector.push_back( ni->current() );
     }
     
     Message* query = 0;
     Message* implant = 0;
     
     int hits;
     double last_hr = 0.0;
     double hit_rate = 0.0;
     double av_edges = 0.0;
     double av_nodes = 0.0;
     double p, delta_p = 0.01;
     int ttl=15;
     //for(ttl = 2; ttl < 12; ttl++)
     {
       p = 0.01;
       //p = 1.0;
       while(p < 0.51)
       {
	 delete query;
	 query = new WalkAndPercMessage(r2,p,ttl,ttl);
	 //query = new MagnetMessage(r2,p,ttl);
	 //query = new BroadcastMessage(ttl);
	 
	 delete implant;
	 implant = new AnycastMessage(r4,1,ttl);
	 //implant = new MagnetMessage(r2,0.75, 2 * ttl);
	 //implant = new WalkAndPercMessage(r4,p,ttl);
	 //implant = new AnycastMessage(r4,1,0);
	 
	 delete my_cnet;
	 my_cnet = new ContentNetwork(*net);
	 // Insert the content into the network
	 for(int i = 0; i < cont_count; i++) 
	 {
	     a_node = node_vector[ r3.getInt( node_vector.size() - 1 ) ];
	     c_node = content_vector[i];
	     auto_ptr<Network> implantnet( implant->visit( a_node, *net) );
	     auto_ptr<NodeIterator> nit( implantnet->getNodeIterator() );
	     my_cnet->insertContent(c_node, nit.get() );
         }
         //Do a random search for each node and see how many are successful.
         Network::NodePSet res_nodes;
         hits = 0;
      
	 int hit_nodes = 0, crossed_edges = 0;
         for(int i = 0; i < runs; i++) {
	     a_node = node_vector[ r3.getInt( node_vector.size() - 1 ) ];
	     c_node = content_vector[ i % cont_count ];
	     auto_ptr<Network> visited( query->visit(a_node, *net) );
	     auto_ptr<NodeIterator> nit( visited->getNodeIterator() );
	     auto_ptr<Network> res_nodes(
			     my_cnet->queryForContent(c_node, nit.get() ) );
	     if( res_nodes->getNodeSize() != 0) {
	       ++hits;
	     }
	     hit_nodes += visited->getNodeSize();
	     crossed_edges += visited->getEdgeSize();
         }
	 hit_rate = (double)hits/(double)runs;
	 av_edges = (double)crossed_edges/(double)runs;
	 av_nodes = (double)hit_nodes/(double)runs;
	 cout << ttl << " " << p
		     << " " << hit_rate
		     << " " << av_edges
		     << " " << av_nodes
		     << endl;
	 /* Do an adapative step:
	 delta_p = 0.00005/abs(hit_rate - last_hr);
	 if( delta_p < 0.001 ) {
           delta_p = 0.001;
	 }
	 if( delta_p > 0.1 ) {
           delta_p = 0.1;
	 }*/
	 p += delta_p;
       }
     }
     //Delete all the content
     my_cnet->deleteContent();
     delete my_cnet;
     delete my_net;
     delete net;
	
  return 1;
	
}
