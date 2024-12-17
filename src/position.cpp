#include "representation.h"
#include "position.h"
#include "misc.h"
#include <string>
#include <sstream>
#include <cmath>
#include <cassert>

#include <iostream> //DEBUG REMOVE LATER



PieceType Position::type( Piece p ) {
  if( p == PIECE_BOUND || p == NO_PIECE ) return NO_TYPE;
  int tau = static_cast<int>(p) - 6; // Remove color
  return static_cast<PieceType>(tau);
}

Color Position::color( Piece p ) {
  if( p == PIECE_BOUND || p == NO_PIECE ) return NO_COLOR;
  return (static_cast<int>(p) % 5) ? BLACK:WHITE;
}


Bitboard Position::pieces( Piece x ) const {
  /* 
    Returns 64 bit int-type representing 
    the pieces specified by Piece <x>
   */
  PieceType t = Position::type(x);
  Color     c = Position::color(x);
  assert(t != NO_TYPE);
  return colorBB[c] & pieceTypeBB[t];
}

Bitboard Position::pieces(PieceType x) const {
  assert(x != NO_TYPE);
  return pieceTypeBB[x];
}

Bitboard Position::pieces(Color x) const {
  assert( x != NO_COLOR );
  return colorBB[x];
}

Piece Position::piece_on(Square sq) const {
  assert(sq != NO_SQUARE);
  return pieceBySquare[static_cast<int>(sq)];
}


void Position::pretty( std::ostream& os ) const {
  /*
    Outputs ASCII chess board to ostream <os>
  */
  char board[64][3];
  for( int i = 0; i < 64; i++) {
    Square square = static_cast<Square>(i);
    Piece piece_on_sq = this->piece_on(square);
    char color_char = ' ';
    char piece_char = ' ';
    if( piece_on_sq != NO_PIECE ) {
      color_char = (Position::color(piece_on_sq) == WHITE) ? 'W':'B';
      switch(Position::type(piece_on_sq)) {
        case PAWN:
          piece_char = 'p';
          break;
        case KNIGHT:
          piece_char = 'n';
          break;
        case BISHOP:
          piece_char = 'b';
          break;
        case ROOK:
          piece_char = 'r';
          break;
        case QUEEN:
          piece_char = 'q';
          break;
        case KING:
          piece_char = 'k';
          break;
        default:
          // ERROR?
          std::cerr << "Position::pretty() type switch statement failed" << std::endl;
          exit(1);
          break;
      }
    }
    board[i][0] = color_char;
    board[i][1] = piece_char;
    board[i][2] = '\0';
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
bool Position::put( Square sq, Piece p ) {
  /*
    Overwrites piece <p> onto the given square <sq>.
    returns true if successful
    false otherwise
  */
  PieceType t = Position::type(p);
  Color c = Position::color(p);
  if( t == NO_TYPE || sq == NO_SQUARE ) return false;

  Bitboard sq_bb = SQUARE_TO_BB(sq);
  this->pieceTypeBB[t] &= sq_bb;
  this->colorBB[c] &= sq_bb;
  this->pieceBySquare[static_cast<int>(sq)] = p;
  
  return true;
}
void Position::remove(Square sq) {
  /* clears Square <sq> from any pieces*/
  if( sq == NO_SQUARE ) return;
  Piece p = this->pieceBySquare[sq];
  if( p != NO_PIECE) {
    Bitboard inv_sq_bb = ~SQUARE_TO_BB(sq);
    PieceType t = Position::type(p);
    Color c = Position::color(p);

    this->pieceTypeBB[t] &= inv_sq_bb;
    this->colorBB[c] &= inv_sq_bb;
    this->pieceBySquare[sq] = NO_PIECE;
  }
}
void Position::add_castle_right(CastleRight right) {
  /*
    Add the right to castle specified by <right> to the
    position... Refer to representation.h for CastleRight enum
  */
  this->castleRightMask |= right;
}
bool Position::has_castle_right( CastleRight right ) {
  /*
    Returns true if castle right specified by <right>
    is true in 'this' position.
    false otherwise
  */
  return ((this->castleRightMask & right) != 0);
}
void Position::revoke_castle_right(CastleRight right) {
  /* Revokes the castle rights in 'this' position
    specified by <right>
  */
  this->castleRightMask &= (~right);
}

void Position::inc_halfmove() {
  // Increments halfmove clock
  this->half_move_clock+=1;
}
void Position::inc_fullmove() {
  // Increments fullmove clock
  this->full_move_clock+=1;
}
uint8_t Position::halfmove() const {
  /* returns the value of the halfmove clock*/
  return (this->half_move_clock);
}
uint16_t Position::fullmove() const {
  /* returns the value of the fullmove clock*/
  return (this->full_move_clock);
}


bool Position::fen(std::string fen_string ) {
  /* TODO UPDATE !!!!*/ 
  /* 
    Sets 'this' object to represent the chess position
    specified by <fen_string> according to 
    Forsyth-Edwards Notation
    https://www.chessprogramming.org/Forsyth-Edwards_Notation
    Returns true if representation is updated.
    False if the fen is invalid.
    WARNING -> does NOT revert changes to 'this' that were 
                manipulated before failure.
    <FEN> ::= <Piece Placement>
          ' ' <Side to move>
          ' ' <Castling ability>
          ' ' <En passant target square>
          ' ' <Halfmove clock>
          ' ' <Fullmove counter>

    <Piece Placement> ::=
        <rank8>'/'<rank7>'/'<rank6>'/'<rank5>'/'
        <rank4>'/'<rank3>'/'<rank2>'/'<rank1>
    <ranki>       ::= [<digit17>]<piece> {[<digit17>]<piece>} [<digit17>] | '8'
    <piece>       ::= <white piece> | <black piece>
    <digit17>     ::= '1' | '2' | '3' | '4' | '5' | '6' | '7'
    <white piece> ::= 'P' | 'N' | 'B' | 'R' | 'Q' | 'K' 
    <black piece> ::= 'p' | 'n' | 'b' | 'r' | 'q' | 'k' 

    <Side to move> ::= {'w' | 'b'}

    <Castling ability> ::= '-' | ['K'] | ['Q'] | ['k'] | ['q'] (1..4)

    <En passant target square> ::= '-' | <epsquare>
    <epsquare>   ::= <fileLetter><eprank>
    <fileLetter> ::= 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h'
    <eprank>     ::= '3' | '6'

    <Halfmove Clock> ::= <digit> {<digit>}
    <digit> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

    <Fullmove counter> ::= <digit19> {<digit>}
    <digit19>          ::= '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
    <digit>            ::= '0' | <digit19>
  */
  return false;
}
void Position::reset() {
  /*
    returns 'this' position back to
    the default chess starting position
  */
  for( int i = 0; i < static_cast<int>(COLOR_BOUND); i++) {
    this->colorBB[i] = 0ULL;
  }
  for( int i = 0; i < static_cast<int>(PIECE_TYPE_BOUND); i++) {
    this->pieceTypeBB[i] = 0ULL;
  }
  for( int i = 0; i < 64; i++ ) {
    this->pieceBySquare[i] = NO_PIECE;
  }
  this->en_passant_target_square = NO_SQUARE;
  this->side_to_move = WHITE;
  this->castleRightMask = (WHITE_SHORT_CASTLE | WHITE_LONG_CASTLE | BLACK_SHORT_CASTLE | BLACK_LONG_CASTLE);
  this->half_move_clock = 0;
  this->full_move_clock = 1;
}
bool Position::_VALID_REP() const {
  /*
    Returns true if representation is valid
    false otherwise.

    rep invariant:
  pieceBB[white_p] & pieceBB[black_p] == 0
  && forall (i,j):[pawn_p,...,king_p] (where i != j): pieceBB[i] & pieceBB[j] == 0
  && forall i:[pawn_p,...,king_p] (!(pieceBB[i] & pieceBB[white_p] == 0 && pieceBB[i] & pieceBB[black_p] == 0))
  && castle_ability < 16 && half_move_clock <= 50 && full_move_clock > 0
  && en_passant_target_square: ([16,23] || [40,47] || 64)
  && (side_to_move == black_p || side_to_move == white_p)
  && forall i:[0,...,63] (inclusive) within pieceBySquare[i] != PIECE_BOUND
  */

  // There are white and black pieces on top of each other!
  if( (colorBB[WHITE] & colorBB[BLACK]) != 0 ) return false;

  for( int i = 0; i < PIECE_TYPE_BOUND; i++ ) {
    for( int j = i+1; j < PIECE_TYPE_BOUND; j++ ) {
      if((pieceTypeBB[i] & pieceTypeBB[j]) != 0)
        // Multiple piece types on a single square
        return false;
    }
  }

  // piece by square should not have PIECE_BOUND enum
  for( int i = 0; i < 64; i++ ) {
    if( this->pieceBySquare[i] == PIECE_BOUND ) return false;
  }

  if( this->castleRightMask >= 0b1111 ) // Should never be greater than the mask (4 bits)
    // Invalid castle value
    return false;
  if( this->half_move_clock > Position::MAX_HALF_MOVE )
    //Impossible half_move_clock
    return false;
  if( this->full_move_clock == 0 )
    // Invalid move clock
    return false;
  int sv = static_cast<int>(this->en_passant_target_square);
  if( !((sv >= 16 && sv <= 23) || (sv >= 40 || sv <= 47) || (sv == 64)) )
    // Invalid en passant square
    return false;
  if( !((this->side_to_move == WHITE)||(this->side_to_move == BLACK)))
    // Invalid piece-type designated side to move
    return false;

  return true;
}

Square Position::en_passant_target() const {
  return this->en_passant_target_square;
}

Position* Position::copy(const Position& pos) {
  Position* copy_ = new Position();

  for( int i = 0; i < static_cast<int>(COLOR_BOUND); i++ ) {
    copy_->colorBB[i] = pos.colorBB[i];
  }
  for( int i = 0; i < static_cast<int>(PIECE_TYPE_BOUND); i++ ) {
    copy_->pieceTypeBB[i] = pos.pieceTypeBB[i];
  }
  for( int i = 0; i < 64; i++ ) {
    copy_->pieceBySquare[i] = pos.pieceBySquare[i];
  }
  copy_->en_passant_target_square = pos.en_passant_target_square;
  copy_->castleRightMask = pos.castleRightMask;
  copy_->half_move_clock = pos.half_move_clock;
  copy_->full_move_clock = pos.full_move_clock;
  return copy_;
}
