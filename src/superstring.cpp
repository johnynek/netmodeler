#include "superstring.h"

using namespace Starsky;
using namespace std;

SuperString::SuperString() : string() {
}

SuperString::SuperString(const string& a) : string(a) {
}

vector<SuperString> SuperString::split(const string& delim) const {

    vector<SuperString> return_value;
    string front, back;
    string::size_type last = 0, pos = 0;
    while( (pos < size()) && (pos != string::npos) ) {
        pos = find(delim, last);
	if( pos != string::npos ) {
            return_value.push_back( substr(last, pos - last) );
	}
	else {
            return_value.push_back( substr(last, size() - last) );
	}
	last = pos + delim.size(); //Skip over the delimeter string
    }
    return return_value;
}
