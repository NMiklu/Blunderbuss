#ifndef POSITION_TEST
#define POSITION_TEST

#include "../../src/position.h"
#include "../../src/representation.h"
#include <iostream>

class PositionTests {
public:
  PositionTests() = delete;
  ~PositionTests() = delete;
  /* Tool Functions */
  static void position_info_pretty(Position* p);
  static void mock_put( Position* pos , Square sq, Piece p );
  static void mock_remove( Position* pos, Square sq );
  static Color mock_color(Piece p);
  static PieceType mock_type(Piece p);

  /* Test Positions */
  static Position* starting_position();
  static Position* kings_position();

  /* Categorical Tests */
  static void all_tests();
  static void translation_tests();
  static void retrieval_tests();
  static void position_manip_tests();
  static void move_generation_tests();

  /* Translation Tests */
  static void to_square_test();
  static void to_piece_test();
  static void color_test();
  static void type_test();

  /* Retrieval Tests */
  static void pieces_Piece_test();
  static void pieces_PieceType_test();
  static void pieces_Color_test();
  static void en_passant_target_test();
  static void piece_on_test();
  static void has_castle_right_test();
  static void halfmove_test();
  static void fullmove_test();
  static void to_attack_test();
  static void to_defend_test();
  static void copy_test();

  /* Position Manip Tests */
  static void reset_test();
  static void put_test();
  static void remove_test();
  static void add_castle_right_test();
  static void revoke_castle_right_test();
  static void inc_halfmove_test();
  static void inc_fullmove_test();
  static void fen_test(); // TODO

  /* Move Generation Tests*/
  static void move_direction_before_edge_sq_test();
  static void move_direction_before_edge_bb_test();
  static void make_move_test();
  static void pseudo_legal_pawn_moves_test();
  static void pseudo_legal_direction_squares_test();
  static void pseudo_legal_castle_moves_test();


};


#endif
