#include "gnutellanetwork.h"

using namespace Starsky;
using namespace std;

GnutellaNetwork::GnutellaNetwork(const string& file,
		                 const string& type) : Network() {
    if( type == "limecrawler" ) {
        readCrawlerFile(file);
    }
    else if( type == "ripeanu" ) {
        readRipeanuFile(file);
    }
}

void GnutellaNetwork::readCrawlerFile( const string& file) {

    ifstream crawler_file( file.c_str() );

    GnutellaNode *a, *b;
    SuperString line, add_a, add_b;
    vector<SuperString> tmp, foo;
    
    while(getline(crawler_file, line, '\n')) {
        /*
	 * There are two cases:
	 * 1) a node definition: address + " : " + attributes
	 * 2) an edge definition: address + " -> " + address
	 */

        tmp = line.split(" -> ");
	if( tmp.size() > 1) {
          //There was a " -> "
	  add_a = tmp[0];
	  foo = tmp[1].split(" ");
	  add_b = foo[0];
	  a = getNodeByAddress(add_a);
	  b = getNodeByAddress(add_b);
	  if(a == 0) {
            //This is a new node
	    a = new GnutellaNode(add_a);
	    _add_node_map[add_a] = a;
	  }
	  if(b == 0) {
            //This is a new node
	    b = new GnutellaNode(add_b);
	    _add_node_map[add_b] = b;
	  }
	  //The files sometimes have loops, we ignore loops:
	  if( a != b) {
	    add(Edge(a,b));
	    //cout << "added: " << add_a << " to " << add_b << endl;
	  }
	}
    }
}

void GnutellaNetwork::readRipeanuFile(const string& file) {
    ifstream ripeanu_file( file.c_str() );

    GnutellaNode *a = 0, *b = 0;
    string this_read;
    string::iterator s_it;
    char type;
    while( ripeanu_file >> this_read ) {
	type = this_read[0];
	s_it = this_read.begin();
	this_read.erase(s_it);
        //cout << this_read << endl;
	if( type == '&' ) {
	    a = getNodeByAddress(this_read);
	    if( a == 0) {
                a = new GnutellaNode(this_read);
	        add(a);
	        _add_node_map[this_read] = a;
	    }
	}
	else if( type == '*' ) {
            b = getNodeByAddress(this_read);
	    if( b == 0) {
                //This is a new node:
		b = new GnutellaNode(this_read);
		add(b);
		_add_node_map[this_read] = b;
	    }
	    if( a != b) {
	      //These graphs sometimes have loops, we remove them
	      add( Edge(a,b) );
	    }
	}
	
    }
}

GnutellaNode* GnutellaNetwork::getNodeByAddress(const string& add) const {
  map<string, GnutellaNode*>::const_iterator i = _add_node_map.find(add);
  if( i != _add_node_map.end() ) {
    return i->second;
  }
  
  return 0;
}
