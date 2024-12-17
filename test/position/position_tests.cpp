#include <iostream>
#include <cassert>
#include "../../src/position.h"
#include "../../src/representation.h"
#include "../tools/bitboard_debug.h"

const Bitboard rank_bbs[8] = {RANK_1_BB, RANK_2_BB, RANK_3_BB, RANK_4_BB,
                              RANK_5_BB, RANK_6_BB, RANK_7_BB, RANK_8_BB};
const Bitboard file_bbs[8] = {FILE_A_BB, FILE_B_BB, FILE_C_BB, FILE_D_BB,
                              FILE_E_BB, FILE_F_BB, FILE_G_BB, FILE_H_BB};
const PieceType piece_types[6] = {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
const Color     piece_colors[2] = {WHITE, BLACK};


class PositionTests : Position{
public:
  /* UNIT TESTS */
  void position_copy_test() {}
  void position_color_test() {
    Piece w_ps[6] = {W_PAWN,W_KNIGHT,W_BISHOP,W_ROOK,W_QUEEN,W_KING};
    Piece b_ps[6] = {B_PAWN,B_KNIGHT,B_BISHOP,B_ROOK,B_QUEEN,B_KING};
    for( int i = 0; i < 6; i++ ) {
      assert(Position::color(w_ps[i]) == WHITE);
      assert(Position::color(b_ps[i]) == BLACK);
    }
    assert(Position::color(PIECE_BOUND) == NO_COLOR);
    assert(Position::color(NO_PIECE) == NO_COLOR);
  }
  void position_type_test() {
    assert(Position::type(W_PAWN) == PAWN);
    assert(Position::type(B_PAWN) == PAWN);
    assert(Position::type(W_KNIGHT) == KNIGHT);
    assert(Position::type(B_KNIGHT) == KNIGHT);
    assert(Position::type(W_BISHOP) == BISHOP);
    assert(Position::type(B_BISHOP) == BISHOP);
    assert(Position::type(W_ROOK) == ROOK);
    assert(Position::type(B_ROOK) == ROOK);
    assert(Position::type(W_QUEEN) == QUEEN);
    assert(Position::type(B_QUEEN) == QUEEN);
    assert(Position::type(W_KING) == KING);
    assert(Position::type(B_KING) == KING);
    assert(Position::type(PIECE_BOUND) == NO_TYPE);
    assert(Position::type(NO_PIECE) == NO_TYPE);
  }
  void position_pieces_Piece_test() {
    for( int t = 0; t < PIECE_BOUND; t++) {
      Piece phi = static_cast<Piece>(t);
      Color c = (t<6) ? WHITE:BLACK;
      PieceType tau = static_cast<PieceType>(t%6);
      for( int i = 0; i < 64; i++ ) {
        Square sq = static_cast<Square>(i);
        int rank = i / 8; // Truncates
        int file = i % 8;
        Bitboard sq_bb = SQUARE_TO_BB(sq);
        Position* p = new Position();
        p->pieceBySquare[sq] = phi;
        p->pieceTypeBB[tau] |= sq_bb;
        p->colorBB[c] |= sq_bb;
        Bitboard piece_retrieved = p->pieces(phi);
        Bitboard piece_check = rank_bbs[rank] & file_bbs[file];
        assert( (piece_retrieved & piece_check) != 0);
        delete p;
      }
    }
    Position* p = new Position();
    assert(p->pieces(PIECE_BOUND) == EMPTY_BB);
    assert(p->pieces(NO_PIECE) == EMPTY_BB);
    delete p;
  }
  void position_pieces_PieceType_test() {}
  void position_piece_color_test(){}

  void position_en_passant_target_test() {}

  void position_piece_on_test() {}
  void position_pretty_test() {}
  void position_fen_test() {}
  void position_reset_test() {}
  void position_put_test() {}
  void position_remove_test() {}
  void position_add_castle_right_test() {}
  void position_has_castle_right_test() {}
  void position_revoke_castle_right_test() {}
  void position_inc_havemove_test() {}
  void position_inc_fullmove_test() {}
  void position_halfmove_test() {}
  void position_fullmove_test() {}
};


int main( int argc, char** argv ) {
  PositionTests t;
  t.position_color_test();
  t.position_type_test();
  t.position_pieces_Piece_test();
  return 0;
}
