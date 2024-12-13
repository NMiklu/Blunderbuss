#ifndef ENGINE_H
#define ENGINE_H

#include "position.h"
#include "representation.h"
#include <vector>


class Engine {

public:
  Engine(); // TODO
  ~Engine(); // TODO

  bool square_is_edge( enum Square sq );
  bool square_is_corner( enum Square sq );

protected:
  Position* pos;
  std::vector<Position*> previous_positions;

  Piece    engine_color();
  Piece    opponent_color();
  Bitboard engine_pieces();
  Bitboard opponent_pieces();


  void propagate_compass_on_bitboard( Bitboard& bb, Square initial_sq, Compass direction);
  Bitboard generate_bishop_move_pattern_bitboard( enum Square sq );
  Bitboard generate_rook_move_pattern_bitboard( enum Square sq );
  Bitboard generate_knight_move_pattern_bitboard( enum Square sq);
  Bitboard generate_king_move_pattern_bitboard( enum Square sq ); 
  Bitboard generate_queen_move_pattern_bitboard( enum Square sq );
  Bitboard generate_pawn_move_pattern_bitboard( enum Square sq );

  std::vector<Square> bitboard_to_squares(Bitboard bb);
  std::vector<Move> generate_pseudo_legal_special_moves(); // Castle, Promotion, En passant
  std::vector<Move> generate_pseudo_legal_moves(enum Piece p, enum Square sq );

  bool legal_move( Move m );
  std::vector<Move> Engine::generate_legal_moves();
  


};


#endif