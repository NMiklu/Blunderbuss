#include <iostream>
#include <cmath>
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
uint8_t Utility::str_to_decimal_uint8(std::string value) {
  /*
    Returns decimal provided by <value>
      -> <value> must start with numerical characters
    If no number is found, 0 is returned
  */
  const std::string digits = "0123456789";
  if( value[0] == '0' || (!Utility::find_char_in_str(value[0],digits)))\
    return 0;
  uint8_t rv = 0;
  for( size_t i = 0; i < value.size(); i++ ) {
    if( !(value[i] >= '0' && value[i] <= '9') )
      return rv;
    double power  = (double)(value.size() - i - 1);
    uint8_t digit = (uint8_t)(value[i] - 48);
    uint8_t place = (uint8_t)std::pow(10.0,power);
    rv += (digit*place);
  }
  return rv;
}