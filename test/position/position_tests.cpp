#include <iostream>
#include <cassert>
#include "position_tests.h"
#include "../../src/representation.h"
#include "../tools/bitboard_debug.h"

const Bitboard rank_bbs[8] = {RANK_1_BB, RANK_2_BB, RANK_3_BB, RANK_4_BB,
                              RANK_5_BB, RANK_6_BB, RANK_7_BB, RANK_8_BB};
const Bitboard file_bbs[8] = {FILE_A_BB, FILE_B_BB, FILE_C_BB, FILE_D_BB,
                              FILE_E_BB, FILE_F_BB, FILE_G_BB, FILE_H_BB};
const PieceType piece_types[6] = {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
const Color     piece_colors[2] = {WHITE, BLACK};
const Square    rank_file_sqs[8][8] = {
                                        {a1,b1,c1,d1,e1,f1,g1,h1},
                                        {a2,b2,c2,d2,e2,f2,g2,h2},
                                        {a3,b3,c3,d3,e3,f3,g3,h3},
                                        {a4,b4,c4,d4,e4,f4,g4,h4},
                                        {a5,b5,c5,d5,e5,f5,g5,h5},
                                        {a6,b6,c6,d6,e6,f6,g6,h6},
                                        {a7,b7,c7,d7,e7,f7,g7,h7},
                                        {a8,b8,c8,d8,e8,f8,g8,h8}
                                      };

/* Tool Functions */
void PositionTests::position_info_pretty(Position* p) {
  p->pretty(std::cout);
  std::cout << std::endl;
  std::cout << "  | Side to move: " << (int(p->side_to_move) == 0 ? "WHITE":"BLACK") << std::endl;
  std::cout << "  | CastleRights: " << int(p->castleRightMask) << std::endl;
  std::cout << "  | En Passant Square: " << p->en_passant_target_square << std::endl;
  std::cout << "  | half: " << int(p->half_move_clock) << " full: " << int(p->full_move_clock) << std::endl;
  std::cout << "  +--+--+--+--+--+--+--+--+" << std::endl;
}
void PositionTests::mock_put(Position* pos, Square sq, Piece p) {
  /* Inserts Piece <p> into Position <pos> onto Square <sq> */
  if( p == NO_PIECE || p == PIECE_BOUND ) return;
  Color c = PositionTests::mock_color(p);
  PieceType pt = PositionTests::mock_type(p);
  pos->colorBB[c] |= (1ULL << sq);
  pos->pieceTypeBB[pt] |= (1ULL << sq);
  pos->pieceBySquare[sq] = p;
}
void PositionTests::mock_remove(Position* pos, Square sq) {
  Bitboard tau = ~(1ULL << sq);
  pos->colorBB[WHITE] &= tau;
  pos->colorBB[BLACK] &= tau;
  pos->pieceTypeBB[PAWN] &= tau;
  pos->pieceTypeBB[KNIGHT] &= tau;
  pos->pieceTypeBB[BISHOP] &= tau;
  pos->pieceTypeBB[ROOK] &= tau;
  pos->pieceTypeBB[QUEEN] &= tau;
  pos->pieceTypeBB[KING] &= tau;
  pos->pieceBySquare[sq] = NO_PIECE;
}
Color PositionTests::mock_color(Piece p) {
  if( p == NO_PIECE || p == PIECE_BOUND ) return NO_COLOR;
  if( p == W_PAWN || p == W_KNIGHT || p == W_BISHOP || p == W_ROOK || p == W_QUEEN || p == W_KING ) return WHITE;
  return BLACK;
}
PieceType PositionTests::mock_type(Piece p) {
  if( p == W_PAWN || p == B_PAWN ) return PAWN;
  else if( p == W_KNIGHT || p == B_KNIGHT ) return KNIGHT;
  else if( p == W_BISHOP || p == B_BISHOP ) return BISHOP;
  else if( p == W_ROOK || p == B_ROOK ) return ROOK;
  else if( p == W_QUEEN || p == B_QUEEN ) return QUEEN;
  else if( p == W_KING || p == B_KING ) return KING;
  else return NO_TYPE; // p == NO_PIECE || p == PIECE_BOUND
}

/* Test Positions */
Position* PositionTests::starting_position() {
  Position* p = new Position();
  p->side_to_move = WHITE;
  p->castleRightMask = (WHITE_SHORT_CASTLE | WHITE_LONG_CASTLE | BLACK_SHORT_CASTLE | BLACK_LONG_CASTLE);
  p->half_move_clock = 0;
  p->full_move_clock = 1;
  p->en_passant_target_square = NO_SQUARE;

  PositionTests::mock_put(p, a2, W_PAWN);
  PositionTests::mock_put(p, b2, W_PAWN);
  PositionTests::mock_put(p, c2, W_PAWN);
  PositionTests::mock_put(p, d2, W_PAWN);
  PositionTests::mock_put(p, e2, W_PAWN);
  PositionTests::mock_put(p, f2, W_PAWN);
  PositionTests::mock_put(p, g2, W_PAWN);
  PositionTests::mock_put(p, h2, W_PAWN);
  PositionTests::mock_put(p, a1, W_ROOK);
  PositionTests::mock_put(p, b1, W_KNIGHT);
  PositionTests::mock_put(p, c1, W_BISHOP);
  PositionTests::mock_put(p, d1, W_QUEEN);
  PositionTests::mock_put(p, e1, W_KING);
  PositionTests::mock_put(p, f1, W_BISHOP);
  PositionTests::mock_put(p, g1, W_KNIGHT);
  PositionTests::mock_put(p, h1, W_ROOK);

  PositionTests::mock_put(p, a7, B_PAWN);
  PositionTests::mock_put(p, b7, B_PAWN);
  PositionTests::mock_put(p, c7, B_PAWN);
  PositionTests::mock_put(p, d7, B_PAWN);
  PositionTests::mock_put(p, e7, B_PAWN);
  PositionTests::mock_put(p, f7, B_PAWN);
  PositionTests::mock_put(p, g7, B_PAWN);
  PositionTests::mock_put(p, h7, B_PAWN);
  PositionTests::mock_put(p, a8, B_ROOK);
  PositionTests::mock_put(p, b8, B_KNIGHT);
  PositionTests::mock_put(p, c8, B_BISHOP);
  PositionTests::mock_put(p, d8, B_QUEEN);
  PositionTests::mock_put(p, e8, B_KING);
  PositionTests::mock_put(p, f8, B_BISHOP);
  PositionTests::mock_put(p, g8, B_KNIGHT);
  PositionTests::mock_put(p, h8, B_ROOK);
  return p;
}
Position* PositionTests::kings_position() {
  /*
    Regular chess starting position, but with only kings.
  */
  Position* p = new Position();
  PositionTests::mock_put(p, e1, W_KING);
  PositionTests::mock_put(p, e8, B_KING);
  p->side_to_move = WHITE;
  p->en_passant_target_square = NO_SQUARE;
  p->castleRightMask = 0; // No castle rights
  p->half_move_clock = 0;
  p->full_move_clock = 1;
  return p;
}

/* Categorical Tests */
void PositionTests::all_tests() {
  PositionTests::translation_tests();
  PositionTests::retrieval_tests();
  PositionTests::position_manip_tests();
  PositionTests::move_generation_tests();
}
void PositionTests::translation_tests() {
  PositionTests::to_square_test();
  PositionTests::to_piece_test();
  PositionTests::color_test();
  PositionTests::type_test();
}
void PositionTests::retrieval_tests() {
  PositionTests::pieces_Piece_test();
  PositionTests::pieces_PieceType_test();
  PositionTests::pieces_Color_test();
  PositionTests::en_passant_target_test();
  PositionTests::piece_on_test();
  PositionTests::has_castle_right_test();
  PositionTests::halfmove_test();
  PositionTests::fullmove_test();
  PositionTests::to_attack_test();
  PositionTests::to_defend_test();
  PositionTests::copy_test();
}
void PositionTests::position_manip_tests() {
  PositionTests::reset_test();
  PositionTests::put_test();
  PositionTests::remove_test();
  PositionTests::add_castle_right_test();
  PositionTests::revoke_castle_right_test();
  PositionTests::inc_halfmove_test();
  PositionTests::inc_fullmove_test();
  PositionTests::fen_test(); // TODO
}
void PositionTests::move_generation_tests() {
  // TODO
  // pseudo_legal_move_is_legal
  // make_move
  // PositionTests::pseudo_legal_normal_moves_test();
  // pseudo_legal_direction_bitboard
  // pseudo_legal_promo_moves
  // pseudo_legal_ep_moves

  PositionTests::move_direction_before_edge_sq_test();
  PositionTests::move_direction_before_edge_bb_test();
  PositionTests::make_move_test();
  PositionTests::pseudo_legal_pawn_moves_test();
  PositionTests::pseudo_legal_direction_squares_test();
  PositionTests::pseudo_legal_castle_moves_test();
}

/* Translation Tests */
void PositionTests::to_square_test() {
  assert(Position::to_square(0) == NO_SQUARE);
  for( int i = 0; i < Position::SQUARE_LIMIT; i++ ) {
    Square sq = Square(i);
    Bitboard sq_bb = SQUARE_TO_BB(sq);
    assert(Position::to_square(sq_bb) == sq);
  }
}
void PositionTests::to_piece_test() {
  Color c[COLOR_BOUND] = { WHITE, BLACK };
  PieceType pt[PIECE_TYPE_BOUND] = {PAWN,KNIGHT,BISHOP,ROOK,QUEEN,KING};
  Piece tau[COLOR_BOUND][PIECE_TYPE_BOUND] =
        { {W_PAWN,W_KNIGHT,W_BISHOP,W_ROOK,W_QUEEN,W_KING} ,
          {B_PAWN,B_KNIGHT,B_BISHOP,B_ROOK,B_QUEEN,B_KING} };

  for( int i = 0; i < COLOR_BOUND; i++ ) {
    for( int j = 0; j < PIECE_TYPE_BOUND; j++ ) {
      assert(Position::to_piece(c[i],pt[j]) == tau[i][j]);
    }
  }
}
void PositionTests::color_test() {
  Piece w_ps[6] = {W_PAWN,W_KNIGHT,W_BISHOP,W_ROOK,W_QUEEN,W_KING};
  Piece b_ps[6] = {B_PAWN,B_KNIGHT,B_BISHOP,B_ROOK,B_QUEEN,B_KING};
  for( int i = 0; i < 6; i++ ) {
    assert(Position::color(w_ps[i]) == WHITE);
    assert(Position::color(b_ps[i]) == BLACK);
  }
  assert(Position::color(PIECE_BOUND) == NO_COLOR);
  assert(Position::color(NO_PIECE) == NO_COLOR);
}
void PositionTests::type_test() {
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

/* Retrieval and/or Manip Tests */
void PositionTests::copy_test() {
    Position* p = PositionTests::starting_position();
    p->side_to_move = BLACK;
    p->castleRightMask &= ~(WHITE_SHORT_CASTLE);
    p->half_move_clock = 0;
    p->full_move_clock = 1;

    p->colorBB[WHITE] |= SQUARE_TO_BB(e4);
    p->pieceTypeBB[PAWN] |= SQUARE_TO_BB(e4);
    p->pieceBySquare[e4] = W_PAWN;

    p->colorBB[WHITE] &= ~SQUARE_TO_BB(e2);
    p->pieceTypeBB[PAWN] &= ~SQUARE_TO_BB(e2);
    p->pieceBySquare[e2] = NO_PIECE;

    p->en_passant_target_square = e3;

    Position* q = Position::copy(*p);

    for( int i = 0; i < COLOR_BOUND; i++ ) {
      assert(p->colorBB[i] == q->colorBB[i]);
    }
    for( int i = 0; i < PIECE_TYPE_BOUND; i++ ) {
      assert( p->pieceTypeBB[i] == q->pieceTypeBB[i] );
    }
    for( int i = 0; i < Position::SQUARE_LIMIT; i++ ) {
      assert( p->pieceBySquare[i] == q->pieceBySquare[i] );
    }
    assert(p->en_passant_target_square == q->en_passant_target_square);
    assert(p->side_to_move == q->side_to_move);
    assert(p->castleRightMask == q->castleRightMask);
    assert(p->half_move_clock == q->half_move_clock);
    assert(p->full_move_clock == q->full_move_clock);
    delete p;
    delete q;
}


void PositionTests::pieces_Piece_test() {
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
void PositionTests::pieces_PieceType_test() {
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
void PositionTests::pieces_Color_test(){
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

void PositionTests::en_passant_target_test() {
  Position* p = new Position();
  for( int i = 0; i < 64; i++ ) {
    Square sq = static_cast<Square>(i);
    p->en_passant_target_square = sq;
    assert(p->en_passant_target() == sq);
  }
  p->en_passant_target_square = NO_SQUARE;
  assert(p->en_passant_target() == NO_SQUARE);
  delete p;
}

void PositionTests::piece_on_test() {
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
void PositionTests::reset_test() {
  Bitboard white_bb = RANK_1_BB | RANK_2_BB;
  Bitboard black_bb = RANK_7_BB | RANK_8_BB;
  Bitboard pawn_bb  = RANK_2_BB | RANK_7_BB;
  Bitboard rook_bb  = CORNER_BB;
  Bitboard bish_bb  = (RANK_1_BB | RANK_8_BB) & (FILE_C_BB | FILE_F_BB);
  Bitboard knight_bb= (RANK_1_BB | RANK_8_BB) & (FILE_B_BB | FILE_G_BB);
  Bitboard king_bb  = (RANK_1_BB | RANK_8_BB) & (FILE_E_BB);
  Bitboard queen_bb = (RANK_1_BB | RANK_8_BB) & (FILE_D_BB);
  Position* p = new Position();

  // Put a random piece on the board somewhere lol
  p->colorBB[WHITE] |= SQUARE_TO_BB(e1);
  p->pieceTypeBB[KING] |= SQUARE_TO_BB(e1);
  p->pieceBySquare[e1] = W_KING;

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

void PositionTests::put_test() {
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
void PositionTests::remove_test() {
  Position* p = new Position();
  Piece pie = W_KING;
  Color c = WHITE;
  PieceType pt = KING;
  Square sq = e4;
  Bitboard sq_bb = SQUARE_TO_BB(sq);
  p->colorBB[c] |= sq_bb;
  p->pieceTypeBB[pt] |= sq_bb;
  p->pieceBySquare[sq] = pie;
  p->remove(sq);

  assert((p->colorBB[c] & sq_bb) == 0);
  assert((p->pieceTypeBB[pt] & sq_bb) == 0);
  assert(p->pieceBySquare[sq] == NO_PIECE);
  delete p;
}
void PositionTests::add_castle_right_test() {
  Position* p = new Position();
  p->castleRightMask = 0;
  assert((p->castleRightMask & (WHITE_SHORT_CASTLE | WHITE_LONG_CASTLE | BLACK_SHORT_CASTLE | BLACK_LONG_CASTLE)) == 0);

  p->add_castle_right(WHITE_SHORT_CASTLE);
  assert((p->castleRightMask & WHITE_SHORT_CASTLE) != 0);
  p->add_castle_right(WHITE_LONG_CASTLE);
  assert((p->castleRightMask & WHITE_SHORT_CASTLE) != 0);

  p->add_castle_right(BLACK_SHORT_CASTLE);
  assert((p->castleRightMask & BLACK_SHORT_CASTLE) != 0);
  p->add_castle_right(BLACK_LONG_CASTLE);
  assert((p->castleRightMask & BLACK_SHORT_CASTLE) != 0);
  delete p;
}
void PositionTests::has_castle_right_test() {
  Position* p = new Position();
  uint8_t all_rights = (WHITE_SHORT_CASTLE | WHITE_LONG_CASTLE | BLACK_SHORT_CASTLE | BLACK_LONG_CASTLE);
  uint8_t white_castle = (WHITE_SHORT_CASTLE | WHITE_LONG_CASTLE);
  uint8_t black_castle = (BLACK_SHORT_CASTLE | BLACK_LONG_CASTLE);
  uint8_t long_castle = (WHITE_LONG_CASTLE | BLACK_LONG_CASTLE);
  uint8_t short_castle = (WHITE_SHORT_CASTLE | BLACK_SHORT_CASTLE);

  p->castleRightMask = all_rights;
  assert(p->has_castle_right(WHITE_SHORT_CASTLE));
  assert(p->has_castle_right(WHITE_LONG_CASTLE));
  assert(p->has_castle_right(BLACK_SHORT_CASTLE));
  assert(p->has_castle_right(BLACK_LONG_CASTLE));
  p->castleRightMask = white_castle;
  assert(p->has_castle_right(WHITE_SHORT_CASTLE));
  assert(p->has_castle_right(WHITE_LONG_CASTLE));
  assert(!p->has_castle_right(BLACK_SHORT_CASTLE));
  assert(!p->has_castle_right(BLACK_LONG_CASTLE));
  p->castleRightMask = black_castle;
  assert(!p->has_castle_right(WHITE_SHORT_CASTLE));
  assert(!p->has_castle_right(WHITE_LONG_CASTLE));
  assert(p->has_castle_right(BLACK_SHORT_CASTLE));
  assert(p->has_castle_right(BLACK_LONG_CASTLE));
  p->castleRightMask = long_castle;
  assert(!p->has_castle_right(WHITE_SHORT_CASTLE));
  assert(p->has_castle_right(WHITE_LONG_CASTLE));
  assert(!p->has_castle_right(BLACK_SHORT_CASTLE));
  assert(p->has_castle_right(BLACK_LONG_CASTLE));
  p->castleRightMask = short_castle;
  assert(p->has_castle_right(WHITE_SHORT_CASTLE));
  assert(!p->has_castle_right(WHITE_LONG_CASTLE));
  assert(p->has_castle_right(BLACK_SHORT_CASTLE));
  assert(!p->has_castle_right(BLACK_LONG_CASTLE));
  delete p;
}
void PositionTests::revoke_castle_right_test() {
  Position* p = new Position();
  uint8_t all_rights = (WHITE_SHORT_CASTLE | WHITE_LONG_CASTLE | BLACK_SHORT_CASTLE | BLACK_LONG_CASTLE);
  uint8_t w_short_check = (~WHITE_SHORT_CASTLE) & 0x0F; // Have to dodge compiler :P
  uint8_t w_long_check = (~WHITE_LONG_CASTLE) & 0x0F;
  uint8_t b_short_check = (~BLACK_SHORT_CASTLE) & 0x0F;
  uint8_t b_long_check = (~BLACK_LONG_CASTLE) & 0x0F;

  p->castleRightMask = all_rights;
  p->revoke_castle_right(WHITE_SHORT_CASTLE);
  assert(p->castleRightMask == w_short_check);

  p->castleRightMask = all_rights;
  p->revoke_castle_right(WHITE_LONG_CASTLE);
  assert(p->castleRightMask == w_long_check);

  p->castleRightMask = all_rights;
  p->revoke_castle_right(BLACK_SHORT_CASTLE);
  assert(p->castleRightMask == b_short_check);

  p->castleRightMask = all_rights;
  p->revoke_castle_right(BLACK_LONG_CASTLE);
  assert(p->castleRightMask == b_long_check);

  delete p;
}
void PositionTests::inc_halfmove_test() {
  Position* p = new Position();
  p->half_move_clock = 20;
  p->inc_halfmove();
  assert(p->half_move_clock == 21);
  delete p;
}
void PositionTests::inc_fullmove_test() {
  Position* p = new Position();
  p->full_move_clock = 70;
  p->inc_fullmove();
  assert(p->full_move_clock == 71);
  delete p;
}
void PositionTests::halfmove_test() {
  Position* p = new Position();
  p->half_move_clock = 15;
  assert(p->halfmove() == 15);
  delete p;
}
void PositionTests::fullmove_test() {
  Position* p = new Position();
  p->full_move_clock = 95;
  assert(p->fullmove() == 95);
  delete p;
}

void PositionTests::fen_test() {} // TODO

void PositionTests::to_attack_test() {
  Position* p = new Position();
  p->side_to_move = WHITE;
  assert(p->to_attack() == WHITE);
  p->side_to_move = BLACK;
  assert(p->to_attack() == BLACK);
  delete p;
}

void PositionTests::to_defend_test() {
  Position* p = new Position();
  p->side_to_move = WHITE;
  assert(p->to_defend() == BLACK);
  p->side_to_move = BLACK;
  assert(p->to_defend() == WHITE);
  delete p;
}



/* Move Generation Tests */
void PositionTests::move_direction_before_edge_sq_test() {
  Square cant_move_west_sqs[8] = {a1,a2,a3,a4,a5,a6,a7,a8};
  Square cant_move_east_sqs[8] = {h1,h2,h3,h4,h5,h6,h7,h8};
  Square cant_move_north_sqs[8] = {a8,b8,c8,d8,e8,f8,g8,h8};
  Square cant_move_south_sqs[8] = {a1,b1,c1,d1,e1,f1,g1,h1};

  Square rank_7_sqs[8] = {a7,b7,c7,d7,e7,f7,g7,h7};
  Square rank_2_sqs[8] = {a2,b2,c2,d2,e2,f2,g2,h2};
  Square file_b_sqs[8] = {b1,b2,b3,b4,b5,b6,b7,b8};
  Square file_g_sqs[8] = {g1,g2,g3,g4,g5,g6,g7,g8};


  // Assert that you may NOT go in a direction
  for( int i = 0; i < 8; i++) {
    //  NOT NORTH
    assert(!Position::move_direction_before_edge(cant_move_north_sqs[i],NORTH));
    //  NOT SOUTH
    assert(!Position::move_direction_before_edge(cant_move_south_sqs[i],SOUTH));
    // NOT WEST
    assert(!Position::move_direction_before_edge(cant_move_west_sqs[i],WEST));
    // NOT EAST
    assert(!Position::move_direction_before_edge(cant_move_east_sqs[i],EAST));
    // NOT NORTH_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_sqs[i],NORTH_EAST));
    assert(!Position::move_direction_before_edge(cant_move_north_sqs[i],NORTH_EAST));
    // NOT NORTH_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_sqs[i],NORTH_WEST));
    assert(!Position::move_direction_before_edge(cant_move_north_sqs[i],NORTH_WEST));
    // NOT SOUTH_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_sqs[i],SOUTH_EAST));
    assert(!Position::move_direction_before_edge(cant_move_south_sqs[i],SOUTH_EAST));
    // NOT SOUTH_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_sqs[i],SOUTH_WEST));
    assert(!Position::move_direction_before_edge(cant_move_south_sqs[i],SOUTH_WEST));
    // NOT NORTH_NORTH_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_sqs[i],NORTH_NORTH_EAST));
    assert(!Position::move_direction_before_edge(cant_move_north_sqs[i],NORTH_NORTH_EAST));
    assert(!Position::move_direction_before_edge(rank_7_sqs[i],NORTH_NORTH_EAST));
    // NOT NORTH_NORTH_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_sqs[i],NORTH_NORTH_WEST));
    assert(!Position::move_direction_before_edge(cant_move_north_sqs[i],NORTH_NORTH_WEST));
    assert(!Position::move_direction_before_edge(rank_7_sqs[i],NORTH_NORTH_WEST));
    // NOT NORTH_EAST_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_sqs[i],NORTH_EAST_EAST));
    assert(!Position::move_direction_before_edge(cant_move_north_sqs[i],NORTH_EAST_EAST));
    assert(!Position::move_direction_before_edge(file_g_sqs[i],NORTH_EAST_EAST));
    // NOT SOUTH_EAST_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_sqs[i],SOUTH_EAST_EAST));
    assert(!Position::move_direction_before_edge(cant_move_south_sqs[i],SOUTH_EAST_EAST));
    assert(!Position::move_direction_before_edge(file_g_sqs[i],SOUTH_EAST_EAST));
    // NOT SOUTH_SOUTH_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_sqs[i],SOUTH_SOUTH_EAST));
    assert(!Position::move_direction_before_edge(cant_move_south_sqs[i],SOUTH_SOUTH_EAST));
    assert(!Position::move_direction_before_edge(rank_2_sqs[i],SOUTH_SOUTH_EAST));
    // NOT SOUTH_SOUTH_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_sqs[i],SOUTH_SOUTH_WEST));
    assert(!Position::move_direction_before_edge(cant_move_south_sqs[i],SOUTH_SOUTH_WEST));
    assert(!Position::move_direction_before_edge(rank_2_sqs[i],SOUTH_SOUTH_WEST));
    // NOT SOUTH_WEST_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_sqs[i],SOUTH_WEST_WEST));
    assert(!Position::move_direction_before_edge(cant_move_south_sqs[i],SOUTH_WEST_WEST));
    assert(!Position::move_direction_before_edge(file_b_sqs[i],SOUTH_WEST_WEST));
    // NOT NORTH_WEST_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_sqs[i],SOUTH_WEST_WEST));
    assert(!Position::move_direction_before_edge(cant_move_south_sqs[i],SOUTH_WEST_WEST));
    assert(!Position::move_direction_before_edge(file_b_sqs[i],SOUTH_WEST_WEST));
  }


  const char file_char[8] = {'a','b','c','d','e','f','g','h'};
  // Assert that you may move in a direction
  for( int rank_idx = 0; rank_idx < 8; rank_idx++) {
    for( int file_idx = 0; file_idx < 8; file_idx++) {
      Square sq = rank_file_sqs[rank_idx][file_idx];

      const int rank = rank_idx+1;
      const char file = file_char[file_idx];

      if( rank != 8 ) {
        // MAY MOVE NORTH
        assert(Position::move_direction_before_edge(sq,NORTH));
      }
      if( rank != 1 ) {
        // MAY MOVE SOUTH
        assert(Position::move_direction_before_edge(sq,SOUTH));
      }
      if( file != 'h' ) {
        // MAY MOVE EAST
        assert(Position::move_direction_before_edge(sq,EAST));
      }
      if( file != 'a') {
        // MAY MOVE WEST
        assert(Position::move_direction_before_edge(sq,WEST));
      }

      if(file != 'h' && rank != 8) {
        // MAY MOVE NORTH_EAST
        assert(Position::move_direction_before_edge(sq,NORTH_EAST));

        if(rank != 7) {
          // MAY MOVE NORTH_NORTH_EAST
          assert(Position::move_direction_before_edge(sq,NORTH_NORTH_EAST));
        }

        if(file != 'g') {
          // MAY MOVE NORTH_EAST_EAST
          assert(Position::move_direction_before_edge(sq,NORTH_EAST_EAST));
        }

      }
      if(file != 'a' && rank != 8) {
        // MAY MOVE NORTH_WEST
        assert(Position::move_direction_before_edge(sq,NORTH_WEST));
        if(rank != 7) {
          // MAY MOVE NORTH_NORTH_WEST
          assert(Position::move_direction_before_edge(sq,NORTH_NORTH_WEST));
        }
        if(file != 'b') {
          // MAY MOVE NORTH_WEST_WEST
          assert(Position::move_direction_before_edge(sq,NORTH_WEST_WEST));
        }
      }

      if(file != 'h' && rank != 1) {
        // MAY MOVE SOUTH_EAST
        assert(Position::move_direction_before_edge(sq,SOUTH_EAST));
        if(rank != 2) {
          // MAY MOVE SOUTH_SOUTH_EAST
          assert(Position::move_direction_before_edge(sq,SOUTH_SOUTH_EAST));
        }
        if(file != 'g') {
          // MAY MOVE SOUTH_EAST_EAST
          assert(Position::move_direction_before_edge(sq,SOUTH_EAST_EAST));
        }
      }
      if(file != 'a' && rank != 1) {
        // MAY MOVE SOUTH_WEST
        assert(Position::move_direction_before_edge(sq,SOUTH_WEST));
        if(rank != 2) {
          // MAY MOVE SOUTH_SOUTH_WEST
          assert(Position::move_direction_before_edge(sq,SOUTH_SOUTH_WEST));
        }
        if(file != 'b') {
          // MAY MOVE SOUTH_WEST_WEST
          assert(Position::move_direction_before_edge(sq,SOUTH_WEST_WEST));
        }
      }
    }
  }

}

void PositionTests::move_direction_before_edge_bb_test() {
  std::vector<Bitboard> cant_move_west_bbs = Bitboard_Debug::discretize(FILE_A_BB);
  std::vector<Bitboard> cant_move_east_bbs = Bitboard_Debug::discretize(FILE_H_BB);
  std::vector<Bitboard> cant_move_north_bbs = Bitboard_Debug::discretize(RANK_8_BB);
  std::vector<Bitboard> cant_move_south_bbs = Bitboard_Debug::discretize(RANK_1_BB);

  std::vector<Bitboard> rank_7_bbs = Bitboard_Debug::discretize(RANK_7_BB);
  std::vector<Bitboard> rank_2_bbs = Bitboard_Debug::discretize(RANK_2_BB);
  std::vector<Bitboard> file_b_bbs = Bitboard_Debug::discretize(FILE_B_BB);
  std::vector<Bitboard> file_g_bbs = Bitboard_Debug::discretize(FILE_G_BB);


  // Assert that you may NOT go in a direction
  for( int i = 0; i < 8; i++) {
    //  NOT NORTH
    assert(!Position::move_direction_before_edge(cant_move_north_bbs[i],NORTH));
    //  NOT SOUTH
    assert(!Position::move_direction_before_edge(cant_move_south_bbs[i],SOUTH));
    // NOT WEST
    assert(!Position::move_direction_before_edge(cant_move_west_bbs[i],WEST));
    // NOT EAST
    assert(!Position::move_direction_before_edge(cant_move_east_bbs[i],EAST));
    // NOT NORTH_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_bbs[i],NORTH_EAST));
    assert(!Position::move_direction_before_edge(cant_move_north_bbs[i],NORTH_EAST));
    // NOT NORTH_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_bbs[i],NORTH_WEST));
    assert(!Position::move_direction_before_edge(cant_move_north_bbs[i],NORTH_WEST));
    // NOT SOUTH_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_bbs[i],SOUTH_EAST));
    assert(!Position::move_direction_before_edge(cant_move_south_bbs[i],SOUTH_EAST));
    // NOT SOUTH_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_bbs[i],SOUTH_WEST));
    assert(!Position::move_direction_before_edge(cant_move_south_bbs[i],SOUTH_WEST));
    // NOT NORTH_NORTH_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_bbs[i],NORTH_NORTH_EAST));
    assert(!Position::move_direction_before_edge(cant_move_north_bbs[i],NORTH_NORTH_EAST));
    assert(!Position::move_direction_before_edge(rank_7_bbs[i],NORTH_NORTH_EAST));
    // NOT NORTH_NORTH_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_bbs[i],NORTH_NORTH_WEST));
    assert(!Position::move_direction_before_edge(cant_move_north_bbs[i],NORTH_NORTH_WEST));
    assert(!Position::move_direction_before_edge(rank_7_bbs[i],NORTH_NORTH_WEST));
    // NOT NORTH_EAST_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_bbs[i],NORTH_EAST_EAST));
    assert(!Position::move_direction_before_edge(cant_move_north_bbs[i],NORTH_EAST_EAST));
    assert(!Position::move_direction_before_edge(file_g_bbs[i],NORTH_EAST_EAST));
    // NOT SOUTH_EAST_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_bbs[i],SOUTH_EAST_EAST));
    assert(!Position::move_direction_before_edge(cant_move_south_bbs[i],SOUTH_EAST_EAST));
    assert(!Position::move_direction_before_edge(file_g_bbs[i],SOUTH_EAST_EAST));
    // NOT SOUTH_SOUTH_EAST
    assert(!Position::move_direction_before_edge(cant_move_east_bbs[i],SOUTH_SOUTH_EAST));
    assert(!Position::move_direction_before_edge(cant_move_south_bbs[i],SOUTH_SOUTH_EAST));
    assert(!Position::move_direction_before_edge(rank_2_bbs[i],SOUTH_SOUTH_EAST));
    // NOT SOUTH_SOUTH_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_bbs[i],SOUTH_SOUTH_WEST));
    assert(!Position::move_direction_before_edge(cant_move_south_bbs[i],SOUTH_SOUTH_WEST));
    assert(!Position::move_direction_before_edge(rank_2_bbs[i],SOUTH_SOUTH_WEST));
    // NOT SOUTH_WEST_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_bbs[i],SOUTH_WEST_WEST));
    assert(!Position::move_direction_before_edge(cant_move_south_bbs[i],SOUTH_WEST_WEST));
    assert(!Position::move_direction_before_edge(file_b_bbs[i],SOUTH_WEST_WEST));
    // NOT NORTH_WEST_WEST
    assert(!Position::move_direction_before_edge(cant_move_west_bbs[i],SOUTH_WEST_WEST));
    assert(!Position::move_direction_before_edge(cant_move_south_bbs[i],SOUTH_WEST_WEST));
    assert(!Position::move_direction_before_edge(file_b_bbs[i],SOUTH_WEST_WEST));
  }


  const char file_char[8] = {'a','b','c','d','e','f','g','h'};
  // Assert that you may move in a direction
  for( int rank_idx = 0; rank_idx < 8; rank_idx++) {
    for( int file_idx = 0; file_idx < 8; file_idx++) {
      Bitboard bb = SQUARE_TO_BB(rank_file_sqs[rank_idx][file_idx]);

      const int rank = rank_idx+1;
      const char file = file_char[file_idx];

      if( rank != 8 ) {
        // MAY MOVE NORTH
        assert(Position::move_direction_before_edge(bb,NORTH));
      }
      if( rank != 1 ) {
        // MAY MOVE SOUTH
        assert(Position::move_direction_before_edge(bb,SOUTH));
      }
      if( file != 'h' ) {
        // MAY MOVE EAST
        assert(Position::move_direction_before_edge(bb,EAST));
      }
      if( file != 'a') {
        // MAY MOVE WEST
        assert(Position::move_direction_before_edge(bb,WEST));
      }

      if(file != 'h' && rank != 8) {
        // MAY MOVE NORTH_EAST
        assert(Position::move_direction_before_edge(bb,NORTH_EAST));

        if(rank != 7) {
          // MAY MOVE NORTH_NORTH_EAST
          assert(Position::move_direction_before_edge(bb,NORTH_NORTH_EAST));
        }

        if(file != 'g') {
          // MAY MOVE NORTH_EAST_EAST
          assert(Position::move_direction_before_edge(bb,NORTH_EAST_EAST));
        }

      }
      if(file != 'a' && rank != 8) {
        // MAY MOVE NORTH_WEST
        assert(Position::move_direction_before_edge(bb,NORTH_WEST));
        if(rank != 7) {
          // MAY MOVE NORTH_NORTH_WEST
          assert(Position::move_direction_before_edge(bb,NORTH_NORTH_WEST));
        }
        if(file != 'b') {
          // MAY MOVE NORTH_WEST_WEST
          assert(Position::move_direction_before_edge(bb,NORTH_WEST_WEST));
        }
      }

      if(file != 'h' && rank != 1) {
        // MAY MOVE SOUTH_EAST
        assert(Position::move_direction_before_edge(bb,SOUTH_EAST));
        if(rank != 2) {
          // MAY MOVE SOUTH_SOUTH_EAST
          assert(Position::move_direction_before_edge(bb,SOUTH_SOUTH_EAST));
        }
        if(file != 'g') {
          // MAY MOVE SOUTH_EAST_EAST
          assert(Position::move_direction_before_edge(bb,SOUTH_EAST_EAST));
        }
      }
      if(file != 'a' && rank != 1) {
        // MAY MOVE SOUTH_WEST
        assert(Position::move_direction_before_edge(bb,SOUTH_WEST));
        if(rank != 2) {
          // MAY MOVE SOUTH_SOUTH_WEST
          assert(Position::move_direction_before_edge(bb,SOUTH_SOUTH_WEST));
        }
        if(file != 'b') {
          // MAY MOVE SOUTH_WEST_WEST
          assert(Position::move_direction_before_edge(bb,SOUTH_WEST_WEST));
        }
      }
    }
  }
}



/* Move Generation */
void PositionTests::make_move_test() {
  // Test Every Flag (Promotions, En Passant, Castle, and No flag)
  {
    // Ensure en passant target square resets
    // Ensure full move clock and half move clock update
    Position* p = PositionTests::starting_position();
    p->make_move(Move(e2,e4));
    assert(p->en_passant_target_square == e3);
    assert(p->full_move_clock == 1);
    assert(p->half_move_clock == 0);
    p->make_move(Move(e7,e5));
    assert(p->en_passant_target_square == e6);
    assert(p->full_move_clock == 2);
    assert(p->half_move_clock == 0);
    p->make_move(Move(g1,f3));
    assert(p->en_passant_target_square == NO_SQUARE);
    assert(p->full_move_clock == 2);
    assert(p->half_move_clock == 1);
    delete p;
  }
  { // NO FLAG -> Rook move removes castle rights
    Position* p = PositionTests::kings_position();
    p->castleRightMask = (WHITE_CASTLE | BLACK_CASTLE);
    p->side_to_move = WHITE;

    PositionTests::mock_put(p,a1, W_ROOK);
    PositionTests::mock_put(p,h1, W_ROOK);
    PositionTests::mock_put(p,a8, B_ROOK);
    PositionTests::mock_put(p,h8, B_ROOK);

    p->make_move(Move(a1,a4));
    assert((p->castleRightMask & (WHITE_LONG_CASTLE)) == 0);

    p->make_move(Move(a8,a5));
    assert((p->castleRightMask & (BLACK_LONG_CASTLE)) == 0);

    p->make_move(Move(h1,h4));
    assert((p->castleRightMask & (WHITE_SHORT_CASTLE)) == 0);

    p->make_move(Move(h8,h5));
    assert((p->castleRightMask & (BLACK_SHORT_CASTLE)) == 0);

    delete p;
  }
  { // NO FLAG -> King move revokes relevant castle rights
    Position* p = PositionTests::kings_position();
    p->side_to_move = WHITE;
    p->castleRightMask = (WHITE_CASTLE | BLACK_CASTLE);

    p->make_move(Move(e1,e2));
    assert((p->castleRightMask & (WHITE_CASTLE)) == 0);

    p->make_move(Move(e8,e7));
    assert((p->castleRightMask & (BLACK_CASTLE)) == 0);
  }
  { // Flag = CASTLE
    Position* p = PositionTests::kings_position();
    p->castleRightMask = (WHITE_CASTLE);
    PositionTests::mock_put(p,a1,W_ROOK);
    PositionTests::mock_put(p,h1,W_ROOK);

    p->side_to_move = WHITE;
    p->make_move(Move(e1,g1, Move::CASTLE));
    assert(p->pieceBySquare[g1] == W_KING);
    assert(p->pieceBySquare[f1] == W_ROOK);
    assert(p->pieceBySquare[h1] == NO_PIECE);
    assert(p->pieceBySquare[e1] == NO_PIECE);
    assert((p->castleRightMask & (WHITE_CASTLE)) == 0);
    delete p;

    Position* q = PositionTests::kings_position();
    p->castleRightMask = (BLACK_CASTLE);
    PositionTests::mock_put(p,a8,B_ROOK);
    PositionTests::mock_put(p,h8,B_ROOK);
    p->side_to_move = BLACK;
    p->make_move(Move(e8,g8, Move::CASTLE));
    assert(p->pieceBySquare[g8] == B_KING);
    assert(p->pieceBySquare[f8] == B_ROOK);
    assert(p->pieceBySquare[h8] == NO_PIECE);
    assert(p->pieceBySquare[e8] == NO_PIECE);
    assert((p->castleRightMask & (BLACK_CASTLE)) == 0);
    delete q;
  }
  { // En passant flag
    Position* p = PositionTests::kings_position();
    p->side_to_move = WHITE;
    PositionTests::mock_put(p,c5,W_PAWN);
    PositionTests::mock_put(p,b5,B_PAWN);
    p->en_passant_target_square = b6;
    p->make_move(Move(c5,b6,Move::EN_PASSANT));

    assert(p->pieceBySquare[b5] == NO_PIECE);
    assert(p->pieceBySquare[c5] == NO_PIECE);
    assert(p->pieceBySquare[b6] == W_PAWN);
    assert(p->en_passant_target_square == NO_SQUARE);
    assert((p->colorBB[WHITE] & p->pieceTypeBB[PAWN]) == SQUARE_TO_BB(b6));
    delete p;
  }
  { // Promote knight flag
    Position* p = PositionTests::kings_position();
    p->side_to_move = WHITE;
    PositionTests::mock_put(p,b7,W_PAWN);
    p->make_move(Move(b7,b8,Move::PROMOTE_KNIGHT));
    assert(p->pieceBySquare[b8] == W_KNIGHT);
    assert( (p->colorBB[WHITE] & p->pieceTypeBB[KNIGHT]) == SQUARE_TO_BB(b8) );
    assert( p->pieceTypeBB[PAWN] == EMPTY_BB );
    delete p;
  }
  { // Promote bishop flag
    Position* p = PositionTests::kings_position();
    p->side_to_move = WHITE;
    PositionTests::mock_put(p,b7,W_PAWN);
    p->make_move(Move(b7,b8,Move::PROMOTE_BISHOP));
    assert(p->pieceBySquare[b8] == W_BISHOP);
    assert( (p->colorBB[WHITE] & p->pieceTypeBB[BISHOP]) == SQUARE_TO_BB(b8) );
    assert( p->pieceTypeBB[PAWN] == EMPTY_BB );
    delete p;
  }
  { // Promote rook flag
    Position* p = PositionTests::kings_position();
    p->side_to_move = WHITE;
    PositionTests::mock_put(p,b7,W_PAWN);
    p->make_move(Move(b7,b8,Move::PROMOTE_ROOK));
    assert(p->pieceBySquare[b8] == W_ROOK);
    assert( (p->colorBB[WHITE] & p->pieceTypeBB[ROOK]) == SQUARE_TO_BB(b8) );
    assert( p->pieceTypeBB[PAWN] == EMPTY_BB );
    delete p;
  }
  { // Promote queen flag
    Position* p = PositionTests::kings_position();
    p->side_to_move = WHITE;
    PositionTests::mock_put(p,b7,W_PAWN);
    p->make_move(Move(b7,b8,Move::PROMOTE_QUEEN));
    assert(p->pieceBySquare[b8] == W_QUEEN);
    assert( (p->colorBB[WHITE] & p->pieceTypeBB[QUEEN]) == SQUARE_TO_BB(b8) );
    assert( p->pieceTypeBB[PAWN] == EMPTY_BB );
    delete p;
  }
}
void PositionTests::pseudo_legal_castle_moves_test() {
  { // No castle rights (WHITE AND BLACK)
    Position* p = PositionTests::kings_position();
    p->castleRightMask = 0;
    p->side_to_move = WHITE;
    PositionTests::mock_put(p,h1, W_ROOK);
    PositionTests::mock_put(p,a1, W_ROOK);
    PositionTests::mock_put(p,h8, B_ROOK);
    PositionTests::mock_put(p,a8, B_ROOK);
    assert(p->pseudo_legal_castle_moves().size() == 0 );

    p->side_to_move = BLACK;
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    delete p;
  }
  { // Blocked short castle
    Position* p = PositionTests::kings_position();
    p->castleRightMask = (WHITE_SHORT_CASTLE | BLACK_SHORT_CASTLE);
    // White
    p->side_to_move = WHITE;
    PositionTests::mock_put(p,h1, W_ROOK);
    PositionTests::mock_put(p,g1, W_KNIGHT);
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    PositionTests::mock_put(p, f1, W_BISHOP );
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    PositionTests::mock_remove(p,g1);
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    // Black
    p->side_to_move = BLACK;
    PositionTests::mock_put(p,h8, B_ROOK);
    PositionTests::mock_put(p,g8, B_KNIGHT);
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    PositionTests::mock_put(p, f8, B_BISHOP );
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    PositionTests::mock_remove(p,g8);
    assert(p->pseudo_legal_castle_moves().size() == 0 );

    delete p;
  }
  { // Unblocked Short Castle
    Position* p = PositionTests::kings_position();
    p->castleRightMask = (WHITE_SHORT_CASTLE | BLACK_SHORT_CASTLE);
    PositionTests::mock_put(p,h1, W_ROOK);
    assert(p->pseudo_legal_castle_moves().size() == 1 );

    PositionTests::mock_put(p,h8, B_ROOK);
    assert(p->pseudo_legal_castle_moves().size() == 1 );
    delete p;
  }
  { // Blocked long castle
    Position* p = PositionTests::kings_position();
    p->castleRightMask = (WHITE_LONG_CASTLE | BLACK_LONG_CASTLE);
    // WHITE
    p->side_to_move = WHITE;
    PositionTests::mock_put(p,a1, W_ROOK);
    PositionTests::mock_put(p,b1, W_KNIGHT);
    PositionTests::mock_put(p,c1, W_BISHOP);
    PositionTests::mock_put(p,d1, W_QUEEN);
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    PositionTests::mock_remove(p,b1);
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    PositionTests::mock_remove(p,c1);
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    // BLACK
    p->side_to_move = BLACK;
    PositionTests::mock_put(p,a8, B_ROOK);
    PositionTests::mock_put(p,b8, B_KNIGHT);
    PositionTests::mock_put(p,c8, B_BISHOP);
    PositionTests::mock_put(p,d8, B_QUEEN);
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    PositionTests::mock_remove(p,b8);
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    PositionTests::mock_remove(p,c8);
    assert(p->pseudo_legal_castle_moves().size() == 0 );
    delete p;
  }
  { // Unblocked long castle
    Position* p = PositionTests::kings_position();
    p->castleRightMask = (WHITE_LONG_CASTLE | BLACK_LONG_CASTLE);
    // WHITE
    p->side_to_move = WHITE;
    PositionTests::mock_put(p,a1, W_ROOK);
    assert(p->pseudo_legal_castle_moves().size() == 1 );
    // BLACK
    p->side_to_move = BLACK;
    PositionTests::mock_put(p,a8, B_ROOK);
    assert(p->pseudo_legal_castle_moves().size() == 1 );
    delete p;
  }
  { // Unblocked long and short castle
    Position* p = PositionTests::kings_position();
    PositionTests::mock_put(p,a1, W_ROOK);
    PositionTests::mock_put(p,h1, W_ROOK);
    PositionTests::mock_put(p,a8, B_ROOK);
    PositionTests::mock_put(p,h8, B_ROOK);
    p->castleRightMask = (WHITE_CASTLE | BLACK_CASTLE);
    p->side_to_move = WHITE;
    assert(p->pseudo_legal_castle_moves().size() == 2);
    p->side_to_move = BLACK;
    assert(p->pseudo_legal_castle_moves().size() == 2);
    delete p;
  }
}
void PositionTests::pseudo_legal_pawn_moves_test() {

  Position* p = new Position();
  PositionTests::mock_put(p,e4,W_PAWN);

  std::vector<Move> tau1 = p->pseudo_legal_pawn_moves(e4);
  assert(tau1.size() ==1 );
  assert(tau1[0].raw() == Move(e4,e5).raw());

  PositionTests::mock_put(p,f5,B_PAWN);
  PositionTests::mock_put(p,d5,B_PAWN);
  std::vector<Move> tau2 = p->pseudo_legal_pawn_moves(e4);
  assert(tau2.size() == 3);
  assert(tau2[0].raw() != tau2[1].raw() && tau2[1].raw() != tau2[2].raw() && tau2[0].raw() != tau2[2].raw());
  assert(tau2[0].raw() == Move(e4,f5).raw() || tau2[0].raw() == Move(e4,d5).raw() || tau2[0].raw() == Move(e4,e5).raw());
  assert(tau2[1].raw() == Move(e4,f5).raw() || tau2[1].raw() == Move(e4,d5).raw() || tau2[1].raw() == Move(e4,e5).raw());
  assert(tau2[2].raw() == Move(e4,f5).raw() || tau2[2].raw() == Move(e4,d5).raw() || tau2[2].raw() == Move(e4,e5).raw());

  PositionTests::mock_put(p,e5,B_PAWN);

  std::vector<Move> phi = p->pseudo_legal_pawn_moves(e4);
  assert(phi.size() == 2);
  assert(phi[0].raw() != phi[1].raw());
  assert(phi[0].raw() == Move(e4,f5).raw() || phi[0].raw() == Move(e4,d5).raw());
  assert(phi[1].raw() == Move(e4,f5).raw() || phi[1].raw() == Move(e4,d5).raw());

  PositionTests::mock_put(p,a2,B_PAWN);
  p->side_to_move = BLACK;
  assert(p->pseudo_legal_pawn_moves(a2).size() == 0);

  delete p;

  Position* p2 = new Position();
  p2->side_to_move = BLACK;
  PositionTests::mock_put(p,e4,W_PAWN);
  PositionTests::mock_put(p,e5,B_PAWN);

  assert(p->pseudo_legal_pawn_moves(e5).size() == 0);

  delete p2;

}
void PositionTests::pseudo_legal_direction_squares_test() {
  // Check propogate
  // Check we cant capture own pieces
  // Check we capture first enemy piece
  { // Capture enemy piece
    Position* p = new Position();
    // White king on e4
    PositionTests::mock_put(p,e4,W_PAWN);
    p->side_to_move = WHITE;

    // Black rook on f5
    PositionTests::mock_put(p,f5,B_ROOK);

    std::vector<Square> generated = p->pseudo_legal_direction_squares(e4,NORTH_EAST,false);
    assert(generated[0] == f5);
    delete p;
  }
  { // Capture enemy piece  w/ propogation -> Only capture first piece
    Position* p = new Position();
    // White king on e4
    p->side_to_move = WHITE;
    PositionTests::mock_put(p,e4,W_KING);

    // Black rook on g6
    PositionTests::mock_put(p,g6,B_ROOK);

    // Black rook on h7
    PositionTests::mock_put(p,h7,B_ROOK);

    std::vector<Square> prop_generated = p->pseudo_legal_direction_squares(e4,NORTH_EAST,true);
    std::vector<Square> no_prop_generated = p->pseudo_legal_direction_squares(e4,NORTH_EAST,false);
    assert(no_prop_generated.size() == 1);
    assert(no_prop_generated[0] == f5);
    assert(prop_generated.size() == 2);
    assert(prop_generated[0] == f5);
    assert(prop_generated[1] == g6);
    delete p;
  }
  { // CANT capture own piece
    Position* p = new Position();
    // White king on e4
    p->side_to_move = WHITE;
    PositionTests::mock_put(p,e4,W_KING);
    // White rook on g6
    PositionTests::mock_put(p,g6,W_ROOK);

    std::vector<Square> generated = p->pseudo_legal_direction_squares(e4,NORTH_EAST,true);
    assert(generated.size() == 1);
    assert(generated[0] == Square(e4 + NORTH_EAST));

    delete p;
  }
}


int main( int argc, char** argv ) {
  PositionTests::all_tests();
  std::cout << "PositionTests Passed" << std::endl;
  return 0;
}
