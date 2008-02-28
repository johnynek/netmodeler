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
 * myCount Function is calculating cdf for a given val in ary.
 */
double myCount(const multiset<int>& ary, int val)
{
  int ret_count = 0;
  multiset<int>::const_iterator mit = ary.begin();
  while(mit!=ary.end() && (*mit <=val) )
  {
    ret_count += 1;
    mit++;
  }
  double ret_val = (double)ret_count / (double)(ary.size() );
  return ret_val;
}
/**
 * returns 1-exp(-a*n/(sqrt(n)*sqrt(m)))
 * @param sx, sqrt(x), @param sy, sqrt(y)
 * @param an, -alpha * net_size
 */
double myFunc(double sx, double sy, double man) {
  return 1 - exp(man/(sx*sy));
}

/*
 * quadExp is function calculating sum of 4 exponentials
 */
double quadExp(double sx, double sx1, double sy, double sy0, double ma_t_n)
{
  double ret_val = 0.0;
  //double (*sExp)(double, double, double);
  //sExp = dExp;
  ret_val  = myFunc(sx1,sy ,ma_t_n);
  ret_val -= myFunc(sx ,sy ,ma_t_n);
  ret_val -= myFunc(sx1,sy0,ma_t_n);
  ret_val += myFunc(sx ,sy0,ma_t_n);
  //cout << "\tmy_exp: " << ret_val << endl;
  return ret_val;
}
double duoExp(double sp, double sp1, double cons, double ma_t_n, bool xy) 
{
  double ret_val = 0.0;
  if (xy == true) {
    ret_val = myFunc(sp1, cons, ma_t_n) - myFunc(sp, cons, ma_t_n);
  }
  else {
    ret_val = myFunc(cons, sp1, ma_t_n) - myFunc(cons, sp, ma_t_n);
  }
  return ret_val;
}


/*
 * returns missing cdf data
 * fill linearly calculated cdf value into empty point (k)
 * @param imap: <net size, cdf> pair
 * @param k : net size
 */
double getCdf(const map<int,double>& imap, int k)
{
  map<int,double>::const_iterator it = imap.upper_bound(k);
  it--;
  //double ret = it->second;
  //return ret;
  return it->second;
}
map<int,double> cdfMap(multiset<int> in) {
  map<int,double> ret_map;
  //ret_map[0]=0.0;
  multiset<int>::iterator it;
  for (it = in.begin(); it != in.end() ; it++)
  {
    int past = -1;	  
    int curr = *it;
    if(curr != past)
    {
      ret_map[curr] = myCount(in,curr);	  
      past = curr;
    }
  }
  return ret_map;
}




int main(int argc, char* argv[])
{
  //----------------------------------------------------------------------
  // make network and every single node estimates networksize 
  clock_t start = clock();
  if (argc < 4) 
  {
    cerr << "Usage: " << argv[0] << " , net size, alpha, max iteration" << endl;
    //cerr << "Usage: " << argv[0] << " , net size, max iteration" << endl;
  }
  int seed = -1;
  //int seed = atoi(argv[2]);
  Ran1Random ran_no = Ran1Random(seed);
  int nodes = atoi(argv[1]);
  double alpha = atof(argv[2]);
  //int m_seed = -atoi(argv[3])-1;
  int m_seed = -atoi(argv[3])-1;
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
	int c_tmp = evennet->guessNetSize(cn,true);
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
	int q_tmp = queryNet_ptr->guessNetSize(cn,false);
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
  //map<int,double> ccdf = cdfMap(c_e_size);
  //map<int,double> qcdf = cdfMap(q_e_size);
  map<int,double> ccdf, qcdf;
  ccdf[nodes] = 1.0;
  qcdf[nodes] = 1.0;
  ccdf[nodes-1]=0.5;
  qcdf[nodes-1]=0.5;
  c_min = q_min = nodes-1;
  c_max = q_max = nodes;

  //double ma_over_n = -alpha / (double)nodes;
  double ma_over_n = -alpha * (double)nodes;
  double hit_rate = 0.0; 
  cout << "c_min, q_min: " << c_min << "\t" << q_min << endl;
  double fac = myFunc(sqrt(c_min-1),sqrt(q_min-1),ma_over_n);
  cout << "fac: " << fac << endl;
  double quad_sum = 0.0;
  double sum1 = 0.0, sum2 = 0.0;
  map<int,double>::iterator cit;
  for(cit=ccdf.begin(); cit!=ccdf.end(); cit++)
  {
    int x0 = cit->first;
    double c_val = cit->second;
    cout << "x0: " << x0 << "\tccdf: " << c_val << endl;
    cit++;
    if (cit == ccdf.end() ) {break;}
    int x1 = cit->first;
    cout << "x1: " << x1 << "\tccdf: " << c_val << endl;
    cit--;
    double tmp_t = 0.0;
    double tmp_sum1 = 0.0;
    bool sum2_check = false;
    double sx = 0.0;
    double sx1 = 0.0;
    double c_val_tmp = 0.0;
    //cout << "-----------------------------" << endl;
    //cout << "x0, x1: " << x0 << "\t" << x1 << endl;
    for(int n = x0-1; n < x1; n++)
    {
      if (n < x0) { 
        c_val_tmp = 0; 
      }
      else {
        c_val_tmp = c_val;
      }
      cout << "n, c_val: " << n << "\t" << c_val_tmp << endl;
      if (sx1 == 0.0) 
      {
	sx = sqrt((double)n);
      }
      else
      {
        sx = sx1;
      }
      sx1 = sqrt((double)n+1.0);
      double tmp_sum2 = 0.0;
      map<int,double>::iterator qit;
      double q_ret = 0.0;
      for(qit=qcdf.begin();qit!=qcdf.end(); qit++)
      {
        int y0 = qit->first;
	double q_val = qit->second;
	double q_val_tmp = 0.0;
	qit++;
	if(qit == qcdf.end() ) { break; }
	int y1 = qit->first;
	qit--;
        cout << "y0, y1: " << y0 << "\t" << y1 << endl;
	double tmp = 0.0;
	double sy = 0.0;
	double sy1 = 0.0;
	for(int m = y0-1; m < y1; m++)
	{
	  if (m < y0) { q_val_tmp = 0; }
	  else { q_val_tmp = q_val; }
	  cout << "m,q_val: " << m << "\t" << q_val_tmp << endl;
	  if (sy1 == 0.0)
	  {
	    sy = sqrt((double)m);
	  }
	  else
	  {
            sy = sy1;
	  }
	  sy1 = sqrt((double)m-1.0);
	  //cout << "n, m, sx, sx1, sy, sy1, ma: " << n << "\t" << m << "\t" << sx << "\t" << sx1 << "\t" << sy << "\t" << sy1 << "\t" << ma_over_n << endl;
          tmp += quadExp(sx,sx1,sy,sy1,ma_over_n);
	  cout << "innerst tmp: " << tmp << endl;
	  if ( !sum2_check ) {
	    tmp_sum2 +=duoExp(sy1,sy,(c_min -1),ma_over_n,0);
	    cout << "tmp_sum2: " << tmp_sum2 << endl;
	    cout << "how many" << endl;
	    sum2_check = true;
	  }
	}
	//cout << "tmp: " << tmp << endl;
	q_ret += (1-q_val_tmp) * tmp;
	sum2 += (1-q_val_tmp) * tmp_sum2;
      }
      //if (!sum2_check) {
        tmp_sum1 += duoExp(sx1,sx,(q_min-1),ma_over_n,1);
      //}
      //cout << "q_ret: " << q_ret << endl;
      tmp_t += q_ret;
    }
    sum1 += (1-c_val_tmp) * tmp_sum1;
    //cout << "tmp_t, hit_rate: " << tmp_t << "\t" << (1-c_val)*tmp_t << endl;
    //hit_rate += (1-c_val)*tmp_t;
    quad_sum += (1-c_val_tmp)*tmp_t;
    //cout << "hit_rate: " << hit_rate << endl;
  }
  hit_rate += fac + sum1 + sum2 + quad_sum;
  cout << nodes << "\t" << fac << "\t" << sum1+fac << "\t" << sum2+sum1+fac << "\t" << hit_rate << endl;
  cout << nodes << "\t" << (1-hit_rate) << "\t" << hit_rate << endl;
  /**
  cout << nodes;
  vector<double>::iterator re_it;
  for (re_it = hit_rate.begin(); re_it != hit_rate.end(); re_it++)
  {
    cout << "\t" << *re_it;
  }
  cout << endl;
  */
  clock_t end = clock();
  cout << "running time: " << (double)(end - start)/CLOCKS_PER_SEC << endl;
}

    
