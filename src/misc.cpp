#include <iostream>
#include "misc.h"

std::vector<std::string> Utility::tokenize( const std::string& str, const char& delim ) {
  std::vector<std::string> tokens;
  std::stringstream ss(str);
  std::string intermidiate;
  while( getline(ss, intermidiate, delim) ) {
    tokens.push_back(intermidiate);
  }
  return tokens;
}
bool Utility::find_char_in_str( const char& ch, const std::string& table ) {
  /*
    returns true if an instance of <ch> is found in <table> 
    returns false otherwise
  */
  for( unsigned long int i = 0; i < table.size(); i++ ) {
    if( table[i] == ch ) return true;
  }
  return false;
}
unsigned long int Utility::index_char_in_str( const char& ch, const std::string& table ) {
  /*
    returns index of <ch> found in <table>
    returns -1 if not found.
  */
  unsigned long int idx = -1;
  for( unsigned long int i = 0; i < table.size(); i++ ) {
    if( table[i] == ch ) {
      idx = i;
      break;
    }
  }
  return idx;
}