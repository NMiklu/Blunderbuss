#ifndef MISC_H
#define MISC_H

#include <sstream>
#include <vector>
#include <string>

namespace Utility {

std::vector<std::string> tokenize( const std::string& str, const char& delim );
bool find_char_in_str( const char& ch, const std::string& table );
unsigned long int  index_char_in_str( const char& ch, const std::string& table );
} // namespace Utility


#endif //MISC_H