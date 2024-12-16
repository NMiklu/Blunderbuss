#include <iostream>
#include <cassert>
#include "../../src/position.h"
#include "../../src/representation.h"

const Bitboard rank_bbs[8] = {RANK_1_BB, RANK_2_BB, RANK_3_BB, RANK_4_BB,
                              RANK_5_BB, RANK_6_BB, RANK_7_BB, RANK_8_BB};
const Bitboard file_bbs[8] = {FILE_A_BB, FILE_B_BB, FILE_C_BB, FILE_D_BB,
                              FILE_E_BB, FILE_F_BB, FILE_G_BB, FILE_H_BB};
const Piece piece_types[8] = {white_p, black_p, pawn_p, rook_p, queen_p, king_p};



/* UNIT TESTS */
void position_copy_test() {}
void position_pieces_test() {}
void position_en_passant_target_test() {}
void position_piece_at_square_test() {}
void position_set_test() {
  for( int rank_idx = 0; rank_idx < 8; rank_idx++ ) {
    for( int file_idx = 0; file_idx < 8; file_idx++ ) { 
      Square sq = static_cast<Square>((rank_idx * 8) + file_idx);
      Bitboard check = rank_bbs[rank_idx] & file_bbs[file_idx];
      for( int piece_idx = 0; piece_idx < 8; piece_idx++ ) {
        Position* p = new Position();
        p->set(sq,piece_types[piece_idx]);
        assert((p->pieceBB[piece_types[piece_idx]] & check) > 0);
        delete( p );
      }
    }
  }
}
void position_remove_test() {}
void position_clear_test() {}
void position_castle_test() {}
void position_revoke_castle_test() {}
void position_halfmove_test() {}
void position_fullmove_test() {}
void position_fen_test() {}
void position_check_rep_test() {}



int main( int argc, char** argv ) {
  position_set_test();
  return 0;
}
