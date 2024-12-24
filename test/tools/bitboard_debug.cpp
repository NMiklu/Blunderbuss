#include "../../src/representation.h"
#include "bitboard_debug.h"
#include <vector>
#include <iostream>


void Bitboard_Debug::pretty(std::ostream& os, Bitboard bb) {
  bool phi[64];
  for(int i = 0; i < 64; i++) {
    Square sq = static_cast<Square>(i);
    if( bb & SQUARE_TO_BB(sq) ) phi[i] = true;
    else phi[i] = false;
  }

  os << "    A   B   C   D   E   F   G   H   " << std::endl;
  os << "  +---+---+---+---+---+---+---+---+" << std::endl;
  for( int i=56; (i<64 && i>=0);i++) {
    if( i%8 == 0) os << (i/8)+1 << " |";
    if( phi[i] ) os << " . |";
    else         os << "   |";
    if( i%8 == 7 ) {
      i-=16;
      os << std::endl;
      os << "  +---+---+---+---+---+---+---+---+" << std::endl;
    }
  }
}
bool Bitboard_Debug::square_filled(Bitboard bb, Square sq) {
  if( SQUARE_TO_BB(sq) & bb ) return true;
  return false;
}

std::vector<Bitboard> Bitboard_Debug::discretize(Bitboard bb) {
  std::vector<Bitboard> vec;
  Bitboard tau = 1ULL;
  for(int i = 0; i < 64; i++) {
    if( tau & bb ) vec.push_back(tau & bb);
    tau = tau << 1;
  }
  return vec;
}

