#include "representation.h"
#include "position.h"

Bitboard Position::pieces( PieceEnum x ) const {
  return pieceBB[x];
}
void Position::pretty( std::ostream& os ) const {
  char board[64][3];
  for( int i = 0; i < 64; i++) {
    LERF_Square square = static_cast<LERF_Square>(i);
    if( this->pieceBB[0] & LERF_SQUARE_TO_BB(square)) {
      board[i][0] = 'W';
      board[i][2] = '\0';
    }
    else if( this->pieceBB[1] & LERF_SQUARE_TO_BB(square)) {
      board[i][0] = 'B';
      board[i][2] = '\0';
    } else {
      board[i][0] = ' ';
      board[i][1] = ' ';
      board[i][2] = '\0';
      continue;
    }

    for( int j = 2; j<8; j++) {
      if( this->pieceBB[j] & LERF_SQUARE_TO_BB(square)) {
        switch(j) {
          case pawn_p:
            board[i][1] = 'p';
            break;
          case knight_p:
            board[i][1] = 'n';
            break;
          case bishop_p:
            board[i][1] = 'b';
            break;
          case rook_p:
            board[i][1] = 'r';
            break;
          case queen_p:
            board[i][1] = 'q';
            break;
          case king_p:
            board[i][1] = 'k';
            break;
          default:
            // idk lmao
            break;
        }
      }
    }
  }

  os << "   A  B  C  D  E  F  G  H  " << std::endl;
  os << "  +--+--+--+--+--+--+--+--+" << std::endl;
  for( int i = 56; (i < 64 && i >= 0); i++) {
    bool carriage_return = (i%8 == 7);
    bool start_of_rank = (i%8 == 0);
    if( start_of_rank ) os << (i/8)+1 << " |";
    os << board[i] << "|";
    if( carriage_return ) {
      i-=16;
      os << std::endl;
      os << "  +--+--+--+--+--+--+--+--+" << std::endl;
    }
  }
}
bool Position::insert( LERF_Square sq, PieceEnum color, PieceEnum piece ) {
  if( !(color == white_p || color == black_p) ) {
    return false;
  }
  if( (piece == white_p || piece == black_p) ) {
    return false;
  }
  Bitboard squareBB = LERF_SQUARE_TO_BB(sq);
  this->pieceBB[color] |= squareBB;
  this->pieceBB[piece] |= squareBB;
  return true;
}
bool Position::fen(std::string fen_string ) {
  // TODO
  return false;
}
void Position::clear() {
  for( int i = white_p; i <= king_p; i++ ) {
    this->pieceBB[i] = EMPTY_BB;
  }
  this->castle |= (white_short_castle);
  this->castle |= (white_long_castle);
  this->castle |= (black_short_castle);
  this->castle |= (black_long_castle);
}
