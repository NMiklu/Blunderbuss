#include "representation.h"
#include "position.h"

Bitboard Position::pieces( PieceEnum x ) const {
  /* 
    Returns 64 bit int-type representing 
    the pieces specified by PieceEnum <x>
   */
  return pieceBB[x];
}
void Position::pretty( std::ostream& os ) const {
  /*
    Outputs ASCII chess board to ostream <os>
  */
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
  /*
    Overwrites <piece> and <color> onto the given square <sq>.
    Returns True if inputs were valid and the representation
    was updated. False otherwise.
  */
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
  /*
    Sets 'this' object to represent the chess position
    specified by <fen_string> according to 
    Forsyth-Edwards Notation
    https://www.chessprogramming.org/Forsyth-Edwards_Notation
    Returns true if representation is updated.
    False otherwise.
  */
  return false;
}
bool Position::move( LERF_Square from, LERF_Square to ) {
  /*
    Changes 'this' object to represent a move <from> -> <to>
    on the chess board. Returns true if successful. False otherwise.
  */
  // TODO
  return false;
}
bool Position::valid_position() const {
  /*
    Returns true if 'this' object represents
    a valid chess position. False otherwise.
  */
  // TODO
  return false;
}
void Position::clear() {
  /*
    Clears 'this' object of any pieces
    that may be represented.
    Also resets castling rights to default (All valid)
  */
  for( int i = white_p; i <= king_p; i++ ) {
    this->pieceBB[i] = EMPTY_BB;
  }
  this->castle |= (white_short_castle);
  this->castle |= (white_long_castle);
  this->castle |= (black_short_castle);
  this->castle |= (black_long_castle);
}
