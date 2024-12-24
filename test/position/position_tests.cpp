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


class PositionTests {
public:

  static void unit_tests() {
    PositionTests::color_test();
    PositionTests::type_test();
    PositionTests::pieces_Piece_test();
    PositionTests::pieces_PieceType_test();
    PositionTests::pieces_Color_test();
    PositionTests::en_passant_target_test();
    PositionTests::piece_on_test();
    PositionTests::reset_test();
    PositionTests::put_test();
    PositionTests::remove_test();
    PositionTests::add_castle_right_test();
    PositionTests::has_castle_right_test();
    PositionTests::revoke_castle_right_test();
    PositionTests::inc_halfmove_test();
    PositionTests::inc_fullmove_test();
    PositionTests::halfmove_test();
    PositionTests::fullmove_test();
    PositionTests::fen_test(); // TODO
    PositionTests::to_attack_test();
    PositionTests::to_defend_test();
    PositionTests::move_direction_before_edge_sq_test();
    PositionTests::move_direction_before_edge_bb_test();

    PositionTests::pseudo_legal_pawn_moves_test();
    PositionTests::pseudo_legal_normal_moves_test();
  }
  /* UNIT TESTS */
  static void copy_test() {}
  static void color_test() {
    Piece w_ps[6] = {W_PAWN,W_KNIGHT,W_BISHOP,W_ROOK,W_QUEEN,W_KING};
    Piece b_ps[6] = {B_PAWN,B_KNIGHT,B_BISHOP,B_ROOK,B_QUEEN,B_KING};
    for( int i = 0; i < 6; i++ ) {
      assert(Position::color(w_ps[i]) == WHITE);
      assert(Position::color(b_ps[i]) == BLACK);
    }
    assert(Position::color(PIECE_BOUND) == NO_COLOR);
    assert(Position::color(NO_PIECE) == NO_COLOR);
  }
  static void type_test() {
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
  static void pieces_Piece_test() {
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
  static void pieces_PieceType_test() {
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
  static void pieces_Color_test(){
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

  static void en_passant_target_test() {
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

  static void piece_on_test() {
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
  static void reset_test() {
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

  static void put_test() {
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
  static void remove_test() {
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
  static void add_castle_right_test() {
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
  static void has_castle_right_test() {
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
  static void revoke_castle_right_test() {
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
  static void inc_halfmove_test() {
    Position* p = new Position();
    p->half_move_clock = 20;
    p->inc_halfmove();
    assert(p->half_move_clock == 21);
    delete p;
  }
  static void inc_fullmove_test() {
    Position* p = new Position();
    p->full_move_clock = 70;
    p->inc_fullmove();
    assert(p->full_move_clock == 71);
    delete p;
  }
  static void halfmove_test() {
    Position* p = new Position();
    p->half_move_clock = 15;
    assert(p->halfmove() == 15);
    delete p;
  }
  static void fullmove_test() {
    Position* p = new Position();
    p->full_move_clock = 95;
    assert(p->fullmove() == 95);
    delete p;
  }

  static void fen_test() {} // TODO

  static void to_attack_test() {
    Position* p = new Position();
    p->side_to_move = WHITE;
    assert(p->to_attack() == WHITE);
    p->side_to_move = BLACK;
    assert(p->to_attack() == BLACK);
    delete p;
  }

  static void to_defend_test() {
    Position* p = new Position();
    p->side_to_move = WHITE;
    assert(p->to_defend() == BLACK);
    p->side_to_move = BLACK;
    assert(p->to_defend() == WHITE);
    delete p;
  }

  static void move_direction_before_edge_sq_test() {
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

  static void move_direction_before_edge_bb_test() {
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
  static void pseudo_legal_normal_moves_test() {} //TODO
  static void pseudo_legal_pawn_moves_test() {
    // TODO UPDATE
    Bitboard e4_bb = SQUARE_TO_BB(e4);
    Bitboard e5_bb = SQUARE_TO_BB(e5);
    Bitboard f5_bb = SQUARE_TO_BB(f5);
    Bitboard d5_bb = SQUARE_TO_BB(d5);
    Position* p = new Position();
    p->side_to_move = WHITE;

    p->colorBB[WHITE] |= e4_bb;
    p->pieceTypeBB[PAWN] |= e4_bb;
    p->pieceBySquare[e4] = W_PAWN;
    std::vector<Move> tau1 = p->pseudo_legal_pawn_moves(e4);
    assert(tau1.size() ==1 );
    assert(tau1[0].raw() == Move(e4,e5).raw());
    
    
    p->colorBB[BLACK] |= f5_bb;
    p->pieceTypeBB[PAWN] |= f5_bb;
    p->pieceBySquare[f5] = B_PAWN;
    p->colorBB[BLACK] |= d5_bb;
    p->pieceTypeBB[PAWN] |= d5_bb;
    p->pieceBySquare[d5] = B_PAWN;

    std::vector<Move> tau2 = p->pseudo_legal_pawn_moves(e4);
    assert(tau2.size() == 3);
    assert(tau2[0].raw() != tau2[1].raw() && tau2[1].raw() != tau2[2].raw() && tau2[0].raw() != tau2[2].raw());
    assert(tau2[0].raw() == Move(e4,f5).raw() || tau2[0].raw() == Move(e4,d5).raw() || tau2[0].raw() == Move(e4,e5).raw());
    assert(tau2[1].raw() == Move(e4,f5).raw() || tau2[1].raw() == Move(e4,d5).raw() || tau2[1].raw() == Move(e4,e5).raw());
    assert(tau2[2].raw() == Move(e4,f5).raw() || tau2[2].raw() == Move(e4,d5).raw() || tau2[2].raw() == Move(e4,e5).raw());
    
    p->colorBB[BLACK] |= e5_bb;
    p->pieceTypeBB[PAWN] |= e5_bb;
    p->pieceBySquare[e5] = B_PAWN;

    std::vector<Move> phi = p->pseudo_legal_pawn_moves(e4);
    assert(phi.size() == 2);
    assert(phi[0].raw() != phi[1].raw());
    assert(phi[0].raw() == Move(e4,f5).raw() || phi[0].raw() == Move(e4,d5).raw());
    assert(phi[1].raw() == Move(e4,f5).raw() || phi[1].raw() == Move(e4,d5).raw());

    p->side_to_move = BLACK;
    p->colorBB[BLACK] |= SQUARE_TO_BB(a2);
    p->pieceTypeBB[PAWN] |= SQUARE_TO_BB(a2);
    p->pieceBySquare[a2] = B_PAWN;
    assert(p->pseudo_legal_pawn_moves(a2).size() == 0);

    delete p;

    Position* p2 = new Position();
    p2->side_to_move = BLACK;
    p2->colorBB[BLACK] |= SQUARE_TO_BB(e5);
    p2->colorBB[WHITE] |= SQUARE_TO_BB(e4);
    p2->pieceTypeBB[PAWN] |= SQUARE_TO_BB(e4);
    p2->pieceTypeBB[PAWN] |= SQUARE_TO_BB(e5);
    p2->pieceBySquare[e4] = W_PAWN;
    p2->pieceBySquare[e5] = B_PAWN;
    
    assert(p->pseudo_legal_pawn_moves(e5).size() == 0);

    delete p2;

  }
  static void pseudo_legal_direction_moves_test() {
    // Check propogate
    // Check we cant capture own pieces
    // Check we capture first enemy piece
    { // Capture enemy piece
      Position* p = new Position();
      // White king on e4
      p->side_to_move = WHITE;
      p->colorBB[WHITE] |= SQUARE_TO_BB(e4);
      p->pieceTypeBB[KING] |= SQUARE_TO_BB(e4);
      p->pieceBySquare[e4] = W_KING;

      // Black rook on f5
      p->colorBB[BLACK] |= SQUARE_TO_BB(f5);
      p->pieceTypeBB[ROOK] |= SQUARE_TO_BB(f5);
      p->pieceBySquare[f5] = B_ROOK;

      Move expected = Move(e4,f5);
      std::vector<Move> generated = p->pseudo_legal_direction_moves(e4,NORTH_EAST,false);
      assert(generated[0].raw() == expected.raw());
      delete p;
    }
    { // Capture enemy piece  w/ propogation -> Only capture first piece
      Position* p = new Position();
      // White king on e4
      p->side_to_move = WHITE;
      p->colorBB[WHITE] |= SQUARE_TO_BB(e4);
      p->pieceTypeBB[KING] |= SQUARE_TO_BB(e4);
      p->pieceBySquare[e4] = W_KING;
      
      // Black rook on g6
      p->colorBB[BLACK] |= SQUARE_TO_BB(g6);
      p->pieceTypeBB[ROOK] |= SQUARE_TO_BB(g6);
      p->pieceBySquare[g6] = B_ROOK;

      // Black rook on h7
      p->colorBB[BLACK] |= SQUARE_TO_BB(h7);
      p->pieceTypeBB[ROOK] |= SQUARE_TO_BB(h7);
      p->pieceBySquare[h7] = B_ROOK;

      Move g6_move = Move(e4,g6);
      Move f5_move = Move(e4,f5);
      std::vector<Move> prop_generated = p->pseudo_legal_direction_moves(e4,NORTH_EAST,true);
      std::vector<Move> no_prop_generated = p->pseudo_legal_direction_moves(e4,NORTH_EAST,false);
      assert(no_prop_generated.size() == 1);
      assert(no_prop_generated[0].raw() == f5_move.raw());
      assert(prop_generated.size() == 2);
      assert(prop_generated[0].raw() == f5_move.raw());
      assert(prop_generated[1].raw() == g6_move.raw());
      delete p;
    }
    { // CANT capture own piece
      Position* p = new Position();
      // White king on e4
      p->side_to_move = WHITE;
      p->colorBB[WHITE] |= SQUARE_TO_BB(e4);
      p->pieceTypeBB[KING] |= SQUARE_TO_BB(e4);
      p->pieceBySquare[e4] = W_KING;
      // White rook on g6
      p->colorBB[WHITE] |= SQUARE_TO_BB(g6);
      p->pieceTypeBB[ROOK] |= SQUARE_TO_BB(g6);
      p->pieceBySquare[g6] = W_ROOK;

      std::vector<Move> generated = p->pseudo_legal_direction_moves(e4,NORTH_EAST,true);
      assert(generated.size() == 1);
      assert(generated[0].raw() == Move(e4,g5).raw());

      delete p;
    }
  }


};


int main( int argc, char** argv ) {
  PositionTests::unit_tests();
  return 0;
}
