#ifndef BB_DEBUG_H
#define BB_DEBUG_H

#include "../../src/representation.h"
#include <iostream>

class Bitboard_Debug {
public:
  static void pretty(std::ostream& os, Bitboard bb);
  static bool square_filled( Bitboard bb, Square sq);
};

#endif