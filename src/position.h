#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <string>
#include "representation.h"

class Position {
  public:
    Position()  = default;
    ~Position() = default;

    static const uint8_t MAX_HALF_MOVE = 50;
    static Position* copy(const Position& pos); 
    static Color color(Piece p);    
    static PieceType type(Piece p);

    Bitboard pieces(Piece x) const;
    Bitboard pieces(PieceType x) const;
    Bitboard pieces(Color x) const; 
    Square en_passant_target() const; 
    Piece piece_on( Square sq ) const;
    void pretty(std::ostream& os) const; 

    bool fen(std::string fen_string); // TODO
    void reset();
    bool put( Square sq, Piece p ); 
    void remove(Square sq); 
    void add_castle_right(CastleRight right);
    void has_castle_right(CastleRight right);
    void revoke_castle_right(CastleRight right);
    void inc_halfmove(); 
    void inc_fullmove(); 
    uint8_t halfmove() const;
    uint16_t fullmove() const;
    


  private:
    bool _VALID_REP() const; 

    Bitboard          colorBB[COLOR_BOUND] = {0ULL};
    Bitboard          pieceTypeBB[PIECE_TYPE_BOUND] = {0ULL};
    Piece             pieceBySquare[64] = {NO_PIECE};
    Square            en_passant_target_square = NO_SQUARE;
    Color             side_to_move = WHITE;
    uint8_t           castleRightMask = (WHITE_SHORT_CASTLE | WHITE_LONG_CASTLE | BLACK_SHORT_CASTLE | BLACK_LONG_CASTLE );
    uint8_t           half_move_clock = 0;
    uint16_t          full_move_clock = 1; 
};

#endif
