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


class PositionTests {
public:

  static void unit_tests() {
    PositionTests::position_color_test();
    PositionTests::position_type_test();
    PositionTests::position_pieces_Piece_test();
    PositionTests::position_pieces_PieceType_test();
    PositionTests::position_pieces_Color_test();
    PositionTests::position_en_passant_target_test();
    PositionTests::position_piece_on_test();
    PositionTests::position_reset_test();
    PositionTests::position_put_test();
  }
  /* UNIT TESTS */
  static void position_copy_test() {}
  static void position_color_test() {
    Piece w_ps[6] = {W_PAWN,W_KNIGHT,W_BISHOP,W_ROOK,W_QUEEN,W_KING};
    Piece b_ps[6] = {B_PAWN,B_KNIGHT,B_BISHOP,B_ROOK,B_QUEEN,B_KING};
    for( int i = 0; i < 6; i++ ) {
      assert(Position::color(w_ps[i]) == WHITE);
      assert(Position::color(b_ps[i]) == BLACK);
    }
    assert(Position::color(PIECE_BOUND) == NO_COLOR);
    assert(Position::color(NO_PIECE) == NO_COLOR);
  }
  static void position_type_test() {
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
  static void position_pieces_Piece_test() {
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
  static void position_pieces_PieceType_test() {
    for( int pie = 0; pie < PIECE_BOUND; pie++) {
      Piece piece_ = static_cast<Piece>(pie);
      PieceType t = static_cast<PieceType>(pie%6);
      Color c = (pie>6) ? BLACK:WHITE;
      for( int i = 0; i < 64; i++ ) {
        Position* p = new Position();
        Square sq = static_cast<Square>(i);
        Bitboard sq_bb = SQUARE_TO_BB(sq);
        p->pieceBySquare[sq] = piece_;
        p->pieceTypeBB[t] |= sq_bb;
        p->colorBB[c] |= sq_bb;
        Bitboard piece_retrieved = p->pieces(t);
        Bitboard piece_check = rank_bbs[i/8] & file_bbs[i%8];
        assert(piece_retrieved == piece_check);
        delete p;
      }
    }
    Position* p = new Position();
    p->pieceTypeBB[PAWN] |= 0xFFFFFFFFFFFFFFFF;
    p->pieceTypeBB[KNIGHT] |= 0xFFFFFFFFFFFFFFFF;
    p->pieceTypeBB[ROOK] |= 0xFFFFFFFFFFFFFFFF;
    p->pieceTypeBB[BISHOP] |= 0xFFFFFFFFFFFFFFFF;
    p->pieceTypeBB[QUEEN] |= 0xFFFFFFFFFFFFFFFF;
    p->pieceTypeBB[KING] |= 0xFFFFFFFFFFFFFFFF;
    assert(p->pieces(PIECE_BOUND) == EMPTY_BB);
    assert(p->pieces(NO_PIECE) == EMPTY_BB);
    delete p;
  }
  static void position_pieces_Color_test(){
    for( int tau = 0; tau < PIECE_BOUND; tau++ ) {
      Piece pie = static_cast<Piece>(tau);
      PieceType ty = static_cast<PieceType>(tau%6);
      Color c = (tau > 6) ? BLACK:WHITE;
      for( int i = 0; i < 64; i++) {
        Position* p = new Position();
        Square sq = static_cast<Square>(i);
        Bitboard sq_bb = SQUARE_TO_BB(sq);
        p->pieceBySquare[sq] = pie;
        p->pieceTypeBB[ty] |= sq_bb;
        p->colorBB[c] |= sq_bb;
        Bitboard piece_retrieved = p->pieces(c);
        Bitboard piece_check = rank_bbs[i/8] & file_bbs[i%8];
        assert(piece_retrieved == piece_check);
        delete p;
      }
    }
    Position* p = new Position();
    p->colorBB[WHITE] |= 0xFFFFFFFF00000000;
    p->colorBB[BLACK] |= 0x00000000FFFFFFFF;
    assert(p->pieces(NO_COLOR) == EMPTY_BB);
    assert(p->pieces(COLOR_BOUND) == EMPTY_BB);
    delete p;
  }

  static void position_en_passant_target_test() {
    Position* p = new Position();
    for( int i = 0; i < 64; i++ ) {
      Square sq = static_cast<Square>(i);
      p->en_passant_target_square = sq;
      assert(p->en_passant_target() == sq);
    }
    p->en_passant_target_square = NO_SQUARE;
    assert(p->en_passant_target() == NO_SQUARE);
  }

  static void position_piece_on_test() {
    for( int pie = 0; pie < PIECE_BOUND; pie++ ) {
      Position* p = new Position();
      Piece piece_ = static_cast<Piece>(pie);
      for( int i = 0; i < 64; i++) {
        Square sq = static_cast<Square>(i);
        p->pieceBySquare[sq] = piece_;
        assert(p->piece_on(sq) == piece_);
      }
      delete p;
    }
    Position* p = new Position();
    for( int i = 0; i < 64; i++) {
      Square sq = static_cast<Square>(i);
      assert(p->piece_on(sq) == NO_PIECE);
    }
    delete p;
  }
  static void position_fen_test() {} // TODO
  static void position_reset_test() {
    Bitboard white_bb = RANK_1_BB | RANK_2_BB;
    Bitboard black_bb = RANK_7_BB | RANK_8_BB;
    Bitboard pawn_bb  = RANK_2_BB | RANK_7_BB;
    Bitboard rook_bb  = CORNER_BB;
    Bitboard bish_bb  = (RANK_1_BB | RANK_8_BB) & (FILE_C_BB | FILE_F_BB);
    Bitboard knight_bb= (RANK_1_BB | RANK_8_BB) & (FILE_B_BB | FILE_G_BB);
    Bitboard king_bb  = (RANK_1_BB | RANK_8_BB) & (FILE_E_BB);
    Bitboard queen_bb = (RANK_1_BB | RANK_8_BB) & (FILE_D_BB);
    Position* p = new Position();
    p->reset();
    assert(p->colorBB[WHITE] == white_bb);
    assert(p->colorBB[BLACK] == black_bb);
    assert(p->pieceTypeBB[PAWN] == pawn_bb);
    assert(p->pieceTypeBB[ROOK] == rook_bb);
    assert(p->pieceTypeBB[BISHOP] == bish_bb);
    assert(p->pieceTypeBB[KNIGHT] == knight_bb);
    assert(p->pieceTypeBB[KING] == king_bb);
    assert(p->pieceTypeBB[QUEEN] == queen_bb);
    Square white_pawn_squares[8] = {a2,b2,c2,d2,e2,f2,g2,h2};
    Square black_pawn_squares[8] = {a7,b7,c7,d7,e7,f7,g7,h7};
    for( int i = 0; i < 8; i++ ) {
      assert(p->pieceBySquare[white_pawn_squares[i]] == W_PAWN);
      assert(p->pieceBySquare[black_pawn_squares[i]] == B_PAWN);
    }
    assert(p->pieceBySquare[a1] == W_ROOK);
    assert(p->pieceBySquare[h1] == W_ROOK);
    assert(p->pieceBySquare[a8] == B_ROOK);
    assert(p->pieceBySquare[h8] == B_ROOK);

    assert(p->pieceBySquare[c1] == W_BISHOP);
    assert(p->pieceBySquare[f1] == W_BISHOP);
    assert(p->pieceBySquare[c8] == B_BISHOP);
    assert(p->pieceBySquare[f8] == B_BISHOP);

    assert(p->pieceBySquare[b1] == W_KNIGHT);
    assert(p->pieceBySquare[g1] == W_KNIGHT);
    assert(p->pieceBySquare[b8] == B_KNIGHT);
    assert(p->pieceBySquare[g8] == B_KNIGHT);

    assert(p->pieceBySquare[d1] == W_QUEEN);
    assert(p->pieceBySquare[d8] == B_QUEEN);

    assert(p->pieceBySquare[e1] == W_KING);
    assert(p->pieceBySquare[e8] == B_KING);

    assert( p->side_to_move == WHITE );
    assert( p->en_passant_target_square == NO_SQUARE);
    assert( p->castleRightMask == (WHITE_SHORT_CASTLE | WHITE_LONG_CASTLE | BLACK_SHORT_CASTLE | BLACK_LONG_CASTLE));
    assert( p->half_move_clock == 0);
    assert( p->full_move_clock == 0);

    delete p;
  }

  static void position_put_test() {
    for( int i = 0; i < PIECE_BOUND; i++ ) {
      Piece tau = static_cast<Piece>(i);
      PieceType pt = static_cast<PieceType>(i%6);
      Color c = (i<6) ? WHITE:BLACK;
      for( int j = 0; j < Position::SQUARE_LIMIT; j++) {
        Position* p = new Position();
        Square sq = static_cast<Square>(j);
        Bitboard sq_bb = SQUARE_TO_BB(sq);
        assert(p->put(sq, tau));
        assert(p->pieceBySquare[sq] == tau);
        assert(p->colorBB[c] == sq_bb);
        assert(p->pieceTypeBB[pt] == sq_bb);
        delete p;
      }
    }
    Position* p = new Position();
    assert(!p->put(NO_SQUARE,W_KING));
    assert(!p->put(a1,NO_PIECE));
    assert(!p->put(a1,PIECE_BOUND));
    delete p;
  }
  static void position_remove_test() {}
  static void position_add_castle_right_test() {}
  static void position_has_castle_right_test() {}
  static void position_revoke_castle_right_test() {}
  static void position_inc_havemove_test() {}
  static void position_inc_fullmove_test() {}
  static void position_halfmove_test() {}
  static void position_fullmove_test() {}
};


int main( int argc, char** argv ) {
  PositionTests::unit_tests();
  return 0;
}
