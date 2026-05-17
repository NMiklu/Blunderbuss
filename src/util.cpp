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

bool util::is_numeric( char c ) {
  unsigned short cast = static_cast<unsigned short>(c);
  const int ascii_numerical_min = 48;
  const int ascii_numerical_max = 57;
  if (cast < ascii_numerical_min || cast > ascii_numerical_max ) return false;
  return true;
}
bool util::is_alphabetical( char c ) {
  unsigned short cast = static_cast<unsigned short>(c);

  const unsigned short upper_case_min_cast = 0x41;
  const unsigned short upper_case_max_cast = 0x5A;
  const unsigned short lower_case_min_cast = 0x61;
  const unsigned short lower_case_max_cast = 0x7A;

  if( cast < upper_case_min_cast ) return false;
  if( cast <= upper_case_max_cast ) return true;
  if( cast < lower_case_min_cast ) return false;
  if( cast <= lower_case_max_cast ) return true;
  return false;
}
