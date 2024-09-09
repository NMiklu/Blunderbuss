#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <string>
#include "representation.h"

class Position {
  private:
    Bitboard  pieceBB[8];
    uint8_t   castle;

  public:
    enum PieceEnum {
      white_p,
      black_p,
      pawn_p,
      knight_p,
      bishop_p,
      rook_p,
      queen_p,
      king_p
    };
    enum CastleEnum {
      white_short_castle  = 1,
      white_long_castle   = 2,
      black_short_castle  = 4,
      black_long_castle   = 8
    };

    Position()  = default;
    ~Position() = default;

    Bitboard pieces(PieceEnum x) const;
    void pretty(std::ostream& os) const;
    bool insert( LERF_Square sq, PieceEnum color, PieceEnum piece );
    bool move( LERF_Square from, LERF_Square to );
    bool valid_position() const;
    bool fen(std::string fen_string);
    void clear();

};

#endif