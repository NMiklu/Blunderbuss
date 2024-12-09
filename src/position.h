#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <string>
#include "representation.h"

class Position {
  public:
    static const uint8_t MAX_HALF_MOVE = 50;

    Position()  = default;
    ~Position() = default;

    Bitboard pieces(Piece x) const;
    void pretty(std::ostream& os) const;
    void set( Square sq, Piece piece_p );
    bool castle(uint8_t bits);
    bool halfmove();
    bool fullmove();
    bool fen(std::string fen_string);
    void clear();
    bool check_rep() const;

  private:
    Bitboard          pieceBB[8] = {0ULL};
    enum Square       en_passant_target_square = NO_SQUARE;
    enum Piece        side_to_move = white_p;
    uint8_t           castle_ability = (white_long_castle | white_short_castle | black_long_castle | black_short_castle);
    uint8_t           half_move_clock = 0;   
    uint8_t           full_move_counter = 1; 

};

#endif