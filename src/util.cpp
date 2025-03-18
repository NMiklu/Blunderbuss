#include "util.h"
#include <vector>
#include <string>


std::vector<std::string> util::tokenize( std::string str, char delimiter ) {
  std::vector<std::string> tokens;
  if( str.size() == 0 ) return tokens;

  unsigned long int i;
  std::string tau = "";
  for( i = 0 ; i < str.size(); i++ ) {
    if( str[i] == delimiter ) {
      tau += str[i];
    } else {
      tokens.push_back( tau );
      tau = "";
    }
  }
  tokens.push_back( tau );

  return tokens;
}
