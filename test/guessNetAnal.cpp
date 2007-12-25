#include <netmodeler.h>
#include <fstream>
#include <cmath>
#include <map>

using namespace Starsky;
using namespace std;

/*
 * This test code is to see the distribution of
 * estimated network sizes for cache and query.
 */

/**
 * myCount Function is calculating cdf for a given val in ary.
 */
double myCount(multiset<int> ary, int val)
{
  int ret_count = 0;
  multiset<int>::iterator mit = ary.begin();
  while(mit!=ary.end() && (*mit <=val) )
  {
    ret_count += 1;
    mit++;
  }
  double ret_val = (double)ret_count / (double)(ary.size() );
  return ret_val;
}

/*
 * myExp is function calculating 4 exponentials
 */
double myExp(int x, int y, int n, double a)
{
  double x1 = x + 1;
  double y1 = y + 1;
  double ret_val = 0.0;
  ret_val = -exp(-a*sqrt(x1*y1) / (double)n);
  ret_val += exp(-a*sqrt(x1*y ) / (double)n);
  ret_val += exp(-a*sqrt(x *y1) / (double)n);
  ret_val -= exp(-a*sqrt(x *y ) / (double)n);
  //cout << "myFucn: " << ret_val << endl;
  return ret_val;
}

/*
 * returns missing cdf data
 * linearly fill empty point
 */
double getCdf(map<int,double> imap, int k)
{
  double ret, n1, n2, c1, c2; 
  map<int,double>::iterator it = imap.begin();
  map<int,double>::iterator ite = imap.end();
  ite--;
  //cout << "==========================================================" << endl;
  if (k < it->first)
  {
    ret = it->second/(double)(it->first) * k;
  }
  else if (k >= ite->first)
  {
    ret = 1.0;
  }   
  else
  {
    it = imap.lower_bound(k);
    n2 = it->first;
    c2 = it->second;
    it--;
    n1 = it->first;
    c1 = it->second;
    ret = ((double)(c2 - c1)/(double)(n2-n1))*(k-n1) + c1; 
    //cout << "k,n1,n2,c1,c2: " << k << "\t" << n1 << "\t" << n2 << "\t" << c1 << "\t" << c2 << endl;
  }
  //cout << "getCdf: " << ret << endl;
  return ret;
}

int main(int argc, char* argv[])
{
  //----------------------------------------------------------------------
  // make network and every single node estimates networksize 
  if (argc < 3) 
  {
    //cerr << "Usage: " << argv[0] << " , net size, alpha, max iteration" << endl;
    cerr << "Usage: " << argv[0] << " , net size, max iteration" << endl;
  }
  int seed = -1;
  //int seed = atoi(argv[2]);
  Ran1Random ran_no = Ran1Random(seed);
  int nodes = atoi(argv[1]);
  //double alpha = atof(argv[2]);
  //int m_seed = -atoi(argv[3])-1;
  int m_seed = -atoi(argv[2])-1;
  multiset<int> c_e_size, q_e_size; 
  while (seed !=m_seed)
  {
    auto_ptr<DeetooNetwork> evennet( new DeetooNetwork(nodes, ran_no) );
    evennet->createEvenNet(nodes);
    auto_ptr<NodeIterator> ni(evennet->getNodeIterator() );
    while (ni->moveNext() ) {
	AddressedNode* cn = dynamic_cast<AddressedNode*>(ni->current() );
	//c_e_size.push_back(evennet->guessNetSize(cn,true) );
	c_e_size.insert(evennet->guessNetSize(cn,true) );
    }
    auto_ptr<DeetooNetwork> queryNet_ptr ( new DeetooNetwork(ran_no) );
    queryNet_ptr->createQueryNet( (evennet.get() )->node_map);
    auto_ptr<NodeIterator> niq(queryNet_ptr->getNodeIterator() );
    while (niq->moveNext() ) {
	AddressedNode* cn = dynamic_cast<AddressedNode*>(niq->current() );
	//q_e_size.push_back(queryNet_ptr->guessNetSize(cn,false) );
	q_e_size.insert(queryNet_ptr->guessNetSize(cn,false) );
    }
    /**
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
  //-----------------------------------------------------------------------
  // get the cdf from the data(c_e_size nad q_e_size distribution.
  multiset<int>::iterator c_it = c_e_size.end();
  multiset<int>::iterator q_it = q_e_size.end();
  c_it--;
  q_it--;
  multiset<int>::iterator c_itS = q_e_size.begin();
  multiset<int>::iterator q_itS = q_e_size.begin();
  int max_pt = max(*c_it,*q_it);
  int min_pt = min(*c_itS,*q_itS);
  int step = (int)(max_pt / 99.0);
  int rg_max = 0;
  if (step == 0) 
  { 
    step = 1;
    rg_max = max_pt+step;
  }
  else
  {
    rg_max = max_pt + step -1;
  }
  //cout << "rg_max, step, max_pt, min_pt: " << rg_max << "\t" << step << "\t" << max_pt << "\t" << min_pt << endl;
  map<int,double> qcdf, ccdf;
  for (int no = min_pt; no <= rg_max; no+=step)
  {
    ccdf[no] = myCount(c_e_size,no);
    qcdf[no] = myCount(q_e_size,no);
  }
  /** print out cache cdf for checking if it's right.
  map<int,double>::iterator iit;
  for (iit=ccdf.begin(); iit != ccdf.end(); iit++)
  {
    cout << iit->first << "\t" << iit->second << endl;
  }
  */
  //-----------------------------------------------------------------------
  //The last part: Let's get the hit rate using ccdf and qcdf
  //double hit_rate = 0.0;
  vector<double> hit_rate(11,0.0);
  for (int n = 0; n <= max_pt; n++)
  {
    //double tmp = 0.0;
    vector<double> tmp(11,0.0);
    //tmp.clear();
    //tmp.assign(11,0);
    for (int m = 0; m <=max_pt; m++)
    {
      //double v_t = myExp(n,m,nodes,alpha);
      double cdf_t = getCdf(qcdf,m);
      tmp.at(0) += (1-cdf_t) * myExp(n,m,nodes,0.1);
      int pos1 = 1;
      for (double al = 0.5; al <=5; al+=0.5)
      {
        //tmp += (1-cdf_t) * v_t;
        tmp.at(pos1) += (1-cdf_t) * myExp(n,m,nodes,al);
	pos1++;
      }
    }
    vector<double>::iterator t_it;
    int pos = 0;
    for (t_it = tmp.begin(); t_it != tmp.end(); t_it++)
    {
    //hit_rate += tmp * (1-getCdf(ccdf,n) );
      hit_rate.at(pos) += (*t_it) * (1-getCdf(ccdf,n) );
      pos++;
    }
  }
  cout << nodes;
  vector<double>::iterator re_it;
  for (re_it = hit_rate.begin(); re_it != hit_rate.end(); re_it++)
  {
    cout << "\t" << *re_it;
  }
  cout << endl;
}
    
