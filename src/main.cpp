#include <iostream>
#include <cassert>
#include "representation.h"
#include "position.h"

bool is_big_endian() 
{
  union {
    uint32_t i;
    char c[4];
  } bint = {0x01020304};
  return bint.c[0] == 1;
}

void test_square_rep() {
  Bitboard rankBB[8];
  rankBB[0] = RANK_1_BB;
  rankBB[1] = RANK_2_BB;
  rankBB[2] = RANK_3_BB;
  rankBB[3] = RANK_4_BB;
  rankBB[4] = RANK_5_BB;
  rankBB[5] = RANK_6_BB;
  rankBB[6] = RANK_7_BB;
  rankBB[7] = RANK_8_BB;
  Bitboard fileBB[8];
  fileBB[0] = FILE_A_BB;
  fileBB[1] = FILE_B_BB;
  fileBB[2] = FILE_C_BB;
  fileBB[3] = FILE_D_BB;
  fileBB[4] = FILE_E_BB;
  fileBB[5] = FILE_F_BB;
  fileBB[6] = FILE_G_BB;
  fileBB[7] = FILE_H_BB;
  for( int i = 0; i < 64; i++) {
    int rank = i/8;
    int file = i%8;
    Bitboard mask = fileBB[file] & rankBB[rank];
    assert((SQUARE_TO_BB(static_cast<Square>(i)) & mask) != 0);
  }
  
}

void test_insert() {

  for( int rank=0;rank<8;rank++ ) {
    for( int file=0;file<8;file++) {
      Position lol;
      lol.clear();
      Square square = static_cast<Square>((rank*8) + file);
      for( int i = 2; i < 8; i++) {
        Piece piece = static_cast<Piece>(i);
        for( int j = 0; j < 2; j++) {
          Piece color = static_cast<Piece>(j);
          lol.set(square,color);
          lol.set(square,piece);
          Bitboard color_and_piece_bb = lol.pieces(color) & lol.pieces(piece);
          assert( color_and_piece_bb != 0 );
          assert( ( SQUARE_TO_BB(square) & lol.pieces(piece)) != 0); 
          assert( ( SQUARE_TO_BB(square) & lol.pieces(color)) != 0);
          lol.clear();
        }
      }
    }
  }

}


int main( int argc, char** argv ) 
{
  test_square_rep();
  Position lol;
  lol.clear();
  lol.set( e4, white_p);
  lol.set( e4, pawn_p);
  lol.pretty(std::cout);
  test_insert();

  lol.clear();
  if (lol.fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"))
    std::cout << "WOAH!!!" << std::endl;
  if( lol.check_rep() )
    std::cout << "check_rep success" << std::endl;
  lol.pretty(std::cout);



  return 0;
}