#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <string>
#include "representation.h"

struct Position {

    Position()  = default;
    ~Position() = default;


    static const uint8_t MAX_HALF_MOVE = 50;
    static Position* copy(const Position& pos);

    Bitboard pieces(Piece x) const;
    Square en_passant_target() const;
    Piece piece_at_square( Square sq );

    void pretty(std::ostream& os) const;
    void set( Square sq, Piece piece_p );
    void remove(Square sq);
    void clear();
    bool castle(uint8_t bits);
    bool revoke_castle(Piece color);
    bool halfmove();
    bool fullmove();
    bool fen(std::string fen_string);
    bool check_rep() const;


    //TODO Piece/Square cache
    Bitboard          pieceBB[8] = {0ULL};
    enum Square       en_passant_target_square = NO_SQUARE;
    enum Piece        side_to_move = white_p;
    uint8_t           castle_ability = (white_long_castle | white_short_castle | black_long_castle | black_short_castle);
    uint8_t           half_move_clock = 0;   
    uint8_t           full_move_counter = 1; 
};

#endif
