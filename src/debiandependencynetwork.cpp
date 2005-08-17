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

#include "debiandependencynetwork.h"

using namespace Starsky;
using namespace std;

DebianDependencyNetwork::DebianDependencyNetwork(const string& file)
{
    readPackageFile(file);
}

DebianNode* DebianDependencyNetwork::getNodeByName(const string& name) const {
  map<string, DebianNode*>::const_iterator i = name_node_map.find(name);
  if (i != name_node_map.end() ) {
      return i->second;
  }
  else {
    return 0;
  }
}

double DebianDependencyNetwork::getSizeAssortativity() const {

  //See eq. 25 in cond-mat/0209450
  Network::EdgeSet::const_iterator i = edge_set.begin();
  DirectedEdge* d_e;
  DebianNode *start = 0, *end = 0;
  int j,k;
  double sum_j, sum_k, sum_jk, sum_j2, sum_k2;

  //These are double because for large networks, they will be more than 2^32
  sum_j = 0.0;
  sum_k = 0.0;
  sum_jk = 0.0;
  sum_j2 = 0.0;
  sum_k2 = 0.0;
  for(; i != edge_set.end(); i++) {
    //We need "remaining degree" for this calculation
    d_e = dynamic_cast<DirectedEdge*>(*i);
    //Make sure that j has the start node:
    if( d_e->pointsFirstToSecond() ) {
	//Here we use fancy C++ features:
	start = dynamic_cast<DebianNode*>(d_e->first);
	end = dynamic_cast<DebianNode*>(d_e->second);
    }
    else {
	start = dynamic_cast<DebianNode*>(d_e->second);
	end = dynamic_cast<DebianNode*>(d_e->first);
    }
    j = start->getSize();
    k = end->getSize();   
    sum_j += (double)j;
    sum_k += (double)k;
    sum_jk += (double)(j * k);
    sum_j2 += (double)(j * j);
    sum_k2 += (double)(k * k);
  }
  double r, m_inv = 1.0 / (double)(edge_set.size());

  /**
   *
   * r = \frac{(\sum_i j_i k_i) - M^{-1}(\sum_i j_i)(\sum_i k_i)}
   *          {\sqrt{[(\sum_i j_i^2) - M^{-1}(\sum_i j_i)^2]
   *                    [(\sum_i k_i^2) - M^{-1}(\sum_i k_i)^2]}
   * 
   */

  r = ((double)sum_jk - m_inv * (double)(sum_j * sum_k))
	  / sqrt( ((double)sum_j2 - m_inv * (double)(sum_j * sum_j)) *
	          ((double)sum_k2 - m_inv * (double)(sum_k * sum_k)) );
  return r;

}

double DebianDependencyNetwork::getLibAssortativity() const {

  //See eq. 25 in cond-mat/0209450
  Network::EdgeSet::const_iterator i;
  DebianNode *start = 0, *end = 0;
  DirectedEdge* d_e;
  
  map< pair<bool, bool>, int> e_ij;
  map< pair<bool,bool>,int >::iterator e_it;
  vector<int> a_i, b_i;
  
  bool start_lib, end_lib;
  
  for(i = edge_set.begin(); i != edge_set.end(); i++) {
    //We need "remaining degree" for this calculation
    d_e = dynamic_cast<DirectedEdge*>(*i);
    //Make sure that j has the start node:
    if( d_e->pointsFirstToSecond() ) {
	start = dynamic_cast<DebianNode*>(d_e->first);
	end = dynamic_cast<DebianNode*>(d_e->second);
    }
    else {
	start = dynamic_cast<DebianNode*>(d_e->second);
	end = dynamic_cast<DebianNode*>(d_e->first);
    }
    start_lib = start->isLib();
    end_lib = end->isLib();
    e_it = e_ij.find( pair<bool,bool>(start_lib,end_lib) );
    if( e_it != e_ij.end() ) {
        e_it->second = e_it->second + 1;
    }
    else {
        e_ij[ pair<bool,bool>(start_lib,end_lib) ] = 1;
    }
  }
  a_i.reserve(2);
  b_i.reserve(2);
  a_i[0] = e_ij[ pair<bool,bool>(false,false) ] + e_ij[ pair<bool, bool>(false,true) ];
  a_i[1] = e_ij[ pair<bool,bool>(true,false) ] + e_ij[ pair<bool, bool>(true,true) ];
  b_i[0] = e_ij[ pair<bool,bool>(false,false) ] + e_ij[ pair<bool, bool>(true,false) ];
  b_i[1] = e_ij[ pair<bool,bool>(false,true) ] + e_ij[ pair<bool, bool>(true,true) ];
  
  double sum_eii = (double)(e_ij[ pair<bool,bool>(false,false) ]
		            + e_ij[ pair<bool, bool>(true,true) ]);
  double sum_aibi = (double)a_i[0] * (double) b_i[0]
	                + (double)a_i[1] * (double) b_i[1];
  
  double r, m_inv = 1.0 / (double)(edge_set.size());

  /**
   *
   * r = \frac{(\sum_i j_i k_i) - M^{-1}(\sum_i j_i)(\sum_i k_i)}
   *          {\sqrt{[(\sum_i j_i^2) - M^{-1}(\sum_i j_i)^2]
   *                    [(\sum_i k_i^2) - M^{-1}(\sum_i k_i)^2]}
   * 
   */

  r = (m_inv * sum_eii - m_inv * m_inv * sum_aibi) /
	        ( 1 - m_inv * m_inv * sum_aibi);
  return r;

}

vector<SuperString>
DebianDependencyNetwork::splitDependencies(const SuperString& depstring) {
	      
    vector<SuperString> return_vec, tmp_vec1, tmp_vec2, deps = depstring.split(", ");
    string::size_type pos=0;
    
    for(unsigned int i = 0; i < deps.size(); i++) {
        //Find the first " " or "("
	pos = deps[i].find(" ");
	if( deps[i].find("(") < pos ) {
            pos = deps[i].find("(");
        }
	//There can still be "|" and "," separations.
	tmp_vec1 = SuperString( deps[i].substr(0,pos) ).split("|");
	for(unsigned int j = 0; j < tmp_vec1.size(); j++) {
	    tmp_vec2 = tmp_vec1[j].split(",");
	    for(unsigned int k = 0; k < tmp_vec2.size(); k++) {
		return_vec.push_back( tmp_vec2[k] );
	    }
        }
    }
    return return_vec;
}

void DebianDependencyNetwork::readPackageFile(const string& filename) {

    ifstream package_file( filename.c_str() );

    SuperString line, depends, tmp; //Starsky::SuperString, has some more features...
    string package, version, tmp_field;
    int size = 0;
    DebianNode* this_node = 0;
    
    vector<SuperString> split_string, provides;
    
    /**
     * Unfortunately, this is pretty ugly.  We make two passes, first
     * we get the nodes and create memory for them, and store the pointers
     * to them.  Then, we read the file again and lookup the pointers by
     * name, and record the edges of the networks.
     */
    
    //Here we make all the nodes:
    while(getline(package_file, line, '\n')) {
	if( line.size() > 0) {
          split_string = line.split(": ");
	  if( split_string[0] == "Package") {
              package = split_string[1];
	  } else if( split_string[0] == "Version" ) {
              version = split_string[1];
	  } else if( split_string[0] == "Size") {
              size = atoi( split_string[1].c_str() );
	  } else if( split_string[0] == "Provides") {
             //Provides are aliases to one of many packages.
	     //if a package "provides" something, then any
	     //or packages that depends on the "provides" is
	     //satisfied by this package.
             provides = split_string[1].split(", ");
	     for(unsigned int i = 0; i < provides.size(); i++) {
                 this_node = new DebianAliasNode( provides[i] );
		 add( this_node );
		 name_node_map[ provides[i] ] = this_node;
	     }
	  }
	}
	else {
        //This is a package separator line, so it is time to make the node
	    this_node = new DebianPackageNode(package, version, size);
	    add( this_node );
	    name_node_map[ package ] = this_node;
	}
    }
    package_file.close();


    /*
     * So we have all the nodes, now we open the file again and
     * record the edges.
     */
    
    package_file.open( filename.c_str() );
    vector<SuperString> deps; 
    Node* end_node;
    
    while(getline(package_file, line, '\n')) {
	split_string = line.split(": ");
	if( split_string[0] != line) {
	  if( split_string[0] == "Package") {
              package = split_string[1];
	      this_node = getNodeByName( package );
	  } else if( split_string[0] == "Depends" ) {
              deps = splitDependencies( split_string[1] );
              for(unsigned int i = 0; i < deps.size(); i++) {
                  end_node = getNodeByName( deps[i] );
		  if( end_node != 0) {
		      add(DirectedEdge(this_node, end_node));
		      //cout << package << " " << deps[i] << endl;
	              //There are some dependencies that are
		      //unmet in debian, we will ignore these
	              //nodes
		  }
              }
          }
	}
    }
    package_file.close();
}


