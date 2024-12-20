#include "representation.h"
#include "position.h"
#include "misc.h"
#include <string>
#include <sstream>
#include <cmath>
#include <cstring> //memset


Position::Position() {
  memset(this->pieceBySquare,NO_PIECE,Position::SQUARE_LIMIT*sizeof(Piece));
}

PieceType Position::type( Piece p ) {
  if( p == PIECE_BOUND || p == NO_PIECE ) return NO_TYPE;
  int tau = static_cast<int>(p); 
  tau %= 6; // Remove color
  return static_cast<PieceType>(tau);
}

Color Position::color( Piece p ) {
  if( p == PIECE_BOUND || p == NO_PIECE ) return NO_COLOR;
  return (static_cast<int>(p) >= 6) ? BLACK:WHITE;
}


Bitboard Position::pieces( Piece x ) const {
  /* 
    Returns 64 bit int-type representing 
    the pieces specified by Piece <x>
   */
  PieceType t = Position::type(x);
  Color     c = Position::color(x);
  if( t == NO_TYPE || c == NO_COLOR ) return EMPTY_BB;
  return colorBB[c] & pieceTypeBB[t];
}

Bitboard Position::pieces(PieceType x) const {
  if( x == PIECE_TYPE_BOUND || x == NO_TYPE) return EMPTY_BB;
  return pieceTypeBB[x];
}

Bitboard Position::pieces(Color x) const {
  if( x == COLOR_BOUND || x == NO_COLOR) return EMPTY_BB;
  return colorBB[x];
}

Color Position::to_attack() const {
  /* Returns the side to move in 'this' position */
  return this->side_to_move;
}
Color Position::to_defend() const {
  /* Returns the opposite side of the current side to move */
  return (this->side_to_move == WHITE) ? BLACK:WHITE;
}

Piece Position::piece_on(Square sq) const {
  if( sq == NO_SQUARE ) return NO_PIECE;
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
  this->pieceTypeBB[t] |= sq_bb;
  this->colorBB[c] |= sq_bb;
  this->pieceBySquare[sq] = p;
  
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

    <FEN> ::= <Piece Placement>
          ' ' <Side to move>
          ' ' <Castling ability>
          ' ' <En passant target square>
          ' ' <Halfmove clock>
          ' ' <Fullmove counter>
  */
  /*
    <Piece Placement> ::=
        <rank8>'/'<rank7>'/'<rank6>'/'<rank5>'/'
        <rank4>'/'<rank3>'/'<rank2>'/'<rank1>
    <ranki>       ::= [<digit17>]<piece> {[<digit17>]<piece>} [<digit17>] | '8'
    <piece>       ::= <white piece> | <black piece>
    <digit17>     ::= '1' | '2' | '3' | '4' | '5' | '6' | '7'
    <white piece> ::= 'P' | 'N' | 'B' | 'R' | 'Q' | 'K' 
    <black piece> ::= 'p' | 'n' | 'b' | 'r' | 'q' | 'k' 
  */
  /*
    <Side to move> ::= {'w' | 'b'}
  */
  /*
    <Castling ability> ::= '-' | ['K'] | ['Q'] | ['k'] | ['q'] (1..4)
  */
  /*
    <En passant target square> ::= '-' | <epsquare>
    <epsquare>   ::= <fileLetter><eprank>
    <fileLetter> ::= 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h'
    <eprank>     ::= '3' | '6'
  */
  /*
    <Halfmove Clock> ::= <digit> {<digit>}
    <digit> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
  */
  /*
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

  memset(this->pieceBySquare,NO_PIECE,Position::SQUARE_LIMIT*sizeof(Piece));
  memset(this->pieceTypeBB,0ULL,PIECE_TYPE_BOUND*sizeof(Bitboard));
  memset(this->colorBB,0ULL,COLOR_BOUND*sizeof(Bitboard));
  this->put(a2,W_PAWN);
  this->put(b2,W_PAWN);
  this->put(c2,W_PAWN);
  this->put(d2,W_PAWN);
  this->put(e2,W_PAWN);
  this->put(f2,W_PAWN);
  this->put(g2,W_PAWN);
  this->put(h2,W_PAWN);
  this->put(a1,W_ROOK);
  this->put(h1,W_ROOK);
  this->put(b1,W_KNIGHT);
  this->put(g1,W_KNIGHT);
  this->put(c1,W_BISHOP);
  this->put(f1,W_BISHOP);
  this->put(e1,W_KING);
  this->put(d1,W_QUEEN);

  this->put(a7,B_PAWN);
  this->put(b7,B_PAWN);
  this->put(c7,B_PAWN);
  this->put(d7,B_PAWN);
  this->put(e7,B_PAWN);
  this->put(f7,B_PAWN);
  this->put(g7,B_PAWN);
  this->put(h7,B_PAWN);
  this->put(a8,B_ROOK);
  this->put(h8,B_ROOK);
  this->put(b8,B_KNIGHT);
  this->put(g8,B_KNIGHT);
  this->put(c8,B_BISHOP);
  this->put(f8,B_BISHOP);
  this->put(e8,B_KING);
  this->put(d8,B_QUEEN);

  this->en_passant_target_square = NO_SQUARE;
  this->side_to_move = WHITE;
  this->castleRightMask = (WHITE_SHORT_CASTLE | WHITE_LONG_CASTLE | BLACK_SHORT_CASTLE | BLACK_LONG_CASTLE);
  this->half_move_clock = 0;
  this->full_move_clock = 0;
}
bool Position::_VALID_REP() const {
  /*
    Returns true if representation is valid
    false otherwise.

    rep invariant:
  (colorBB[WHITE] & colorBB[BLACK]) == 0
  && forall i,j:[PAWN,ROOK,BISHOP,KNIGHT,KING,QUEEN] where i!=j (pieceTypeBB[i] & pieceTypeBB[j] == 0)
  && castle_ability < 16 
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



bool Position::move_direction_before_edge(Square sq, Compass dir) {
  /* Returns true if the move direction specified by <dir> 
      does not reach past the edge of the board from 
      the square specified by <sq>.
      false otherwise.
  */
  if( sq == NO_SQUARE ) return false;
  return Position::move_direction_before_edge(SQUARE_TO_BB(sq),dir);
};

bool Position::move_direction_before_edge(Bitboard sq_bb, Compass dir) {
  /* Returns true if the move direction specified by <dir> 
      does not reach past the edge of the board from 
      the square specified by <sq_bb>.
      false otherwise.
  */
  if( sq_bb == EMPTY_BB ) return false;
  switch(dir) {
    case NORTH:
      return ((sq_bb & NORTH_BB) != 0);
    case SOUTH:
      return ((sq_bb & SOUTH_BB) != 0);
    case EAST:
      return ((sq_bb & EAST_BB) != 0);
    case WEST:
      return ((sq_bb & WEST_BB) != 0);
    case NORTH_WEST:
      return ((sq_bb & NORTH_WEST_BB) != 0);
    case NORTH_EAST:
      return ((sq_bb & NORTH_EAST_BB) != 0);
    case SOUTH_WEST:
      return ((sq_bb & SOUTH_WEST_BB) != 0);
    case SOUTH_EAST:
      return ((sq_bb & SOUTH_EAST_BB) != 0);
    case NORTH_NORTH_EAST:
      return ((sq_bb & NORTH_NORTH_EAST_BB) != 0);
    case NORTH_NORTH_WEST:
      return ((sq_bb & NORTH_NORTH_WEST_BB) != 0);
    case NORTH_EAST_EAST:
      return ((sq_bb & NORTH_EAST_EAST_BB) != 0);
    case SOUTH_EAST_EAST:
      return ((sq_bb & SOUTH_EAST_EAST_BB) != 0);
    case SOUTH_SOUTH_EAST:
      return ((sq_bb & SOUTH_SOUTH_EAST_BB) != 0);
    case SOUTH_SOUTH_WEST:
      return ((sq_bb & SOUTH_SOUTH_WEST_BB) != 0);
    case SOUTH_WEST_WEST:
      return ((sq_bb & SOUTH_WEST_WEST_BB) != 0);
    case NORTH_WEST_WEST:
      return ((sq_bb & NORTH_WEST_WEST_BB) != 0);
    default:
      return false;
  }
  return false;
};


Bitboard Position::pseudo_legal_normal_move_bb(Square sq) const {
  /*
    Creates a bitboard of pseudo legal squares
    that the piece on the square could move to.
     -> Does not check if move puts own king in check
     -> Only accounts for simple moves, i.e.
          NOT Promotion, Castling, En Passant...
     -> Assumes that the piece on the square <sq> is 
         the current side to move i.e. <this->to_attack()>
  */
  Piece pieceAtSq = this->piece_on(sq);
  Bitboard move_pattern = EMPTY_BB;
  Compass dirs[16] = {NORTH,EAST,SOUTH,WEST,NORTH_EAST,NORTH_WEST,SOUTH_EAST,SOUTH_WEST,
                      NORTH_NORTH_EAST, NORTH_NORTH_WEST,
                      NORTH_EAST_EAST, SOUTH_EAST_EAST,
                      SOUTH_SOUTH_EAST, SOUTH_SOUTH_WEST,
                      SOUTH_WEST_WEST, NORTH_WEST_WEST};
  switch(Position::type(pieceAtSq)) {
    case PAWN:
      move_pattern |= Position::pseudo_legal_pawn_move_bb(sq);
      break;
    case KNIGHT:
      for( int i = 8; i < 16; i++ )
        move_pattern |= Position::pseudo_legal_direction_move_bb(sq,dirs[i],false);
      break;
    case BISHOP:
      for( int i = 4; i < 8; i++ )
        move_pattern |= Position::pseudo_legal_direction_move_bb(sq,dirs[i],true);
      break;
    case ROOK:
      for( int i = 0; i < 4; i++ )
        move_pattern |= Position::pseudo_legal_direction_move_bb(sq,dirs[i],true);
      break;
    case QUEEN:
      for( int i = 0; i < 8; i++ )
        move_pattern |= Position::pseudo_legal_direction_move_bb(sq,dirs[i],true);
      break;
    case KING:
      for( int i = 0; i < 8; i++ )
        move_pattern |= Position::pseudo_legal_direction_move_bb(sq,dirs[i],false);
      break;
    default:
      // pieceAtSq == NO_PIECE
      break;
  }
  return move_pattern;
}

Bitboard Position::pseudo_legal_pawn_move_bb( Square sq ) const {
  /*
    Generates bitboard of 'normal' moves for a pawn on square <sq>
    such that the pawn is on the side of <this->to_attack()> i.e. the side
    to move.
  */

  Bitboard move_pattern = EMPTY_BB;
  Bitboard sq_bb = SQUARE_TO_BB(sq);
  Bitboard to_promote_check = (this->to_attack() == WHITE ) ? RANK_7_BB:RANK_2_BB;
  Compass pawn_move_dir = (this->to_attack() == WHITE ) ? NORTH:SOUTH;
  Compass pawn_attack_east = static_cast<Compass>(pawn_move_dir + EAST);
  Compass pawn_attack_west = static_cast<Compass>(pawn_move_dir + WEST);

  // Move forward
  if( (sq_bb & to_promote_check) == 0 ) {
    // Not a promotion, therefore still counts as a 'normal' move
    move_pattern |= (SQUARE_TO_BB(static_cast<Square>(sq+pawn_move_dir)));
  }

  if( Position::move_direction_before_edge(sq, pawn_attack_east)) {
    Bitboard attack_east_bb = SQUARE_TO_BB(static_cast<Square>(sq+pawn_attack_east));
    Bitboard opponent_pieces = this->pieces(this->to_defend());
    if( opponent_pieces & attack_east_bb ) {
      // Pawn is attacking enemy piece!
      move_pattern |= attack_east_bb;
    }
  }

  if( Position::move_direction_before_edge(sq, pawn_attack_west)) {
    Bitboard attack_west_bb = SQUARE_TO_BB(static_cast<Square>(sq+pawn_attack_west));
    Bitboard opponent_pieces = this->pieces(this->to_defend());
    if( opponent_pieces & attack_west_bb ) {
      // Pawn is attacking enemy piece!
      move_pattern |= attack_west_bb;
    }
  }

  return move_pattern;
}

Bitboard Position::pseudo_legal_direction_move_bb(Square sq, Compass dir, bool propagate) const {
  /*
    Creates a bitboard of pseudo legal move squares given the direction
    from the square specified by <sq> where the direction is propagated
    until a edge is hit. 
    (only propogates the direction if <propagate> is true
     else it'll only move in that direction once)
    -> Does not check if move will put own king in check.
    -> Does not include moves that capture or move over
        <side_to_move> pieces
    -> INCLUDES the 'first' capture on a given direction...
    NOTE: Does not include the square <sq> in the op:
    +---+---+---+---+---+    +---+---+---+---+---+
    | p |   |   |   |   |    | * |   |   |   |   |
    +---+---+---+---+---+    +---+---+---+---+---+
    |   |   |   |   |   |    |   | * |   |   |   |
    +---+---+---+---+---+    +---+---+---+---+---+
    |   |   | B |   |   | -> |   |   |   |   |   |
    +---+---+---+---+---+    +---+---+---+---+---+
    |   |   |   |   |   |    |   |   |   |   |   |
    +---+---+---+---+---+    +---+---+---+---+---+
    |   |   |   |   |   |    |   |   |   |   |   |
    +---+---+---+---+---+    +---+---+---+---+---+
    where: B = White Bishop
           p = Black pawn

    +---+---+---+---+---+    +---+---+---+---+---+
    |   |   |   |   |   |    |   |   |   |   |   |
    +---+---+---+---+---+    +---+---+---+---+---+
    |   |   |   |   |   |    |   |   |   |   |   |
    +---+---+---+---+---+    +---+---+---+---+---+
    |   |   | B |   |   | -> |   |   |   |   |   |
    +---+---+---+---+---+    +---+---+---+---+---+
    |   |   |   |   |   |    |   | * |   |   |   |
    +---+---+---+---+---+    +---+---+---+---+---+
    | K |   |   |   |   |    |   |   |   |   |   |
    +---+---+---+---+---+    +---+---+---+---+---+
    where: B = White Bishop
           K = White Knight 
  */
  Bitboard move_pattern = EMPTY_BB;
  Bitboard self_pieces = this->pieces(this->to_attack());
  Bitboard opponent_pieces = this->pieces(this->to_defend());
  if( Position::move_direction_before_edge(sq, dir)) {
    Square next_square = static_cast<Square>(sq+dir);
    do {
      Bitboard next_square_bb = SQUARE_TO_BB(next_square);
      if( next_square_bb & self_pieces ) return move_pattern;
      move_pattern |= next_square_bb;
      if( next_square_bb & opponent_pieces ) return move_pattern;
      next_square = static_cast<Square>(next_square + dir);
    } while(Position::move_direction_before_edge(next_square,dir) && propagate);
  }
  return move_pattern;
}