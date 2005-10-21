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

#include "netmodeler.h"

#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

#if 0
	if( argc < 2) {
           cout << "usage: " << argv[0] << " packagefile" << endl;
	   return 0;
	}
	DebianDependencyNetwork my_net(argv[1]);
	cout << "#read debian package: " << argv[1] << endl;
/*
	map<int, int> in_deg_dist = my_net.getOutDegreeDist();
	
        map<int, int>::iterator deg_it = in_deg_dist.begin();
	cout << "#printing out degree distribution" << endl;
	while( deg_it != in_deg_dist.end() ) {
          cout << deg_it->first << " " << deg_it->second << endl;
	  deg_it++;
	}
	//cout << "#cluster coeff: " << my_net.getClusterCoefficient() << endl;
*/

	Network deb_undir;
	cerr << "#about to move into an undirected network" << endl;
	my_net.moveIntoUndirectedNetwork(deb_undir);

	cerr << "#converted to undirected" << endl;
       
	set<Node*>::iterator n = nodes.begin();
	for(; n != nodes.end(); n++) {
          cout << deb_undir.getDegree( *n ) << " " 
		  << deb_undir.getClusterCoefficient( *n ) << endl;
	}
/*
        map<int, int> deg_dist = deb_undir.getDegreeDist();
        map<int, int>::iterator deg_it = deg_dist.begin();
	cout << "#printing out degree distribution" << endl;
	for(; deg_it != deg_dist.end(); deg_it++ ) {
          cout << deg_it->first << " " << deg_it->second << endl;
	}
        cout << "#cluster coeff: " << deb_undir.getClusterCoefficient() << endl;
	cout << "#edges: " << deb_undir.getEdges().size() << endl;
	cout << "#nodes: " << deb_undir.getNodes().size() << endl;
*/
	//Delete the allocated memory (nodes)
	deb_undir.clear();
#endif

#if 0
	set<Node*> all_nodes;
	Network my_net;
	//Make all the nodes:
	for(int i = 0; i < 100; i++) {
          all_nodes.insert( new Node() );
	}
	
	//Make a network:
        set<Node*>::iterator n_it = all_nodes.begin();
	Node* last_node = 0;
	while(n_it != all_nodes.end()) {
          if(last_node) {
            my_net.add( Edge(last_node, *n_it) );
	  }
          last_node = *n_it;
          n_it++;
	}
#endif

#if 0
    /**
     * Test transitivity calculation:
     */
    Network* my_net;
    Ran1Random my_rand;
    double p = 0.1;
    int n = 100;
    while(p < 1.0) {
      my_net = new RandomNetwork(n, p, my_rand);
      cout << p << " " << my_net->getTransitivity() << endl;
      p += 0.1;
    }
#endif
	
#if 0
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
	
#endif
	

#if 0

   //Test the edge betweenness calculation for some small graphs
   //Using the examples from page 5 of cond-mat/0308217
   vector<Node*> nodes;
   //This is the first example:
   Network net;
   stringstream node_name;
   for(int i = 0; i < 7; i++) {
     node_name.str("");
     node_name << i;
     nodes.push_back(new Node( node_name.str() ));
     net.add( nodes.back() );
   }
   //Add the edges:
   net.add( Edge(nodes[0], nodes[1]) );
   net.add( Edge(nodes[0], nodes[2]) );
   net.add( Edge(nodes[1], nodes[3]) );
   net.add( Edge(nodes[2], nodes[4]) );
   net.add( Edge(nodes[2], nodes[5]) );
   net.add( Edge(nodes[5], nodes[6]) );

   map<Edge*, double> betweenness;
   Edge* max_edge = net.getEdgeBetweennessFor(nodes[0], betweenness);
   cout << "Max Edge: " << max_edge->toString() << endl;
   map<Edge*, double>::iterator betit;
   for(betit = betweenness.begin(); betit != betweenness.end(); betit++) {
     cout << betit->first->toString() << " bet: " << betit->second << endl;
   }

   
   //This is the second example:
   net.clear();
   nodes.clear();
   for(int i = 0; i < 7; i++) {
     node_name.str("");
     node_name << i;
     nodes.push_back( new Node(node_name.str()) );
     net.add( nodes.back() );
   }
   //Add the edges:
   net.add( Edge(nodes[0], nodes[1]) );
   net.add( Edge(nodes[0], nodes[2]) );
   net.add( Edge(nodes[1], nodes[3]) );
   net.add( Edge(nodes[2], nodes[3]) );
   net.add( Edge(nodes[2], nodes[4]) );
   net.add( Edge(nodes[3], nodes[5]) );
   net.add( Edge(nodes[4], nodes[5]) );
   net.add( Edge(nodes[4], nodes[6]) );
   betweenness.clear();
   max_edge = net.getEdgeBetweennessFor(nodes[0], betweenness);
   cout << "Max Edge: " << max_edge->toString() << endl;
   for(betit = betweenness.begin(); betit != betweenness.end(); betit++) {
     cout << betit->first->toString() << " bet: " << betit->second << endl;
   }

   //Try a circle:
   net.clear();
   nodes.clear();
   for(int i = 0; i < 10; i++) {
     node_name.str("");
     node_name << i;
     nodes.push_back( new Node(node_name.str()) );
     net.add( nodes.back() );
   }
   //connect each node to the next:
   for(int i = 0; i < nodes.size(); i++) {
     net.add( Edge( nodes[i], nodes[ (i + 1) % nodes.size() ] ) );
   }
   betweenness.clear();
   max_edge = net.getEdgeBetweennessFor(nodes[0], betweenness);
   cout << "Max Edge: " << max_edge->toString() << endl;
   for(betit = betweenness.begin(); betit != betweenness.end(); betit++) {
     cout << betit->first->toString() << " bet: " << betit->second << endl;
   }
   betweenness.clear();
   max_edge = net.getEdgeBetweenness(betweenness);
   cout << "Max Edge: " << max_edge->toString() << endl;
   for(betit = betweenness.begin(); betit != betweenness.end(); betit++) {
     cout << betit->first->toString() << " bet: " << betit->second << endl;
   }
   //Here is a line:
   net.clear();
   nodes.clear();
   for(int i = 0; i < 10; i++) {
     node_name.str("");
     node_name << i;
     nodes.push_back( new Node(node_name.str()) );
     net.add( nodes.back() );
   }
   //connect each node to the next:
   for(int i = 0; i < nodes.size() - 1; i++) {
     net.add( Edge( nodes[i], nodes[ i + 1] ) );
   }
   betweenness.clear();
   max_edge = net.getEdgeBetweennessFor(nodes[0], betweenness);
   cout << "Max Edge: " << max_edge->toString() << endl;
   for(betit = betweenness.begin(); betit != betweenness.end(); betit++) {
     cout << betit->first->toString() << " bet: " << betit->second << endl;
   }
   betweenness.clear();
   max_edge = net.getEdgeBetweenness(betweenness);
   cout << "Max Edge: " << max_edge->toString() << endl;
   for(betit = betweenness.begin(); betit != betweenness.end(); betit++) {
     cout << betit->first->toString() << " bet: " << betit->second << endl;
   }
   //A star network:
   net.clear();
   nodes.clear();
   for(int i = 0; i < 20; i++) {
     node_name.str("");
     node_name << i;
     nodes.push_back( new Node(node_name.str()) );
     net.add( nodes.back() );
   }
   //connect each node to the center:
   for(int i = 1; i < nodes.size(); i++) {
     net.add( Edge( nodes[0], nodes[ i ] ) );
   }
   betweenness.clear();
   max_edge = net.getEdgeBetweennessFor(nodes[1], betweenness);
   cout << "Max Edge: " << max_edge->toString() << endl;
   for(betit = betweenness.begin(); betit != betweenness.end(); betit++) {
     cout << betit->first->toString() << " bet: " << betit->second << endl;
   }
   betweenness.clear();
   max_edge = net.getEdgeBetweenness(betweenness);
   cout << "Max Edge: " << max_edge->toString() << endl;
   for(betit = betweenness.begin(); betit != betweenness.end(); betit++) {
     cout << betit->first->toString() << " bet: " << betit->second << endl;
   }

#endif
	
#if 1 
	//Make a network:
	int i_nodes;
	if( argc > 1) {
	  i_nodes = atoi(argv[1]);
	}
	else {
            i_nodes = 10;
	}
	Ran1Random my_rand, my_rand2(-100), my_rand3(-1000);
        //RandomNetwork my_net(i_nodes, 0.005, my_rand);
	//RandomDirectedNetwork my_net(1000, 0.005, my_rand);
	//PolyPreference my_p;
	//ExpPreference my_p(1.0);
        //DoublePrefAtNetwork my_net(RandomNetwork(10,0.5,my_rand), my_rand, 2);
	//PrefAtNetwork my_net(RandomNetwork(3,1,my_rand),my_rand,1);
	//my_net.incrementTime(i_nodes-3);
	/*
	PrefDelCompNetwork my_net(RandomNetwork(i_nodes, 1.0, my_rand),
			          my_rand,
				  0.7,
				  1);
	//make sure we have 20,000 nodes;
	while( my_net.getNodes().size() < 2000) {
	  //cerr << "edges: " << my_net.getEdges().size() << endl;
	  my_net.incrementTime(1);
	}
	*/
	//ifstream content_crawl(argv[2]);
	//Network my_net(cin);
	//UniformAttachNet my_net(RandomNetwork(7, 0.5, my_rand), my_rand, 2);
	//my_net.incrementTime(i_nodes - 7);
	//GnutellaFileSpaceNetwork my_net( std::cin, 100000);
	//CallawayNetwork my_net(1000, 0.2, my_rand);
	//SmallWorldNetwork my_net(30, 0.1, my_rand);
	//NoisyStarDNet my_net(0.0,my_rand, 0.0, 1.0);
	//my_net.incrementTime(50000);
	//SimkinNetwork my_net(2000,10,0.75,my_rand);
	//VazquezDirectedNetwork my_net(200,0.3,my_rand);
	//PowerLawProbabilityFunction pl(-2.0,2,sqrt(i_nodes));
	//ExpProbabilityFunction expl(2.0/3.0,2, i_nodes);
	//DegreeLawRandomNetwork my_net(i_nodes,expl,my_rand,true); 
	//SpatialNetwork my_net(i_nodes,my_rand,3,2);
	//PercolationMessage my_mes(my_rand,0.9,2);
	//AnycastMessage my_mes(my_rand, 1, 5);
	//BroadcastMessage my_mes(2);
	AmnesiacMessage my_mes(my_rand,5);
	//MagnetMessage my_mes(my_rand,0.0,5);
	//MessageBuiltNetwork my_net(5000, my_mes, my_rand2,3);
	//DoubleMBNet my_net(RandomNetwork(3, 1.0, my_rand), my_mes, my_rand2, 2);
        DoublePrefAtNetwork my_net(RandomNetwork(3,1.0,my_rand), my_rand2, 2);
	my_net.incrementTime(5000 - 3);
        //DebianDependencyNetwork deb_net(argv[1]);
	//DebianDependencyNetwork my_net(argv[1]);
	//Network my_net;
	//cout << "#read debian package: " << argv[1] << endl;
        //my_net.reverseEdges();
	//cout << "#reversed edges" << endl;
	
	//deb_net.moveIntoUndirectedNetwork( my_net );
	//cout << "#converted to undirected networks" << endl;
	
	//GnutellaNetwork my_net(argv[1],"ripeanu");
	//GnutellaNetwork my_net(argv[1],"limecrawler");
  //vector<double> netp_v;
  //netp_v.push_back(tp);

//ER graph mixing  
#if 0
  int increments = 1;
  int net_num = increments;
  int netn= (int)pow(2.0,9.0); 
  double lp = 1/netn;
  double hp = 50/netn;
  double incrp = (hp-lp)/((double)increments);
  double tp = lp;
  while (net_num-- > 0)
  {
    double ave_d = tp*netn;
    RandomNetwork ert(netn,.5,my_rand);
    //RandomNetwork ert(netn,tp,my_rand);
    cout<<ert.getNodes().size()<<endl;
    set<Network> er_components = ert.getComponents();
    cout <<"Components: "<< er_components.size()<<endl;
    set<Network>::iterator comp_it_big = er_components.begin();
    set<Network>::iterator comp_it_biggest;
    int biggest_size = 0;
    int tmp_size = 0;
    for ( comp_it_big = er_components.begin();
        comp_it_big != er_components.end();
        comp_it_big++)
    {
      tmp_size = comp_it_big->getNodes().size();
      if ( biggest_size < tmp_size )
      {
      cout<<"csize "<<tmp_size<<" "<<biggest_size<<endl;
        biggest_size = tmp_size; 
        comp_it_biggest = comp_it_big;
      }
    }
    cout <<  comp_it_biggest->getNodes().size()<<endl;
    
    vector<Node*> node_v;
    set<Node*>::iterator node_it;
    for (node_it = comp_it_biggest->getNodes().begin(); 
      node_it != comp_it_biggest->getNodes().end(); 
      node_it++ )
    {
      node_v.push_back( *node_it);
    }
    GraphMatrix g_matrix(*comp_it_biggest);
    stringstream num_string;
    num_string <<"er_trans_matrix_"<<ave_d<<".dat";
    g_matrix.transitionProbabilityMatrixToAsciiFiles(
      num_string.str(),node_v);

    tp += incrp;
  }
#endif  
  
  // write out various matrices associated with the graph
#if 0
  vector<Node*> node_v;
  set<Node*>::iterator node_it;
  for (node_it = my_net.getNodes().begin(); 
      node_it != my_net.getNodes().end(); 
      node_it++ )
  {
    node_v.push_back( *node_it);
  }

  GraphMatrix g_matrix(my_net);
  g_matrix.unitSquareIncidenceMatrixToAsciiFile(
      string("new_incid.txt"),node_v);
  g_matrix.transitionProbabilityMatrixToAsciiFiles(
      string("new_trans_matrix.txt"),node_v);
#endif

#if 1
        IntStats ns(true);
	auto_ptr<NodeIterator> ni( my_net.getNodeIterator() );
	ns.collect((Network*)&my_net,&Network::getDegree, ni.get());
	ns.collectByEdge(&my_net, &Network::getDegree);
        cout << "#loaded network" << endl;	
	cout << "#assortativity: " << ns.getEdgeCorrelation() << endl
	     << "#cluster coeff: " << my_net.getClusterCoefficient() << endl
	     << "#transitivity: " << my_net.getTransitivity() << endl
	     << "#nodes: " << my_net.getNodeSize() << endl
	     << "#edges: " << my_net.getEdgeSize() << endl
	     << "#<k>: " << ns.getAverage() << endl
	     << "#<k^2>: " << ns.getMoment(2) << endl
	     << "#H(degs): " << ns.getEntropy() << endl
	     << "#EdgeMI: " << ns.getEdgeMutualInfo() << endl;
#endif
  //Some stats for GnutellaFileSpaceNetworks:
#if 0
	cout << "#queries: " << my_net.getNodes(
			   GnutellaFileSpaceNetwork::query_type).size() << endl
	     << "#contents: " << my_net.getNodes(
			   GnutellaFileSpaceNetwork::content_type).size() << endl
	     << "#peers: " << my_net.getNodes(
			   GnutellaFileSpaceNetwork::node_type).size() << endl;
#endif
	//These are valid for debian networks:
	//cout << "#size assortativity: " << my_net.getSizeAssortativity() << endl
	//     << "#lib assortativity: " << my_net.getLibAssortativity() << endl;

#if 0
        //Look at the number of n-th neighbors:
	map<int, int> nth_n(my_net.getNthNeighborDist(2));
	map<int, int>::const_iterator nth_it;
	ofstream nth_nf("last.nth_n");
	for(nth_it = nth_n.begin(); nth_it != nth_n.end(); nth_it++) {
          nth_nf << nth_it->first << " " << nth_it->second << endl;
	}
#endif
	
#if 0
	//Look at degree-cluser coeff. correlations
	ofstream deg_cc("last.deg_cc");
	map<int, double> deg_cc_m = my_net.getCCvsDegree();
	map<int, double>::const_iterator dcc_it;
	for(dcc_it = deg_cc_m.begin(); dcc_it != deg_cc_m.end(); dcc_it++) {
          deg_cc << dcc_it->first << " " 
		  << dcc_it->second << endl;
	}
#endif

#if 0

	//Look at all cc, degree pairs:
	ofstream cc_deg_all("last.cc_deg_pairs");
	Network::NodePSet cda_nodes = my_net.getNodes();
	Network::NodePSet::iterator n_ita;
	for(n_ita = cda_nodes.begin(); n_ita != cda_nodes.end(); n_ita++) {
            cc_deg_all 
		       << my_net.getDegree( *n_ita )
		       << " " 
		       << my_net.getClusterCoefficient( *n_ita )
		       << endl;
	}
#endif
	
#if 0
		//Look at a histogram of the cc:
	vector<int> hist = my_net.getCCHist(100);
        ofstream cc_hist("last.cc_hist");
	for(unsigned int hi = 0; hi < hist.size(); hi++) {
            cc_hist << hi << " " << hist[hi] << endl;
	}
#endif

#if 0
     //This is the email graph cutter:
     //Do the recursive cut on all the components until each
     //have cc = 0, or cc > 0.4
     set<Network> components = my_net.getComponents();
     int size = my_net.getNodes().size();
     set<Network>::iterator comp_it;
     ofstream spam_graphs("spam.graph");
     ofstream ham_graphs("ham.graph");
     Edge* cut_edge = 0;
     while( components.size() > 0 ) {
       //Choose the biggest graph
       comp_it = components.begin();
       //Only pay attention to big graphs
       if( comp_it->getNodes().size() >= size/100 ) {
       if( comp_it->getClusterCoefficient() == 0.0 ) {
	 //cout << "Printing spam graph" << endl;
         //comp_it->printTo(spam_graphs);
       }
       else if( comp_it->getClusterCoefficient() > 0.4 ) {
	 //cout << "Printing ham graph" << endl;
	 vector< pair<int, int> > joins;
	 vector<double> q_t;
	 int step = comp_it->getCommunities(q_t,joins);
	 set<Network> coms = comp_it->getCommunity(step, joins);
	 set<Network>::iterator comit;
	 int community = 0;
	 for(comit = coms.begin(); comit != coms.end(); comit++) {
           stringstream comname;
	   comname << "community_" << community++ << ".graph";
           ofstream comstream( comname.str().c_str() );
	   comit->printTo( comstream );
	 }
         //comp_it->printTo(ham_graphs);
       }
       else {
         //This is a graph that needs to be split
	 cout << "Splitting Graph of size: "
		 << comp_it->getNodes().size() << endl;
	 map<Edge*, double> bet;
	 set<Network> split;
	 cut_edge = comp_it->getEdgeBetweenness(bet);
	 //Remove both nodes at either ends of this edge
	 Network tmp = *comp_it;
	 tmp.remove( cut_edge->first );
	 tmp.remove( cut_edge->second );
	 split = tmp.getComponents();
	 components.insert( split.begin(), split.end() );
       }
       }
       else {
         //cout << "Skipping graph" << endl;
       }
       //cout << "graphs: " << components.size() << endl;
       components.erase( comp_it );
     }
#endif
	
#if 0	
	//Look on components:
	set<Network> components = my_net.getComponents();
	set<Network>::const_reverse_iterator comp_it = components.rbegin();
	stringstream outname;
	cout << "#components: " << components.size() << endl;
	for(int c_count = 0;comp_it != components.rend(); comp_it++) {
	  double exp_cc = comp_it->getExpectedTransitivity();
	  double cc = comp_it->getTransitivity();
	  int wedges = comp_it->getWedgeCount();
	  Network::NodePSet::const_iterator n_it;
	  int kmax = 0;
	  for(n_it = comp_it->getNodes().begin();
	      n_it != comp_it->getNodes().end();
	      n_it++) {
            if( kmax < comp_it->getDegree( *n_it ) ) {
              kmax = comp_it->getDegree( *n_it );
	    }
	  }
          cout << "#comp(" << ++c_count
	       << ") size=" << comp_it->getNodes().size()
	       << " kmax=" << kmax
	       << " kmax+1/size="
	          << (double)(kmax+1)/(double)comp_it->getNodes().size()
	       << " CC=" << comp_it->getClusterCoefficient()
	       //<< " CCV=" << comp_it->getCCStdErr()
	       << " exp_c=" << exp_cc
	       << " c=" << cc
	       << " ratio=" << (cc/exp_cc)
	       //<< " variance=" << sqrt( exp_cc * (1.0-exp_cc)/(double)wedges )
	       << endl;
	  /*
	       << "<k^2> = " << k2 << " "
	       << "<k> = " << comp_it->getDegreeMoment(1) << " "
	       << "cc: " << comp_it->getClusterCoefficient() << " "
	       << "a: " << comp_it->getAssortativity() << " ";
	       */
	  /*
	  //Print the mean distance:
	  map<int, int> ddist = comp_it->getDegreeDist();
	  map<int, int>::reverse_iterator ddit;
	  double dave = 0.0;
	  ddit = ddist.rbegin();
	  cout << "d = " << ddist.rbegin()->first << endl;
	  */
	  outname.str("");
	  outname << "comp." << c_count;
	  ofstream comp_print( outname.str().c_str() );
          comp_it->printTo( comp_print );
#if 0
	//if( (comp_it->getNodes().size() * 100) > my_net.getNodes().size() ) {
	if( comp_it->getClusterCoefficient() > 0.0 ) {
        //Print out the communities:
	vector< pair<int, int> > joins;
	vector<double> q_t;
	int step = comp_it->getCommunities(q_t, joins);
	set< Network > comms = comp_it->getCommunity(step, joins);
	set< Network >::const_reverse_iterator comit;
	Network::NodePSet::const_iterator comnodeit;
	int community = 0;
	cout << "#Q: " << q_t[step] << endl;
	cout << "#step: " << step << endl;
	for(comit = comms.rbegin();
	    comit != comms.rend();
	    comit++) {
          //if( comit->getClusterCoefficient() < 0.05 ) { continue; }
          cout << "#Community: " << community++ << endl
	       << "#size: " << comit->getNodes().size() << endl
	       << "#cc: " << comit->getClusterCoefficient() << endl;
	  for(comnodeit = comit->getNodes().begin();
	      comnodeit != comit->getNodes().end();
	      comnodeit++) {
            cout << (*comnodeit)->toString() << endl;
	  }
	  cout << endl;
	  //Get sub communities:
	  vector< pair<int,int> > sjoins;
	  vector<double> sq;
	  int sub_steps = comit->getCommunities(sq, sjoins);
	  set<Network> subcom = comit->getCommunity(sub_steps, sjoins);
	  set< Network >::const_reverse_iterator comit2;
	  if( sq[sub_steps] > 0.3 ) {
            cout << "#subcommunity: " << endl
		 << "#Q: " << sq[sub_steps] << endl;
            //There is a subcommunity structure:
	    for(comit2 = comms.rbegin();
	        comit2 != comms.rend();
	        comit2++) {
	     	  for(comnodeit = comit2->getNodes().begin();
	              comnodeit != comit2->getNodes().end();
	              comnodeit++) {
                      cout << (*comnodeit)->toString() << endl;
	          }
                  cout << endl;
	    }
	  }
	}
	}
#endif
	  
	}
#endif

	
#if 1
//Print the network:
	ofstream dot_out("last.nm");
	my_net.printTo(dot_out);
	
#endif
	
#if 0 
	//Print out the degree distribution:
	ofstream deg_out("last.degs");
	cout << "about to getNodes" << endl;
	map<int, int> deg_dist = my_net.getDegreeDist(
			my_net.getNodes(
				//GnutellaFileSpaceNetwork::node_type
				)
				                     );
        map<int, int>::iterator deg_it = deg_dist.begin();
	deg_out << "#printing out degree distribution" << endl;
	for(;deg_it != deg_dist.end(); deg_it++ ) {
          deg_out << deg_it->first << " " << deg_it->second << endl;
	}
#endif

#if 0
        //Look at wieght distributions for GnutellaFileSpace
	ofstream w_out("last_content_weights.dat");
	w_out << "#content weight histogram" << endl;
	map<double,int> weights = my_net.getContentWeights();
	map<double,int>::iterator wit;
	w_out << "#using an exponentially increasing window size" << endl;
	for(wit = weights.begin(); wit != weights.end(); wit++) {
          w_out << wit->first << " " << wit->second << endl;
	}
#endif
	
#if 0 
	//Print out the in degree distribution:
	ofstream indeg_out("last.indegs");
	map<int, int> indeg_dist = my_net.getInDegreeDist();
        map<int, int>::iterator indeg_it = indeg_dist.begin();
	deg_out << "#printing out in-degree distribution" << endl;
	for(;indeg_it != indeg_dist.end(); indeg_it++ ) {
          indeg_out << indeg_it->first << " " << indeg_it->second << endl;
	}
#endif

#if 0
	//Print out the distance distribution:
	ofstream dist_out("last_distance.dist");
	map<int, int> dist_dist = my_net.getDistanceDist();
        map<int, int>::iterator dist_it = dist_dist.begin();
	dist_out << "#printing out distance distribution" << endl;
	for(;dist_it != dist_dist.end(); dist_it++ ) {
          dist_out << dist_it->first << " " << dist_it->second << endl;
	}
#endif
	
#if 0
	//Print out the associated number distribution:
	ofstream an_out("last_an.dist");
	map<int, int> an_dist = my_net.getAssociatedNumberDist();
        map<int, int>::iterator an_it = an_dist.begin();
	dist_out << "#printing out associated number distribution" << endl;
	for(;an_it != an_dist.end(); an_it++ ) {
          an_out << an_it->first << " " << an_it->second << endl;
	}
#endif
	
#if 1
	//Look at the CDF:
	//Print out the degree distribution:
	ofstream cdf_out("last.cdf");
	/*
	map<int, int> cdf_dist = my_net.getDegreeDist(
			my_net.getNodes(GnutellaFileSpaceNetwork::node_type)
				                     );
						     */
	const map<int, int>& cdf_dist = ns.getDist();
        map<int, int>::const_iterator cdf_it;
	cdf_out << "#printing out degree distribution" << endl;
	int degs = 0;
	for( cdf_it = cdf_dist.begin(); cdf_it != cdf_dist.end(); cdf_it++) {
          degs += cdf_it->second;
	}
	int sum = 0;
	for(cdf_it = cdf_dist.begin(); cdf_it != cdf_dist.end(); cdf_it++ ) {
	  //plot the number of nodes which have degree >= cdf_it->first
          cdf_out << cdf_it->first << " " << degs - sum << endl;
	  sum += cdf_it->second;
	}
#endif
#if 0
//Look at percolation on a network:
	Message *my_message = 0; 
	double p = 1e-5, q = 0.01;
        my_rand3.setBoolTrueBias(q);
	cout << "#each node gets bug with probability: " << q << endl;
	set<Node*> nodes = my_net.getNodes();
        set<Node*>::iterator n;
        set<Node*> vnodes;
      
	p = 0.001;
	//int ttl = -1;
	//while( ttl < 20) {
	while( p < 1.0) {
	  my_rand3.setBoolTrueBias(q);
	  delete my_message;
	  my_message = new PercolationMessage(my_rand2,p);
	  //my_message = new AnycastMessage(my_rand2,max,ttl);
	n = nodes.begin();
	for(; n != nodes.end(); n++) {
	  if( my_rand3.getBool() ) {
            my_message->visit( *n, my_net );
	  }
	/*
          cout << my_net.getDegree( *n ) << " " 
		  << my_net.getClusterCoefficient( *n ) << endl;
	*/
	}
	cout
	     << p
	     //<< ttl
	     << " " << my_message->getVisitedNodes().size() << endl;
        
	  p *= 1.05;
	  //++ttl;
	}
          
	delete my_message;
#endif
	//Delete all the nodes
	cout << "clearing the network" << endl;
	my_net.clear();
	cout << "cleared network" << endl;
	
#endif 

#if 0
    //Here is a debian analyzer.
    if( argc < 2) {
        cerr << "usage: " << argv[0] << " packagefile" << endl;
        return 0;
    }
    
    DebianDependencyNetwork my_net(argv[1]);
    cout << "read debian package: " << argv[1] << endl;
  
    string out_name = argv[1];
    out_name += ".out.dat";
    string in_name = argv[1];
    in_name += ".in.dat";
    
    ofstream out_file( out_name.c_str() );
    ofstream in_file( in_name.c_str() );

    out_file << "#debian package file: " << argv[1] << endl;
    in_file << "#debian package file: " << argv[1] << endl;
    
    //Look at the in degree:
    in_file << "#in degree distribution:" << endl;
    map<int, int> deg_dist = my_net.getInDegreeDist();
    map<int, int>::const_iterator deg_it = deg_dist.begin();
    for(; deg_it != deg_dist.end(); deg_it++) {
        in_file << deg_it->first << " " << deg_it->second << endl;
    }
    int nodes = my_net.getNodes().size();
    in_file << "#nodes: " << nodes << endl;
    //In degree:
    deg_it = deg_dist.begin();
    int in_edges = 0, in_edges2 = 0;
    for(;deg_it != deg_dist.end(); deg_it++) {
        in_edges += (deg_it->first) * (deg_it->second);
        in_edges2 += (deg_it->first) * (deg_it->second) * (deg_it->second);	
    }
    in_file << "#<deg> = " << (double)(in_edges)/(double)nodes << endl;
    in_file << "#<deg^2> = " << (double)(in_edges2)/(double)nodes << endl;
    
    //Look at the out degree:
    out_file << "#out degree distribution:" << endl;
    deg_dist = my_net.getOutDegreeDist();
    deg_it = deg_dist.begin();
    for(; deg_it != deg_dist.end(); deg_it++) {
        out_file << deg_it->first << " " << deg_it->second << endl;
    }
    deg_it = deg_dist.begin();
    in_edges = 0; in_edges2 = 0;
    for(;deg_it != deg_dist.end(); deg_it++) {
        in_edges += (deg_it->first) * (deg_it->second);
        in_edges2 += (deg_it->first) * (deg_it->second) * (deg_it->second);	
    }
    
    out_file << "#nodes: " << nodes << endl;
    out_file << "#<deg> = " << (double)(in_edges)/(double)nodes << endl;
    out_file << "#<deg^2> = " << (double)(in_edges2)/(double)nodes << endl;
    
    cout << "#assortativity: " << my_net.getAssortativity() << endl;
    cout << "#lib assortativity: " << my_net.getLibAssortativity() << endl;
    cout << "#edges: " << my_net.getEdges().size() << endl;
    
    //Network undir_net;
    //my_net.moveIntoUndirectedNetwork( undir_net );

    //double cc = undir_net.getClusterCoefficient();
    //cout << "#clustering coeff: " << cc << endl;
    
    //undir_net.clear();
#endif

#if 0 
     //Here we consider a network for a P2P topology
     Ran1Random r1(-1), r2(-1000), r3(-1000000), r4(-5678);
     int nodes;
     int node_max = 256*1024;
     //PrefAtNetwork my_net(nodes, r1, 2, 2);
     //GnutellaNetwork my_net(argv[1],"limecrawler");
     PowerLawProbabilityFunction* pl = 0;
     //DynamicNetwork* my_net = 0;
     Network* my_net = 0;
     ContentNetwork* my_cnet = 0;
     Node* a_node = 0;
     ContentNode* c_node = 0;
     vector<ContentNode*> content_vector;
     vector<Node*> node_vector;
     int cont_count = 100;
     
     content_vector.reserve(cont_count);
     for(int i = 0; i < cont_count; i++) {
         content_vector.push_back(new ContentNode);
     }
     
     Message* query = 0;
     Message* implant = 0;
     double p, p_max, p_min;
     double hit_rate = 0.0;
     double target_rate = 0.25;
     double eps = 0.025;
     int hits, runs;
     int ttl;
     
     cout << "#check percolation search on size n:" << endl;
     p = 0.0;
     nodes = 1<<14;
     //for(nodes = 1024; nodes < 256*1024; nodes*=2) {
       //my_net = new PrefAtNetwork(nodes,r1,2,2);
       //my_net = new DoublePrefAtNetwork(nodes,r1,2,2);
       //my_net = new PrefDelCompNetwork(RandomNetwork(nodes,0.001,r1), r1);
       //my_net->incrementTime(4*nodes);
       pl = new PowerLawProbabilityFunction(-2.0,1,nodes);
       my_net = new DegreeLawRandomNetwork(nodes, *pl, r1);
       
       //Put the nodes into a vector so we can randomly select them easier:
       node_vector.clear();
       node_vector.insert(node_vector.begin(), my_net->getNodes().begin(), my_net->getNodes().end() );
       r3.setIntRange( node_vector.size() - 1);
       ttl =  ((int)(log(nodes)/log(2)) + 1);
       hit_rate = 0.0;
       //We keep the previous p each time as our first try.
       p_max = 1.0;
       p_min = 0.0;
       //p = 5.0 * log(nodes)/((double)(nodes));
       while( p < 1.0) {
	 //Do a binary search for |hit_rate - target| < eps
	 delete query;
	 //query = new WalkAndPercMessage(r2,p,ttl);
	 query = new MagnetMessage(r2,p,ttl);
	 delete implant;
	 //implant = new AnycastMessage(r4,1,ttl);
	 implant = new MagnetMessage(r2,p,ttl);
	 //implant = new WalkAndPercMessage(r4,p,ttl);
	 delete my_cnet;
	 my_cnet = new ContentNetwork(*my_net);
	 // Insert the content into the network
	 for(int i = 0; i < cont_count; i++) 
	 {
	     a_node = node_vector[ r3.getInt() ];
	     c_node = content_vector[i];
	     my_cnet->insertContent(a_node, c_node, *implant);
         }
         //Do a random search for each node and see how many are successful.
         Network::NodePSet res_nodes;
         hits = 0;
         runs = 10;
     
         r1.setIntRange( content_vector.size() - 1);
         for(int i = 0; i < runs; i++) {
	     a_node = node_vector[ r3.getInt() ];
	     c_node = content_vector[ r1.getInt() ];
	     res_nodes = my_cnet->queryForContent(a_node, c_node, *query);
	     if( res_nodes.size() != 0) {
		     ++hits;
		     /*
		     Network::NodePSet::iterator rn_it;
		     for(rn_it = res_nodes.begin(); rn_it != res_nodes.end(); rn_it++) {
                       cout << " address: " << *rn_it << endl;
		     }
		     cout << " Hit: " << hits << " Found content: " << c_node << endl;
		     cout << "hits: " << res_nodes.size() << endl;
		     */
	     }
         }
	 hit_rate = (double)hits/(double)runs;
	 cout << p << " " << hit_rate << endl;
	 p+=0.1;
       }
       //Delete all the content
       my_net->clear();
       cout << nodes << " " << p << " " << hit_rate << endl;
       delete my_net;
       delete pl;
     //}
       my_cnet->deleteContent();
#endif
   

#if 0
    //Here we look at actual bond percolation on a network.
    //We create a network, then delete edges with probability p, and
    //look at the connected components. 
    int nodes;
    
    if( argc > 1) {
        nodes = atoi(argv[1]);
    }
    else {
        nodes = 5000;
    }
    Ran1Random r1(-1234), r2(-23456);
    DoublePrefAtNetwork my_net(nodes, r1, 1, 2);
    //This is the network we will remove nodes from:
    Network tmp_net;
    //We get the edges because we will selectively delete some:
    Network::EdgeSet::const_iterator e_it;
    double keep_prob;
    if (argc > 2) {
        keep_prob = atof( argv[2] );
    }
    else {
        keep_prob = 0.1;
    }
   
    for(keep_prob = 0.0001; keep_prob < 1.0; keep_prob *= 1.05) {
    r2.setBoolTrueBias(keep_prob);

    //Copy the network:
    tmp_net = my_net;
    for(e_it = my_net.getEdges().begin(); e_it != my_net.getEdges().end(); e_it++) {
        if(r2.getBool() == false) {
            tmp_net.remove( *e_it );
	}
    }
    //Find the largest component:
    set<Network> comps = tmp_net.getComponents();
    set<Network>::const_reverse_iterator comp_it = comps.rbegin();
    
    cout << keep_prob << " " << comp_it->getNodes().size() << endl;
    
#if 0
    cout << "components: " << comps.size() << " ";
    cout << "nodes: " << nodes << " p: " << keep_prob << " max comp: " << comp_it->getNodes().size()
	    << endl;
#endif
    }

#endif
    

#if 0
    //What is the largest degree as a function of network size?

    PowerLawProbabilityFunction* pl = 0;
    Network* my_net = 0;
    Ran1Random my_rand;

    int nodes = 10;
    for(nodes = 10; nodes < 10000; nodes += 200) {
      pl = new PowerLawProbabilityFunction(-2.0,1,nodes);
      my_net = new DegreeLawRandomNetwork(nodes,*pl,my_rand);
      //Find the maximum degree:
      map<int, int> deg_dist = my_net->getDegreeDist();
      map<int, int>::reverse_iterator deg_it = deg_dist.rbegin();
      cout << nodes << " " << deg_it->first << endl;
      delete pl;
      pl = 0;
      delete my_net;
      my_net = 0;
    }
#endif 
  return 1;
}
