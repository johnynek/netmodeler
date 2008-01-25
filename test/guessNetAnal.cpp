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

/*
 * myExp is function calculating sum of 4 exponentials
 */
//double myExp(int x, int y, int n, double a)
double myExp(double sx, double sx1, double sy, double sy1, double ma_over_n)
{
  //double sx1 = sqrt((double)x + 1.0);
  //double sy1 = sqrt((double)y + 1.0);
  //double sx = sqrt((double)x);
  //double sy = sqrt((double)y);
  double ret_val = 0.0;
  //double ma_over_n = -a/(double)n;
  ret_val = -exp(ma_over_n * sx1 * sy1);
  ret_val += exp(ma_over_n * sx1 * sy );
  ret_val += exp(ma_over_n * sx  * sy1);
  ret_val -= exp(ma_over_n * sx  * sy );
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
  /**
  double ret, n1, n2, c1, c2; 
  map<int,double>::const_iterator it = imap.begin();
  map<int,double>::const_iterator ite = imap.end();
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
  */
  double ret;
  map<int,double>::const_iterator it = imap.upper_bound(k);
  it--;
  ret = it->second;
    
  return ret;
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
    //evennet->createEvenNet(nodes);
    evennet->create(nodes);
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
    cout << "c_max q_max: " << c_max << "\t" << q_max << "\t" 
    	 << "c_min q_min: " << c_min << "\t" << q_min << "\t" << endl;
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
  // get the cdf from the data(c_e_size and q_e_size distribution).
  //***********************************************************************
  // limiting cdfs to 99%
  /**
  int c_step = (int)(c_max / 99.0);
  int q_step = (int)(q_max / 99.0);
  if (c_step == 0) { c_step = 1; }
  if (q_step == 0) { q_step = 1; }
  int c_max_pt = 0;
  int q_max_pt = 0;
  float cdf_limit = 0.99;
  for (int c_no = 0; c_no <= c_max; c_no += c_step)
  {
    double cct = myCount(c_e_size,c_no);
    if (cct <= cdf_limit) {
      c_max_pt = c_no;
    }
    else { break; }
    //if (cct <=0.99 && qct <= 0.99) {break;}
  }
  //cout << "q_max, q_step: " << q_max << "\t" << q_step << endl;
  for (int q_no = 0; q_no <= q_max; q_no += q_step)
  {
    double qct = myCount(q_e_size,q_no);
    if (qct <= cdf_limit) {
      q_max_pt = q_no;
    }
    else { break; }
    //if (cct <=0.99 && qct <= 0.99) {break;}
  }
  */
  //cout << "maxs: " << c_max_pt <<"\t" << q_max_pt << endl;
  //**********************************************************************
  map<int,double> qcdf, ccdf;

  ccdf[0]=0.0;
  qcdf[0]=0.0;
  multiset<int>::iterator c_it;
  for (c_it = c_e_size.begin(); c_it != c_e_size.end() ; c_it++)
  {
    int past_c_size = -1;	  
    int current_c_size = *c_it;
    if(current_c_size != past_c_size)
    {
      ccdf[current_c_size] = myCount(c_e_size,current_c_size);	  
    }
  }
  multiset<int>::iterator q_it;
  for (q_it = q_e_size.begin(); q_it != q_e_size.end() ; q_it++)
  {
    int past_q_size = -1;	  
    int current_q_size = *q_it;
    if(current_q_size != past_q_size)
    {
      qcdf[current_q_size] = myCount(q_e_size,current_q_size);	  
    }
  }
  //******************************************************************************
  /**
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
  //For a small size network, it happens zero step size. do not let this happen.
  if (step == 0) 
  { 
    step = 1;
    rg_max = max_pt+step;
  }
  else
  {
    rg_max = max_pt + step -1;
  }
  cout << "rg_max, step, max_pt, min_pt: " << rg_max << "\t" << step << "\t" << max_pt << "\t" << min_pt << endl;
  int c_max_pt = 0;
  int q_max_pt = 0;
  // count cdf till 99% for faster running time.
  map<int,double> qcdf, ccdf;
  for (int no = min_pt; no <= rg_max; no+=step)
  {
    double cct = myCount(c_e_size,no);
    double qct = myCount(q_e_size,no);
    if (cct <= 0.99) {
      ccdf[no] = cct;
      c_max_pt = no;
    }
    if (qct <= 0.99) {
    qcdf[no] = qct;
    q_max_pt = no;
    }
  }
  cout << c_max_pt << "\t" << q_max_pt << endl;
  */
  /**
  //print out cache cdf for checking if it's right.
  map<int,double>::iterator iit;
  for (iit=ccdf.begin(); iit != ccdf.end(); iit++)
  {
    cout << iit->first << "\t" << iit->second << endl;
  }
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  map<int,double>::iterator qit;
  for (qit=qcdf.begin(); qit != qcdf.end(); qit++)
  {
    cout << qit->first << "\t" << qit->second << endl;
  }
  */
  //-----------------------------------------------------------------------
  //The last part: Let's get the hit rate using ccdf and qcdf
  //double hit_rate = 0.0;


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //vector<double> hit_rate(11,0.0);
  //for (int n = 0; n <= c_max_pt; n++)

  /**
  for (int n = 0; n <= c_max; n++)
  {
    double tmp = 0.0;
    //cout << "-=---------------------------------" << endl;
    //vector<double> tmp(11,0.0);
    //tmp.clear();
    //tmp.assign(11,0);
    //int m = 69742;
    //for (int m = 0; m <= q_max_pt; m++)
    for (int m = 0; m <= q_max; m++)
    {
      double v_t = myExp(n,m,nodes,alpha);
      double cdf_t = getCdf(qcdf,m);
      tmp += (1-cdf_t) * v_t;
      //cout << "m, myExp, cdf, tmp: " << m << "\t" << v_t << "\t" << cdf_t << "\t" << tmp << endl;
      //tmp.at(0) += (1-cdf_t) * myExp(n,m,nodes,0.1);
      //int pos1 = 1;
      //for (double al = 0.5; al <=5; al+=0.5)
      //{
        //tmp += (1-cdf_t) * v_t;
      //  tmp.at(pos1) += (1-cdf_t) * myExp(n,m,nodes,al);
      //  pos1++;
      //}
      
    }
    
    //vector<double>::iterator t_it;
    //int pos = 0;
    //for (t_it = tmp.begin(); t_it != tmp.end(); t_it++)
    //{
    //  hit_rate.at(pos) += (*t_it) * (1-getCdf(ccdf,n) );
    //  pos++;
    //}
    double tmp_cdf = getCdf(ccdf,n);
    hit_rate += tmp * (1 - tmp_cdf);
    //cout << nodes << "\t" << hit_rate << endl;
    //cout << "n,tmp,tmp_cdf,hit_rate: " << n << "\t" << tmp << "\t" << tmp_cdf << "\t" << hit_rate << endl;
  }
  */

  //cout << "------------------------------------------" << endl;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  double hit_rate = 0.0;
  double ma_over_n = -alpha / (double)nodes;
  map<int,double>::iterator cit;
  for(cit=ccdf.begin(); cit!=ccdf.end(); cit++)
  {
    int x0 = cit->first;
    double c_val = cit->second;
    cit++;
    if (cit == ccdf.end() ) {break;}
    int x1 = cit->first;
    cit--;
    double tmp_t = 0.0;
    double sx = 0.0;
    double sx1 = 0.0;
    //cout << "x0, x1: " << x0 << "\t" << x1 << endl;
    for(int n = x0; n < x1; n++)
    {
      if (sx1 == 0.0) 
      {
	sx = sqrt((double)n);
      }
      else
      {
        sx = sx1;
      }
      sx1 = sqrt((double)n+1.0);
      map<int,double>::iterator qit;
      double q_ret = 0.0;
      for(qit=qcdf.begin();qit!=qcdf.end(); qit++)
      {
        int y0 = qit->first;
	double q_val = qit->second;
	qit++;
	if(qit == qcdf.end() ) { break; }
	int y1 = qit->first;
	qit--;
        //cout << "y0, y1: " << y0 << "\t" << y1 << endl;
	double tmp = 0.0;
	double sy = 0.0;
	double sy1 = 0.0;
	for(int m = y0; m < y1; m++)
	{
	  if (sy1 == 0.0)
	  {
	    sy = sqrt((double)m);
	  }
	  else
	  {
            sy = sy1;
	  }
	  sy1 = sqrt((double)m+1.0);
	  //cout << "n, m, sx, sx1, sy, sy1, ma: " << n << "\t" << m << "\t" << sx << "\t" << sx1 << "\t" << sy << "\t" << sy1 << "\t" << ma_over_n << endl;
          tmp += myExp(sx,sx1,sy,sy1,ma_over_n);
	}
	//cout << "tmp: " << tmp << endl;
	q_ret += (1-q_val) * tmp;
      }
      //cout << "q_ret: " << q_ret << endl;
      tmp_t += q_ret;
    }
    //cout << "tmp_t, hit_rate: " << tmp_t << "\t" << (1-c_val)*tmp_t << endl;
    hit_rate += (1-c_val)*tmp_t;
    //cout << "hit_rate: " << hit_rate << endl;
  }
  cout << nodes << "\t" << hit_rate << endl;
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

    
