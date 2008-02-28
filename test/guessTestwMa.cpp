#include <netmodeler.h>
#include <fstream>
#include <cmath>
#include <ctime>
#include <map>

using namespace Starsky;
using namespace std;

/*
 * This test code is to see the distribution of
 * estimated network sizes for cache and query.
 */

/**
 * returns 1-exp(-a*n/(sqrt(n)*sqrt(m)))
 * @param sx, sqrt(x), @param sy, sqrt(y)
 * @param an, -alpha * net_size
 */
double myFunc(double sx, double sy, double man) {
  return 1 - exp(man/(sx*sy));
}

map<double,double> makePdfMap(const multiset<int>& in_set, int n_size)
{
  map<double,double> ret_map;
  multiset<int>::const_iterator it;
  double prev = 0.0;
  for (it = in_set.begin(); it != in_set.end(); it++) {
    double cur_val = sqrt(*it);
    //cout << "it: " << *it << "\tcur_val: " << cur_val << endl;
    if (cur_val != prev) {
      //cout << "prev: " << prev << "\tin_set.count(*it): " << in_set.count(*it) << endl;
      ret_map[cur_val] = (double)(in_set.count(*it) / (double)(n_size));
      prev = cur_val;
    }
    //cout << "it: " << *it << "\tcur_val: " << cur_val << "\t ret_val: " << ret_map[cur_val] << endl;
  }
  //cout << ret_map.size() << endl;
  return ret_map;
}


int main(int argc, char* argv[])
{
  //----------------------------------------------------------------------
  // make network and every single node estimates networksize 
  clock_t start = clock();
  if (argc < 3) 
  {
    cerr << "Usage: " << argv[0] << " , net size, alpha, max iteration" << endl;
    //cerr << "Usage: " << argv[0] << " , net size, max iteration" << endl;
  }
  int seed = -1;
  //int seed = atoi(argv[2]);
  Ran1Random ran_no = Ran1Random(seed);
  int nodes = atoi(argv[1]);
  //int m_seed = -atoi(argv[3])-1;
  int m_seed = -atoi(argv[2])-1;
  multiset<int> c_e_size, q_e_size; 
  int c_max = 0;
  int q_max = 0;
  int c_min = nodes, q_min = nodes;
  while (seed !=m_seed)
  {
    auto_ptr<DeetooNetwork> evennet( new DeetooNetwork(nodes, ran_no) );
    evennet->createEvenNet(nodes);
    //evennet->create(nodes);
    auto_ptr<NodeIterator> ni(evennet->getNodeIterator() );
    while (ni->moveNext() ) {
	AddressedNode* cn = dynamic_cast<AddressedNode*>(ni->current() );
	//int c_tmp = evennet->guessNetSize(cn,true);
	int c_tmp = evennet->guessNetSizeLog(cn,true);
	//c_e_size.insert(evennet->guessNetSize(cn,true) );
	c_e_size.insert(c_tmp);
	if (c_tmp > c_max) { c_max = c_tmp; }
	if (c_tmp < c_min) { c_min = c_tmp; }
	
    }
    auto_ptr<DeetooNetwork> queryNet_ptr ( new DeetooNetwork(ran_no) );
    queryNet_ptr->createQueryNet( (evennet.get() )->node_map);
    auto_ptr<NodeIterator> niq(queryNet_ptr->getNodeIterator() );
    while (niq->moveNext() ) {
	AddressedNode* cn = dynamic_cast<AddressedNode*>(niq->current() );
	//q_e_size.insert(queryNet_ptr->guessNetSize(cn,false) );
	//int q_tmp = queryNet_ptr->guessNetSize(cn,false);
	int q_tmp = queryNet_ptr->guessNetSizeLog(cn,false);
	q_e_size.insert(q_tmp);
	if (q_tmp > q_max) { q_max = q_tmp; }
	if (q_tmp < q_min) { q_min = q_tmp; }
    }
    /**
    cout << "c_max q_max: " << c_max << "\t" << q_max << "\t" 
    	 << "c_min q_min: " << c_min << "\t" << q_min << "\t" << endl;
    
    multiset<int>::iterator m_it;
    for (m_it = c_e_size.begin(); m_it != c_e_size.end() ; m_it++) {
	    cout << *m_it << endl;
    }
    multiset<int>::iterator q_it;
    for (q_it = q_e_size.begin(); q_it != q_e_size.end() ; q_it++) {
	    cout << *q_it << endl;
    }
    */
    seed--;
  }
  //**********************************************************************
  map<double,double> cpdf = makePdfMap(c_e_size,nodes);
  map<double,double> qpdf = makePdfMap(q_e_size,nodes);
  //cout << "cpdf_size: " << cpdf.size() << "\tqpdf_size: " << qpdf.size() << endl;
  //double ma_n = -alpha * (double)nodes;
  double ma_n = -0.1 * (double)nodes;
  vector<double> ma_vec(11,0.0);
  ma_vec.at(0) = ma_n;
  int po = 1;
  for (double a = 0.5; a <= 5.0 ; a += 0.5) {
    ma_n = -a * (double)nodes;
    ma_vec.at(po) = ma_n;
    po++;
  }
  //double hit_rate = 0.0; 
  vector<double> hit_r(11,0.0);
  map<double,double>::const_iterator cit;
  for(cit=cpdf.begin(); cit!=cpdf.end(); cit++)
  {
    double sx = cit->first;
    double c_val = cit->second;
    //cout << "sx: " << sx << "\tcpdf: " << c_val << endl;
    //double q_sum = 0.0;
    vector<double> q_sum(11,0.0);
    map<double,double>::const_iterator qit;
    for(qit=qpdf.begin();qit!=qpdf.end(); qit++)
    {
      double sy = qit->first;
      double q_val = qit->second;
      //cout << "sy: " << sy << "\tqpdf: " << q_val << endl;
      vector<double>::const_iterator ma_it;
      int pos = 0;
      for (ma_it = ma_vec.begin(); ma_it != ma_vec.end(); ma_it++) {
	///cout << "ma_it: " << *ma_it << endl;
        q_sum.at(pos) += myFunc(sx,sy,*ma_it) * q_val;
        pos++;
      }
    }
    int pos1 = 0;
    vector<double>::const_iterator ma_it1;
    for (ma_it1 = q_sum.begin(); ma_it1 != q_sum.end(); ma_it1++)
    {
      hit_r.at(pos1) += (*ma_it1) * c_val;
      pos1++;
    }
  }
  //cout << nodes << "\t" << hit_rate << endl;
  cout << nodes ;
  vector<double>::const_iterator re_it;
  for (re_it = hit_r.begin(); re_it != hit_r.end(); re_it++)
  {
    cout << "\t" << *re_it;
  }
  cout << endl;
  clock_t end = clock();
  //cout << "running time: " << (double)(end - start)/CLOCKS_PER_SEC << endl;
}

    
