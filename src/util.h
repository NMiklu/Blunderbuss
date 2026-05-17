#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>

namespace util {

  std::vector<std::string> tokenize( std::string str, char delimiter );
  bool is_numeric( char c );
  bool is_alphabetical( char c );

}

#endif
