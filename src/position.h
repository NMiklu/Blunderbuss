#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <string>
#include "representation.h"

class Position {
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
    bool check_rep() const;

  private:
    Bitboard          pieceBB[8] = {0ULL};
    enum LERF_Square  en_passant_target_square = NO_SQUARE;
    enum PieceEnum    side_to_move             = white_p;
    uint8_t           castle_ability = (white_long_castle & white_short_castle & black_long_castle & black_short_castle);
    uint8_t           half_move_clock = 0;   
    uint8_t           full_move_counter = 1; 

};

#endif