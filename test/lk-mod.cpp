#include <netmodeler.h>
#include <fstream>

using namespace Starsky;
using namespace std;

int main(int argc, char *argv[]) {
   if (argc < 2) {
      cerr << "Usage: " << argv[0] << ", Prewire, Pgreedy" << endl;
   }
   int nodes = 2048;
   double p_rewire=0.5;
   int max_iter=atoi(argv[1]);
   int pairs=5;
   int method;
   double Preach, hop1_Preach1,aveHops;
   int sum_hits1; 
   double sum_hops1;
   Ran1Random rand;
   cout << "#network size: " << nodes << "\n"
	<< "#p_greedy\t" << "hit_rate1\t" << "hops1\t" << "hop1_Preach1" << endl; 
   for (double p_greedy = 0.00; p_greedy < 1.01; p_greedy += 0.01) {
      //cout << "=================================================" << endl;
      //cout << "Pgreedy:\t" << p_greedy << endl;
      auto_ptr<SWNetwork> my_net1 ( new SWNetwork(nodes,rand,2) );
      EdgeFactory ef1;
      SWEdgeRewirer mapper1(ef1, p_rewire, rand);
      mapper1.map(my_net1.get(), 1);
      sum_hits1 = 0;
      sum_hops1 = 0;
      aveHops = 0;
      Preach = 0;
      hop1_Preach1=0;
      int pair_times;
      for (pair_times=0; pair_times < pairs; pair_times++) {
	//cout << pair_times << "'s pair" << endl;
        UniformNodeSelector uns_source1(rand);
        uns_source1.selectFrom(my_net1.get() );
        RandAddrNode* source1 = dynamic_cast<RandAddrNode*> (uns_source1.select());
        RandAddrNode* target1 = dynamic_cast<RandAddrNode*> (uns_source1.select(source1));
	auto_ptr<RandRoutingMessage> my_msg1 ( new RandRoutingMessage(rand,p_greedy, nodes,0) );
	my_msg1->setTarget(target1);
	int it_no=0;
	bool hit=0;
	int hops = 0;
	while ( it_no < max_iter && hit==0) {
	  auto_ptr<SWNetwork> visited_net1(my_msg1->visit(source1, *my_net1) );
	  hops = hops + my_msg1->hops; 
	  if ( (visited_net1->has(source1) ) && (visited_net1->has(target1) ) ) {
	    hit = 1;
	    //cout << "-----------hit---------" << endl;
	  }
	  it_no++;
	}
	sum_hops1+= ( (double)hops / (double)(it_no+1) );
	if (hit == 1) {
	    sum_hits1+=hit;
	}
	//cout << "sum_hits:\t" << sum_hits1 << endl;
      }
      Preach = (double)sum_hits1 / (double)pairs;
      aveHops = sum_hops1 / pair_times;
      hop1_Preach1 = aveHops / Preach; 
      cout << p_greedy << "\t" << Preach << "\t" <<  aveHops <<  "\t" << hop1_Preach1 << endl;
   }
}
