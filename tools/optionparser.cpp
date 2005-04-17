#include "optionparser.h"

using namespace std;
using namespace Starsky;

map<string, string> OptionParser::getOpts(int argc, char* argv[])
{
  vector<string> empty;
  return getOpts(argc, argv, empty);
}

map<string, string> OptionParser::getOpts(int argc, char* argv[],
		                          vector<string>& pos_keys)
{
  map<string, string> result;
  int pos = 0;
  for(int i = 1; i < argc; i++) {
    if( argv[i][0] == '-' && argv[i][1] == '-' ) {
      //This is an option
      char* this_arg = argv[i] + 2;
      //Skip the --
      string arg = string(this_arg);
      //Find the '='
      int eqpos = arg.find('=');
      if( eqpos != string::npos ) {
        string key = arg.substr(0, eqpos);
	string value = arg.substr(eqpos + 1, string::npos);
	result[key] = value;
	cout << key << " -> " << value << endl;
      }
      else {
        result[arg] = "";
      }
    }
    else {
    //This is a positional option;
      if( pos < pos_keys.size() ) {
        result[ pos_keys[pos] ] = argv[i];
        pos++;
      }
      else {
        //We are loosing this option:
	cerr << "Unknown option: " << argv[i] << endl;
	throw exception();
      }
    }
  }
  return result;
}
