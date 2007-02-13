#include <netmodeler.h>
#include <fstream>

using namespace Starsky;
using namespace std;

int main(int argc, char *argv[]) {
   if (argc < 2) {
      cerr << "Usage: " << argv[0] << ", Prewire, Pgreedy" << endl;
   }
   //for (int i=1;i<argc;i++) {cout << argv[i]<<endl;} 
   int nodes = 2048;
   double p_rewire=atof(argv[1]);
   //int seed = atoi(argv[2]);
   double p_greedy = atof(argv[2]);
   //int max_iter=atoi(argv[3]);
   int max_iter=500;
   //int pairs=atoi(argv[4]);
   int pairs=500;
   int method;
   double p_reach;
   double avePreach;
   double p_reach1;
   double hop1_Preach1;
   int sum_hits1;
   int sum_hops1;
   double varHit;
   double aveHops;
   double tAveHops;
   double aveVar;
   //Ran1Random rand= Ran1Random(seed);
   Ran1Random rand;
   cout << "#network size: " << nodes << "\n"
	<< "#p_greedy\t" << "hit_rate1\t" << "hit_var\t" << "hops1\t" << "hop1_Preach1" << endl; 
   //for (double p_greedy = 0.00; p_greedy < 1.01; p_greedy += 0.01) {
      avePreach=0;
      tAveHops=0;
      aveVar = 0;
      double sum_aveHops = 0;
      double sum_varHit = 0;
      double sum_Preach = 0;
      //cout << "-----------------------------------" << endl << "P greedy: " << p_greedy << endl;
      auto_ptr<SWNetwork> my_net1 ( new SWNetwork(nodes,rand,2) );
      EdgeFactory ef1;
      SWEdgeRewirer mapper1(ef1, p_rewire, rand);
      mapper1.map(my_net1.get(), 1);
      int pair_times;
      for (pair_times=0; pair_times < pairs; pair_times++) {
        UniformNodeSelector uns_source1(rand);
        uns_source1.selectFrom(my_net1.get() );
        RandAddrNode* source1 = dynamic_cast<RandAddrNode*> (uns_source1.select());
        RandAddrNode* target1 = dynamic_cast<RandAddrNode*> (uns_source1.select(source1));
	//cout << "source and target: " << source1->getAddress() << " and " << target1->getAddress() << endl;
	auto_ptr<RandRoutingMessage> my_msg1 ( new RandRoutingMessage(rand,p_greedy, nodes,0) );
	my_msg1->setTarget(target1);
        sum_hits1 = 0;
        sum_hops1 = 0;
        varHit = 0;
	p_reach = 0;
	hop1_Preach1=0;
	aveHops = 0;
        for (int it_no = 0; it_no < max_iter; it_no++) {
	  //cout << it_no << "'s iteration" << endl;
	  auto_ptr<SWNetwork> visited_net1(my_msg1->visit(source1, *my_net1) );
	  sum_hops1 = sum_hops1 + my_msg1->hops; 
	  //cout << "sum_hops: " << sum_hops1 << endl;
	  if ( (visited_net1->has(source1) ) && (visited_net1->has(target1) ) ) {
	    sum_hits1 = sum_hits1 + 1;
	    //cout << "hit " << endl; 
	  }
	}
	//cout << "sum_hits"<< sum_hits1 << endl;
	//cout << "max_iter"<< max_iter << endl;
        p_reach = (double)sum_hits1 / (double)max_iter;
	sum_Preach += p_reach;
	//cout << "ave_hits"<< p_reach << endl;
	//double tmp_varHit=( pow( (1-p_reach),2) *sum_hits1 + pow( (0-p_reach),2)*(max_iter-sum_hits1)  ) ;
	//cout << "tmp_varHit\t" << tmp_varHit << endl;
	//varHit = tmp_varHit / (double)(max_iter-1 );
	varHit=( pow( (1-p_reach),2) *sum_hits1 + pow( (0-p_reach),2)*(max_iter-sum_hits1)  ) / (double)(max_iter-1 );
	//cout << "varHit\t" << varHit << endl;
	sum_varHit+=varHit;
	aveHops = sum_hops1 / max_iter;
	sum_aveHops += aveHops;
	//cout << "aveHops  " << aveHops << endl;
	//cout << p_reach << " and " << varHit << endl;
	
      }
      avePreach = sum_Preach / pair_times;
      tAveHops = sum_aveHops / pair_times;
      aveVar = sum_varHit / pair_times;
      hop1_Preach1 = tAveHops / avePreach; 
      //p_reach1 = (double)sum_hits1 / (double)max_iter;
      //hop1_Preach1 = p_reach / p_reach1;
      cout << p_greedy << "\t" << avePreach << "\t" << aveVar << "\t" << tAveHops <<  "\t" << hop1_Preach1 << endl;
   //}
}
