#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <string>
#include <vector>
#include "representation.h"

class Position {
  public:
    Position();
    ~Position() = default;

    static const uint8_t MAX_HALF_MOVE = 50;
    static const uint8_t SQUARE_LIMIT = 64;

    static Position* copy(const Position& pos); 
    static Color color(Piece p);    
    static PieceType type(Piece p);
    static Piece to_piece(Color c, PieceType t);
    static Square to_square(Bitboard single_pop_bb);
    static bool move_direction_before_edge(Square sq, Compass dir);
    static bool move_direction_before_edge(Bitboard sq, Compass dir);

    Bitboard pieces(Piece x) const;
    Bitboard pieces(PieceType x) const;
    Bitboard pieces(Color x) const; 
    Color to_attack() const;
    Color to_defend() const;
    Square en_passant_target() const; 
    Piece piece_on( Square sq ) const;
    bool is_attacked(Square sq) const;
    void pretty(std::ostream& os) const;

    bool fen(std::string fen_string); // TODO
    void reset();
    bool put( Square sq, Piece p ); 
    void remove(Square sq);

    void add_castle_right(CastleRight right);
    bool has_castle_right(CastleRight right) const;
    void revoke_castle_right(CastleRight right);
    void inc_halfmove(); 
    void inc_fullmove(); 
    uint8_t halfmove() const;
    uint16_t fullmove() const;

    void make_move(const Move& m);
    std::vector<Move> legal_moves( Square sq ) const;
    std::vector<Move> legal_moves() const;

  private:

    Bitboard pseudo_legal_direction_bitboard(Square sq, Compass dir, bool propogate) const;
    std::vector<Square> pseudo_legal_direction_squares(Square sq, Compass dir, bool propagate) const;

    std::vector<Move> pseudo_legal_normal_moves(Square sq) const; // Simple From-To moves
    std::vector<Move> pseudo_legal_pawn_moves(Square sq) const; // Non-promote pawn moves

    // Special Moves
    std::vector<Move> pseudo_legal_promo_moves(Square sq) const;
    std::vector<Move> pseudo_legal_ep_moves(Square sq) const;
    std::vector<Move> pseudo_legal_castle_moves() const;

    static bool pseudo_legal_move_is_legal(const Position& pos, const Move& m);


    bool _VALID_REP() const;

    Bitboard          colorBB[COLOR_BOUND] = {0ULL};
    Bitboard          pieceTypeBB[PIECE_TYPE_BOUND] = {0ULL};
    Piece             pieceBySquare[SQUARE_LIMIT];
    Square            en_passant_target_square = NO_SQUARE;
    Color             side_to_move = WHITE;
    uint8_t           castleRightMask = (WHITE_CASTLE | BLACK_CASTLE);
    uint8_t           half_move_clock = 0;
    uint16_t          full_move_clock = 1; // Always start on the first move :O

  #ifdef POSITION_TEST
    friend class PositionTests;
  #endif
};


#endif
