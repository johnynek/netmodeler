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

#include "trustgraphmatrix.h"
#include "directednetwork.h"
#include "node.h"
#include "directedweightednetwork.h"
#include <cmath>
#include <map>

#define DEBUG

using namespace std;
using namespace Starsky;

TrustGraphMatrix::TrustGraphMatrix(const DirectedWeightedNetwork& network):_network(network)
{

}

double TrustGraphMatrix::TrustMatrixElement(Node* i, Node* j)
{
  double prob = 0.0;
  double denom = _network.getOutStrength(i);
  double numerator = _network.getDirectedWeight(i,j); //get the weight of the directed edge from i to j
  
  if (denom > 0 )
  {
    prob = numerator/denom;  
  }
  else if ( denom == 0 && ((*j).toString() == "s00005486@00001208" || (*j).toString() == "s00005961@00001208") ){
    prob = 0.5;
  }
  /*else if ( denom == 0 && ((*j).toString() == "A" || (*j).toString() == "B") ){
    prob = 0.5;
    }*/
  else{
    prob = 0.0;
  }
  return prob;
}

double TrustGraphMatrix::TrustMatrixElement(Node* i, Node* j, RealContentNetwork* rc_net, Network::NodePSet pre_trusted)
{
  double prob = 0.0;
  std::map<Node*, double> ltv = rc_net->obtainRealNode(i)->getLTV();
  if(!ltv.empty()){
    std::map<Node*, double>::const_iterator m_it = ltv.find(j);
    if( m_it != ltv.end()){
      prob = ltv[j];
    }
  }
  else{
    Network::NodePSet::const_iterator n_it = pre_trusted.find(j);
    if(n_it != pre_trusted.end()){
      prob = 1.0/(double)pre_trusted.size();
    }
  }
  return prob;
}


void TrustGraphMatrix::TrustMatrixToAsciiFile(
          std::string base_name, 
          std::vector<Node*> matrix_order)
{
  vector<Node*>::const_iterator rows;
  vector<Node*>::const_iterator cols;
  filebuf fb;
  fb.open (base_name.c_str(),ios::out);
  ostream out(&fb);

  out << '\t';
  for (cols = matrix_order.begin(); cols != matrix_order.end(); cols++ )
  {
       out << (*cols)->toString() << '\t';  
  } 
  out << endl;
  
  vector<Node*>::const_iterator order_start = matrix_order.begin();
  for(rows = matrix_order.begin(); rows != matrix_order.end(); rows++) 
  {
    out << (*rows)->toString() << '\t';
    for (cols = matrix_order.begin(); cols != matrix_order.end(); cols++ )
    {
       out << TrustMatrixElement( *rows,*cols) << '\t';  
    }      
    out << endl;
  }
}

vector<double>  TrustGraphMatrix::getPrincipalEigVector(vector<Node*> matrix_order, 
							Network::NodePSet pre_trusted, RealContentNetwork* rc_net, const double eps)
{
  int sz = matrix_order.size();
  double d = 0.9;  //corresponds to the damping factor in google pagerank computation
  vector<Node*>::const_iterator rows, cols, m_it;
  vector<double>::iterator it1, it2;
  vector<double> vec(sz), new_vec(sz);

  int pre_size = pre_trusted.size();

  it1 = vec.begin();
  for(m_it = matrix_order.begin(); m_it != matrix_order.end(); m_it++){
    if( pre_trusted.find( *m_it ) != pre_trusted.end() ){
      *it1 = 1.0/(double)pre_size;
    }//nodes in the pre_trusted set  are "pre-trusted" peers
    else{
      *it1 = 0.0;
    }
    it1++;
  }//the initial vector

  int counter = 0;
  double sum;

  do{
    
    for(it2 = new_vec.begin(); it2 != new_vec.end(); it2++){
      *it2 = 0.0;
    }

  it2 = new_vec.begin();
  for(cols = matrix_order.begin(); cols != matrix_order.end(); cols++) 
  {        
    it1 = vec.begin();
    for (rows = matrix_order.begin(); rows != matrix_order.end(); rows++ )
    {    
      ///Attention: two implementations of TrustMatrixElement fcn, one
      ///for a weighteddirectednetwork and one for a realcontentnetwork
      ///*it2 += TrustMatrixElement(*rows, *cols) * (*it1);
      *it2 += TrustMatrixElement(*rows, *cols, rc_net, pre_trusted) * (*it1);
      it1++;
    }        
    it2++;  //we have problem here!!  
  }
  
  it2 = new_vec.begin();
  for(m_it = matrix_order.begin(); m_it != matrix_order.end(); m_it++){
    if( pre_trusted.find( *m_it ) != pre_trusted.end() ){
      *it2 = (d * (*it2) + (1.0 - d)/(double)pre_size );
    }//node A and B are "pre-trusted" peers
    else{
      *it2 = d * (*it2);
    }
    it2++;
  }

  sum = 0.0;
  it2 = new_vec.begin();
  for(it1 = vec.begin(); it1 != vec.end(); it1++){
    double delta = *it1 - *it2;
    sum += delta*delta;
    it2++;
  }

  vec = new_vec;
  counter++;
  }while(sum > eps*eps && counter < 50);
#ifdef DEBUG
  cout << "The number of iteration is: " << counter << endl;
  double check_sum = 0.0, val;
  for(it2 = new_vec.begin(); it2 != new_vec.end(); it2++){
    double val = *it2;
    cout << val << " ";
    check_sum += val;    
  }
  cout << "Value of the check sum is: " << check_sum << endl;
#endif
  return new_vec;
} 

vector<double>  TrustGraphMatrix::getPrincipalEigVectorLog(vector<Node*> matrix_order, Network::NodePSet pre_trusted, const double eps)
{
  int sz = matrix_order.size();
  vector<Node*>::const_iterator rows;
  vector<Node*>::const_iterator cols;
  vector<Node*>::const_iterator m_it;
  vector<double>::iterator it1;
  vector<double>::iterator it2;
  vector<double> vec(sz);
  vector<double> new_vec(sz);  
  double d = 0.9;

  int pre_size = pre_trusted.size();
  cout << "Number of pre_trusted peers: " << pre_size << endl;

  it1 = vec.begin();
  for(m_it = matrix_order.begin(); m_it != matrix_order.end(); m_it++){
    if( pre_trusted.find( *m_it ) != pre_trusted.end() ){
      *it1 = log(1.0/(double)pre_size);
    }//nodes in the pre_trusted set  are "pre-trusted" peers
    else{
      *it1 = 1.0; //Here we use 1.0 to denote -inf
    }
    it1++;
  }//the initial vector

  int counter = 0;
  int row_counter;
  double logsum, checksum = 1.0;
  bool found_nz;  //found first non_zero element

  do{
    checksum = 1.0;
    for(it1 = vec.begin(); it1 != vec.end(); it1++){
      if( (*it1) != 1.0 ){
	//cout << "log domain: " << *it1 << " reg domain: " << exp(*it1) << endl;
	if(checksum != 1.0)
	  checksum = logadd(checksum, *it1);
	else
	  checksum = *it1;
      }
            
    }
    cout << "The sum is: " << exp(checksum) << endl;

  for(it2 = new_vec.begin(); it2 != new_vec.end(); it2++){
    *it2 = 1.0; //again, 1.0 denotes -inf (negative infinity
  }
  it2 = new_vec.begin();
  for(cols = matrix_order.begin(); cols != matrix_order.end(); cols++) 
  {        
    it1 = vec.begin();
    row_counter = 0;
    found_nz = false;
    for (rows = matrix_order.begin(); rows != matrix_order.end(); rows++ )
    {      
      row_counter++;
      if(row_counter == 1 || !found_nz){
	if( TrustMatrixElement( *rows,*cols) != 0.0 && (*it1) != 1.0 ){ //notice that TME is in reg domain, *it1 is in log domain
	  *it2 = log(TrustMatrixElement( *rows,*cols)) + (*it1);
	  found_nz = true;
	}
	it1++;
      }
      else{
	if( TrustMatrixElement( *rows,*cols) != 0.0 && (*it1) != 1.0 ){
	  *it2 = logadd(*it2, log(TrustMatrixElement( *rows,*cols)) + (*it1));
	}
	it1++;
      }
    }      
    it2++;
  }

  it2 = new_vec.begin();
  for(m_it = matrix_order.begin(); m_it != matrix_order.end(); m_it++){
    if( pre_trusted.find( *m_it ) != pre_trusted.end() ){
      if( (*it2) != 1.0) {
	*it2 = logadd( log(d) + (*it2) , log( 1.0 - d ) - log( (double)pre_size));
      }
      else{
	*it2 = log( 1.0 - d ) - log( (double)pre_size);
      }
    }//node A and B are "pre-trusted" peers
    else{
      if( (*it2) != 1.0){
	*it2 = log(d) + (*it2);
      }
    }
    it2++;
  }
  
  double logdelta;
  logsum = 1.0; //again, 1.0 here denotes -inf
  it2 = new_vec.begin();
  for(it1 = vec.begin(); it1 != vec.end(); it1++){
    if( (*it1) != 1.0 && (*it2) != 1.0 ){
      logdelta = logminus(*it1, *it2);      
    }
    else if( (*it1) == 1.0 && (*it2) != 1.0 ) {
      logdelta = *it2;
    }
    else{
      logdelta = 1.0;
    }

    if(logsum == 1.0 && logdelta != 1.0){
      logsum = 2.0*logdelta;
    }
    else if(logsum != 1.0 && logdelta != 1.0){
      logsum = logadd(logsum, 2.0*logdelta);
    }
    else{
    }
    it2++;
  }

  vec = new_vec;
  counter++;
  cout << "Counter is at: " << counter << endl;
  }while(logsum > 2.0*log(eps) && counter < 50);

  cout << "The total number of iteration is: " << counter << endl;

  return new_vec;
} 


double TrustGraphMatrix::logadd(double num1, double num2){
  	if(num1 <= num2){
	  return num2 + log(1.0 + exp(-1.0*fabs(num1 - num2)));
	}
	else{
	  return num1 + log(1.0 + exp(-1.0*fabs(num1 - num2)));
	}
}

double TrustGraphMatrix::logminus(double num1, double num2){
  	if(num1 <= num2){
	  return num2 + log(1.0 - exp(-1.0*fabs(num1 - num2)));
	}
	else{
	  return num1 + log(1.0 - exp(-1.0*fabs(num1 - num2)));
	}
}
