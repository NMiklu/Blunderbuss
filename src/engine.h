#ifndef ENGINE_H
#define ENGINE_H

#include "position.h"
#include "representation.h"
#include <vector>


class Engine {

public:
  Engine(); // TODO
  ~Engine(); // TODO


protected:

  Piece    engine_color(const Position& pos);
  Piece    opponent_color(const Position& pos);
  Bitboard engine_pieces(const Position& pos);
  Bitboard opponent_pieces(const Position& pos);


  /* Bitboard Manip */
  Square single_pop_bitboard_to_square( Bitboard one_pop_bb);
  std::vector<Square> bitboard_to_squares(Bitboard bb);
  void propagate_compass_on_position(const Position& pos, Bitboard& bb, Square initial_sq, Compass direction);

  /* Move Generation */
  Bitboard pseudo_legal_bishop_move_bitboard( const Position& pos, Square sq );
  Bitboard pseudo_legal_rook_move_bitboard( const Position& pos, Square sq );
  Bitboard pseudo_legal_knight_move_bitboard( const Position& pos, Square sq );
  Bitboard pseudo_legal_king_move_bitboard( const Position& pos, Square sq );
  Bitboard pseudo_legal_queen_move_bitboard( const Position& pos, Square sq );
  Bitboard pseudo_legal_pawn_move_bitboard( const Position& pos, Square sq );
  std::vector<Move> generate_pseudo_legal_special_moves(const Position& pos); // Castle, Promotion, En passant
  std::vector<Move> generate_pseudo_legal_moves(const Position& pos, Piece p, Square sq );
  std::vector<Move> generate_legal_moves(const Position& pos); 
  bool legal_move( const Position& pos, const Move& m );
  bool check_direction( Square sq, Compass dir );

  /* Position Manip */
  bool square_is_edge( enum Square sq );
  bool square_is_corner( enum Square sq );
  bool apply_move_to_position( Position& pos, const Move& m)
  

};


#endif