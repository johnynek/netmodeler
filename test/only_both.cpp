#include <netmodeler.h>
#include <fstream>

using namespace Starsky;
using namespace std;

int main(int argc, char *argv[]) {
   if (argc < 3) {
      cerr << "Usage: " << argv[0] << ", Prewire, seed, max_iteration" << endl;
   }
   for (int i=1;i<argc;i++) {cout << argv[i]<<endl;} 
   //int max_node=atoi(argv[1]);
   int nodes = 2048;
   //double p_greedy=atof(argv[2]);
   double p_rewire=atof(argv[1]);
   //double p_greedy_start = atof(argv[2]);
   int seed = atoi(argv[2]);
   //double p_greedy_end = atof(argv[3]);
   int max_iter=atoi(argv[3]);
   //int max_iter=1000;
   int method;
   double ave_hop1 = 0;
   double p_reach1 = 0;
   double hop1_Preach1 = 0;
   int reachables1;
   int sum_hops1;
   Ran1Random rand= Ran1Random(seed);
   cout << "#network size: " << nodes << "\n"
	<< "#p_greedy\t " << "hit_rate1\t" << "hops1\t" << "hop1_Preach1" << endl; 
   for (double p_greedy = 0.00; p_greedy < 1.01; p_greedy += 0.01) {
   //double p_greedy_end = p_greedy_start + 0.11;
   //for (double p_greedy = p_greedy_start; p_greedy <= p_greedy_end; p_greedy += 0.01) {
      reachables1 = 0;
      sum_hops1 = 0;
      
      for (int it_no = 0; it_no < max_iter; it_no++) {
	 //int tmp = rand.getInt(nodes);
	 auto_ptr<SWNetwork> my_net1 ( new SWNetwork(nodes,rand,2) );
	 //cout << "------------------------new network made---------------------" << endl;
	 auto_ptr<RandRoutingMessage> my_msg1 ( new RandRoutingMessage(rand,p_greedy, nodes,0) );
	 UniformNodeSelector uns_source1(rand);
	 uns_source1.selectFrom(my_net1.get() );
	 RandAddrNode* source1 = dynamic_cast<RandAddrNode*> (uns_source1.select());
         RandAddrNode* target1 = dynamic_cast<RandAddrNode*> (uns_source1.select(source1));
	 my_msg1->setTarget(target1);
	 //cout << "source and target: " << source1->getAddress() << "\t" << target1->getAddress() << endl;
	 //cout << "new net's size: " << my_msg1->new_net->getNodeSize() << endl;
	 EdgeFactory ef1;
	 SWEdgeRewirer mapper1(ef1, p_rewire, rand);
	 //cout << "mapping" << endl;
	 mapper1.map(my_net1.get(), 1);
	 //cout << "mapping finished" << endl;
	 auto_ptr<SWNetwork> visited_net1(my_msg1->visit(source1, *my_net1) );
	 //cout << "message routing finished" << endl;
	 //sum_hops1 = sum_hops1 + visited_net1->getNodeSize(); 
	 sum_hops1 = sum_hops1 + my_msg1->hops; 
	 //cout << "-----------------------hops hops----------------------" << endl;
	 if ( (visited_net1->has(source1) ) && (visited_net1->has(target1) ) ) {
	    reachables1 = reachables1 + 1;
	 }
      }
      ave_hop1 = (double)sum_hops1 / (double)max_iter;
      p_reach1 = (double)reachables1 / (double)max_iter;
      hop1_Preach1 = ave_hop1 / p_reach1;
      cout << p_greedy << "\t" << p_reach1 << "\t" << ave_hop1 << "\t" << hop1_Preach1 << endl;
   }
}
