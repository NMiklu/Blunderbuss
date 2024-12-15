#ifndef REPRESENTATION_H
#define REPRESENTATION_H
#include <cstdint>

using Bitboard = uint64_t;

const Bitboard EMPTY_BB       = 0x0000000000000000;
const Bitboard UNIVERSE_BB    = 0xFFFFFFFFFFFFFFFF;

const Bitboard FILE_A_BB = 0x0101010101010101;
const Bitboard FILE_B_BB = 0x0202020202020202;
const Bitboard FILE_C_BB = 0x0404040404040404;
const Bitboard FILE_D_BB = 0x0808080808080808;
const Bitboard FILE_E_BB = 0x1010101010101010;
const Bitboard FILE_F_BB = 0x2020202020202020;
const Bitboard FILE_G_BB = 0x4040404040404040;
const Bitboard FILE_H_BB = 0x8080808080808080;

const Bitboard RANK_1_BB = 0x00000000000000FF;
const Bitboard RANK_2_BB = 0x000000000000FF00;
const Bitboard RANK_3_BB = 0x0000000000FF0000;
const Bitboard RANK_4_BB = 0x00000000FF000000;
const Bitboard RANK_5_BB = 0x000000FF00000000;
const Bitboard RANK_6_BB = 0x0000FF0000000000;
const Bitboard RANK_7_BB = 0x00FF000000000000;
const Bitboard RANK_8_BB = 0xFF00000000000000;

constexpr Bitboard EDGE_BB = RANK_1_BB | RANK_8_BB | FILE_A_BB | FILE_H_BB;
constexpr Bitboard A1_BB     = (RANK_1_BB & FILE_A_BB);
constexpr Bitboard H1_BB     = (RANK_1_BB & FILE_H_BB);
constexpr Bitboard A8_BB     = (RANK_8_BB & FILE_A_BB);
constexpr Bitboard H8_BB     = (RANK_8_BB & FILE_H_BB);
constexpr Bitboard CORNER_BB = A1_BB | H1_BB | A8_BB | H8_BB;

constexpr Bitboard NORTH_BB      = ~(RANK_8_BB);
constexpr Bitboard EAST_BB       = ~(FILE_H_BB);
constexpr Bitboard WEST_BB       = ~(FILE_A_BB);
constexpr Bitboard SOUTH_BB      = ~(RANK_1_BB);
constexpr Bitboard NORTH_EAST_BB = ~(RANK_8_BB & FILE_H_BB);
constexpr Bitboard NORTH_WEST_BB = ~(RANK_8_BB & FILE_A_BB);
constexpr Bitboard SOUTH_EAST_BB = ~(RANK_1_BB & FILE_H_BB);
constexpr Bitboard SOUTH_WEST_BB = ~(RANK_1_BB & FILE_A_BB);
constexpr Bitboard NORTH_NORTH_EAST_BB = ~(RANK_8_BB & RANK_7_BB & FILE_H_BB);
constexpr Bitboard NORTH_NORTH_WEST_BB = ~(RANK_8_BB & RANK_7_BB & FILE_A_BB);
constexpr Bitboard NORTH_EAST_EAST_BB = ~(RANK_8_BB & FILE_G_BB & FILE_H_BB);
constexpr Bitboard SOUTH_EAST_EAST_BB = ~(RANK_1_BB & FILE_G_BB & FILE_H_BB);
constexpr Bitboard SOUTH_SOUTH_EAST_BB = ~(RANK_1_BB & RANK_2_BB & FILE_H_BB);
constexpr Bitboard SOUTH_SOUTH_WEST_BB = ~(RANK_1_BB & RANK_2_BB & FILE_A_BB);
constexpr Bitboard SOUTH_WEST_WEST_BB = ~(RANK_1_BB & FILE_A_BB & FILE_B_BB);
constexpr Bitboard NORTH_WEST_WEST_BB = ~(RANK_8_BB & FILE_A_BB & FILE_B_BB);



enum Square : int {
  // Little Endian rank-file
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8,
  NO_SQUARE
};
enum Compass : int {
  NORTH = 8,
  SOUTH = -8,
  EAST  = 1,
  WEST  = -1,
  NORTH_WEST = NORTH + WEST,
  NORTH_EAST = NORTH + EAST,
  SOUTH_WEST = SOUTH + WEST,
  SOUTH_EAST = SOUTH + EAST,
  
  /*Knight Moves*/
  NORTH_NORTH_EAST = NORTH + NORTH_EAST,
  NORTH_NORTH_WEST = NORTH + NORTH_WEST,

  NORTH_EAST_EAST  = NORTH_EAST + EAST,
  SOUTH_EAST_EAST  = SOUTH_EAST + EAST,

  SOUTH_SOUTH_EAST = SOUTH + SOUTH_EAST,
  SOUTH_SOUTH_WEST = SOUTH + SOUTH_WEST,

  SOUTH_WEST_WEST  = SOUTH_WEST + WEST,
  NORTH_WEST_WEST  = NORTH_WEST + WEST
};

enum Piece {
  white_p,
  black_p,
  pawn_p,
  knight_p,
  bishop_p,
  rook_p,
  queen_p,
  king_p,
  none_p
};

enum CastleMask : uint8_t {
  white_short_castle  = 0b1000,
  white_long_castle   = 0b0100,
  black_short_castle  = 0b0010,
  black_long_castle   = 0b0001
};

struct Move {
  typedef enum {
    INITIAL_SQUARE = 0xFC00,
    FINAL_SQUARE   = 0x03F0,
    SPECIAL_FLAG   = 0x000F // Promote, Castle, En Passant
  }MoveMask;
  typedef enum {
    /*
      NO FLAG      = 0000 0000
      Flag on      = 0000 1000
      Promo knight = 0000 1001
      Promo bishop = 0000 1010
      promo rook   = 0000 1011
      promo queen  = 0000 1100
      long castle  = 0000 1101
      short castle = 0000 1110
      en_passant   = 0000 1111
    */
    NO_FLAG        = 0x00,
    PROMOTE_KNIGHT = 0b00001001,
    PROMOTE_BISHOP = 0b00001010,
    PROMOTE_ROOK   = 0b00001011,
    PROMOTE_QUEEN  = 0b00001100,
    LONG_CASTLE    = 0b00001101,
    SHORT_CASTLE   = 0b00001110,
    EN_PASSANT     = 0b00001111
  }MoveFlag;

  Move(enum Square init, enum Square final, MoveFlag flag);
  Move()  = default;
  ~Move() = default;

  enum Square inital_square();
  enum Square final_square();
  MoveFlag flag();

  uint16_t     _move_data;
};


inline Bitboard SQUARE_TO_BB(Square sq) {
  return 1ULL << sq;
}


#endif