#include "representation.h"
#include "position.h"
#include <string>
#include <sstream>
#include <cmath>
#include <cstring> //memset
#include <vector>
#include <cstdint>


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
Piece Position::to_piece(Color c, PieceType t) {
  if( t == PIECE_TYPE_BOUND || t == NO_TYPE ) return NO_PIECE;
  if( c == NO_COLOR || c == COLOR_BOUND ) return NO_PIECE;
  uint8_t enum_offset = (c == WHITE ? 0 : 6);
  return static_cast<Piece>(t + enum_offset);
}
Square Position::to_square(Bitboard single_pop_bb) {
  /*
    Converts a Singly Populated Bitboard to a Square

    if <single_pop_bb> == 0 returns NO_SQUARE
    else returns Square closest to 'bottom-left'
    of board.
  */
  if( single_pop_bb == 0 ) return NO_SQUARE;
  int rank_idx = 0;
  int file_idx = 0;
  while( (single_pop_bb & RANK_1_BB) == 0 ) {
    single_pop_bb = single_pop_bb >> 8;
    rank_idx++;
  }
  while( (single_pop_bb & FILE_A_BB) == 0 ) {
    single_pop_bb = single_pop_bb >> 1;
    file_idx++;
  }
  return static_cast<Square>((8*rank_idx + file_idx));
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
  return this->pieceBySquare[sq];
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
  if( p == NO_PIECE ||t == NO_TYPE || sq == NO_SQUARE ) return false;

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
bool Position::has_castle_right( CastleRight right ) const {
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
  && castleRightMask < 16 
  && en_passant_target_square: ([16,23] || [40,47] || 64)
  && (side_to_move == black_p || side_to_move == white_p)
  && forall i:[0,...,63] (inclusive) within pieceBySquare[i] != PIECE_BOUND
  && ((pieceTypeBB[PAWN] & colorBB[WHITE]) & RANK_8_BB) == 0
  && ((pieceTypeBB[PAWN] & colorBB[BLACK]) & RANK_1_BB) == 0
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

  if(((pieceTypeBB[PAWN] & colorBB[WHITE]) & RANK_8_BB) != 0 )
    // White pawn on the 8th rank
    return false;

  if(((pieceTypeBB[PAWN] & colorBB[BLACK]) & RANK_1_BB) != 0 )
    // Black pawn on the 1st rank
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
  copy_->side_to_move = pos.side_to_move;
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
}

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


std::vector<Move> Position::pseudo_legal_normal_moves(Square sq) const {
  /*
    Creates a vector of pseudo legal moves
    that the piece on the square could move to.
     -> Does not check if move puts own king in check
     -> Only accounts for simple moves, i.e.
          NOT Promotion, Castling, En Passant...
     -> Assumes that the piece on the square <sq> is 
         the current side to move i.e. <this->to_attack()>
  */
  std::vector<Square> destination_squares;
  Piece pieceAtSq = this->piece_on(sq);
  Compass dirs[16] = {NORTH,EAST,SOUTH,WEST,NORTH_EAST,NORTH_WEST,SOUTH_EAST,SOUTH_WEST,
                      NORTH_NORTH_EAST, NORTH_NORTH_WEST,
                      NORTH_EAST_EAST, SOUTH_EAST_EAST,
                      SOUTH_SOUTH_EAST, SOUTH_SOUTH_WEST,
                      SOUTH_WEST_WEST, NORTH_WEST_WEST};
  switch(Position::type(pieceAtSq)) {
    case PAWN:
      return Position::pseudo_legal_pawn_moves(sq);
    case KNIGHT:
      for( int i = 8; i < 16; i++ ) {
        // Since we dont expect more than 1 move
        std::vector<Square> tau = Position::pseudo_legal_direction_squares(sq,dirs[i],false);
        if( tau.size() != 0 ) destination_squares.push_back(tau[0]);
      }
      break;
    case BISHOP:
      for( int i = 4; i < 8; i++ ) {
        std::vector<Square> gamma = Position::pseudo_legal_direction_squares(sq,dirs[i],true);
        destination_squares.insert(destination_squares.end(),gamma.begin(),gamma.end());
      }
      break;
    case ROOK:
      for( int i = 0; i < 4; i++ ) {
        std::vector<Square> gamma = Position::pseudo_legal_direction_squares(sq,dirs[i],true);
        destination_squares.insert(destination_squares.end(),gamma.begin(),gamma.end());
      }
      break;
    case QUEEN:
      for( int i = 0; i < 8; i++ ) {
        std::vector<Square> gamma = Position::pseudo_legal_direction_squares(sq,dirs[i],true);
        destination_squares.insert(destination_squares.end(),gamma.begin(),gamma.end());
      }
      break;
    case KING:
      for( int i = 0; i < 8; i++ ) {
        // Only expect at most 1 move
        std::vector<Square> tau = Position::pseudo_legal_direction_squares(sq,dirs[i],false);
        if( tau.size() != 0 ) destination_squares.push_back(tau[0]);
      }
      break;
    default:
      // pieceAtSq == NO_PIECE
      break;
  }

  std::vector<Move> moves;
  moves.reserve(destination_squares.size());
  for( long unsigned int i = 0; i < destination_squares.size(); i++ ) {
    moves.push_back(Move(sq,destination_squares[i]));
  }
  return moves;
}

std::vector<Move> Position::pseudo_legal_pawn_moves( Square sq ) const {
  /*
    Generates bitboard of 'normal' moves for a pawn on square <sq>
    such that the pawn is on the side of <this->to_attack()> i.e. the side
    to move.
  */

  std::vector<Move> moves;
  Bitboard sq_bb = SQUARE_TO_BB(sq);
  Bitboard to_promote_check = (this->to_attack() == WHITE ) ? RANK_7_BB:RANK_2_BB;
  Compass pawn_move_dir = (this->to_attack() == WHITE ) ? NORTH:SOUTH;
  Compass pawn_attack_east = Compass(pawn_move_dir + EAST);
  Compass pawn_attack_west = Compass(pawn_move_dir + WEST);

  // Move forward
  if( (sq_bb & to_promote_check) == 0 ) {
    // Not a promotion, therefore still counts as a 'normal' move
    Square move_fwd_sq = Square(sq + pawn_move_dir);
    if( this->piece_on(move_fwd_sq) == NO_PIECE ) {
      // Only move forward if there is nothing in the way!!!
      moves.push_back(Move(sq,move_fwd_sq));
    }

    if( Position::move_direction_before_edge(sq, pawn_attack_east)) {
      Square attack_east_sq = Square(sq+pawn_attack_east);
      if(Position::color(this->piece_on(attack_east_sq)) == this->to_defend()) {
        moves.push_back(Move(sq,attack_east_sq));
      }
    }

    if( Position::move_direction_before_edge(sq, pawn_attack_west)) {
      Square attack_west_sq = Square(sq+pawn_attack_west);
      if(Position::color(this->piece_on(attack_west_sq)) == this->to_defend()) {
        moves.push_back(Move(sq,attack_west_sq));
      }
    }

  }

  return moves;
}
Bitboard Position::pseudo_legal_direction_bitboard(Square sq, Compass dir, bool propagate) const {
  /*
    Creates a bitboard of pseudo legal moves squares given the direction
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
    Square next_square = sq;
    do {
      next_square = static_cast<Square>(next_square+dir);
      Bitboard next_square_bb = SQUARE_TO_BB(next_square);
      if( next_square_bb & self_pieces ) return move_pattern;
      move_pattern |= next_square_bb;
      if( next_square_bb & opponent_pieces ) return move_pattern;
    } while(Position::move_direction_before_edge(next_square,dir) && propagate);
  }

  return move_pattern;
}
std::vector<Square> Position::pseudo_legal_direction_squares(Square sq, Compass dir, bool propagate) const {
  /*
    Creates a vector of pseudo legal destination squares given the direction
    from the square specified by <sq> where the direction is propagated
    until a edge is hit. 
    (only propogates the direction if <propagate> is true
     else it'll only move in that direction once)
    -> Does not check if move will put own king in check.
    -> Does not include moves that capture or move over
        <side_to_move> pieces
    -> INCLUDES the 'first' capture on a given direction...
    NOTE: Does NOT include the square <sq> in the op
  */
  std::vector<Square> moves;
  Bitboard self_pieces = this->pieces(this->to_attack());
  Bitboard opponent_pieces = this->pieces(this->to_defend());
  if( Position::move_direction_before_edge(sq, dir)) {
    Square next_square = sq;
    do {
      next_square = static_cast<Square>(next_square+dir);
      Bitboard next_square_bb = SQUARE_TO_BB(next_square);
      if( next_square_bb & self_pieces ) return moves;
      moves.push_back(next_square);
      if( next_square_bb & opponent_pieces ) return moves;
    } while(Position::move_direction_before_edge(next_square,dir) && propagate);
  }

  return moves;
}

bool Position::is_attacked(Square sq) const {
  /*
    Returns true is square <sq> is attacked by color <this->to_defend()>
    (opponent pieces)
    false otherwise
  */
  
  Bitboard enemy_pieces = this->pieces(this->to_defend());
  Bitboard tau = EMPTY_BB;
  tau |= this->pseudo_legal_direction_bitboard(sq, NORTH, true);
  tau |= this->pseudo_legal_direction_bitboard(sq, SOUTH, true);
  tau |= this->pseudo_legal_direction_bitboard(sq, EAST, true);
  tau |= this->pseudo_legal_direction_bitboard(sq, WEST, true);
  tau &= enemy_pieces;
  if( tau & this->pieces(ROOK) || tau & this->pieces(QUEEN)) return true;
  tau = EMPTY_BB;
  tau |= this->pseudo_legal_direction_bitboard(sq, NORTH_EAST, true);
  tau |= this->pseudo_legal_direction_bitboard(sq, NORTH_WEST, true);
  tau |= this->pseudo_legal_direction_bitboard(sq, SOUTH_EAST, true);
  tau |= this->pseudo_legal_direction_bitboard(sq, SOUTH_WEST, true);
  tau &= enemy_pieces;
  if( tau & this->pieces(BISHOP) || tau & this->pieces(QUEEN)) return true;
  tau = EMPTY_BB;
  tau |= this->pseudo_legal_direction_bitboard(sq, NORTH_NORTH_EAST, false);
  tau |= this->pseudo_legal_direction_bitboard(sq, NORTH_NORTH_WEST, false);
  tau |= this->pseudo_legal_direction_bitboard(sq, SOUTH_SOUTH_EAST, false);
  tau |= this->pseudo_legal_direction_bitboard(sq, SOUTH_SOUTH_WEST, false);
  tau |= this->pseudo_legal_direction_bitboard(sq, NORTH_EAST_EAST, false);
  tau |= this->pseudo_legal_direction_bitboard(sq, NORTH_WEST_WEST, false);
  tau |= this->pseudo_legal_direction_bitboard(sq, SOUTH_EAST_EAST, false);
  tau |= this->pseudo_legal_direction_bitboard(sq, SOUTH_WEST_WEST, false);
  tau &= enemy_pieces;
  if( tau & this->pieces(KNIGHT)) return true;
  return false;
}

std::vector<Move> Position::pseudo_legal_promo_moves( Square sq ) const {
  std::vector<Move> moves;
  Bitboard sq_bb = SQUARE_TO_BB(sq);
  PieceType pt = Position::type(this->piece_on(sq));
  if( pt == PAWN ) {
    Bitboard promo_rank = this->to_attack() == WHITE ? RANK_7_BB : RANK_2_BB;
    if( sq_bb & promo_rank ) {
      Compass pawn_move_dir = (this->to_attack() == WHITE ? NORTH:SOUTH);
      Square move_fwd_sq = Square(sq + pawn_move_dir);

      if( this->piece_on(move_fwd_sq) == NO_PIECE ) {
        moves.push_back(Move(sq,move_fwd_sq,Move::PROMOTE_KNIGHT));
        moves.push_back(Move(sq,move_fwd_sq,Move::PROMOTE_BISHOP));
        moves.push_back(Move(sq,move_fwd_sq,Move::PROMOTE_ROOK));
        moves.push_back(Move(sq,move_fwd_sq,Move::PROMOTE_QUEEN));
      }

      if( Position::move_direction_before_edge(sq,WEST) ) {
        Square west_attack_sq = Square(sq+pawn_move_dir+WEST);
        if( Position::color(this->piece_on(west_attack_sq)) == this->to_defend()) {
          moves.push_back(Move(sq,west_attack_sq,Move::PROMOTE_KNIGHT));
          moves.push_back(Move(sq,west_attack_sq,Move::PROMOTE_BISHOP));
          moves.push_back(Move(sq,west_attack_sq,Move::PROMOTE_ROOK));
          moves.push_back(Move(sq,west_attack_sq,Move::PROMOTE_QUEEN));
        }
      }
      if( Position::move_direction_before_edge(sq,EAST) ) {
        Square east_attack_sq = Square(sq+pawn_move_dir+EAST);
        if( Position::color(this->piece_on(east_attack_sq)) == this->to_defend()) {
          moves.push_back(Move(sq,east_attack_sq,Move::PROMOTE_KNIGHT));
          moves.push_back(Move(sq,east_attack_sq,Move::PROMOTE_BISHOP));
          moves.push_back(Move(sq,east_attack_sq,Move::PROMOTE_ROOK));
          moves.push_back(Move(sq,east_attack_sq,Move::PROMOTE_QUEEN));
        }
      }
    }
  }
  return moves;
}
std::vector<Move> Position::pseudo_legal_ep_moves( Square sq ) const {
  std::vector<Move> moves;
  PieceType pt = Position::type(this->piece_on(sq));
  if( pt == PAWN ) {
    Square ep_target = this->en_passant_target();
    if( ep_target != NO_SQUARE ) {
      Compass pawn_move_dir = this->to_attack() == WHITE ? NORTH:SOUTH;
      if( Position::move_direction_before_edge(sq,WEST) ) {
        // Check West
        Square possible_west_attacker = static_cast<Square>(ep_target - pawn_move_dir + WEST);
        if( possible_west_attacker == sq ) {
          moves.push_back(Move(sq,ep_target,Move::EN_PASSANT));
        }
      }
      if( Position::move_direction_before_edge(sq,EAST) ) {
        // Check East
        Square possible_east_attacker = static_cast<Square>(ep_target - pawn_move_dir + EAST);
        if( possible_east_attacker == sq ) {
          moves.push_back(Move(sq,ep_target,Move::EN_PASSANT));
        }
      }
    }
  }
  return moves;
}
std::vector<Move> Position::pseudo_legal_castle_moves( Square sq ) const {
  std::vector<Move> moves;
  PieceType pt = Position::type(this->piece_on(sq));
  if( pt == KING && (sq == e1 || sq == e8) ) {
    bool is_white = this->to_attack() == WHITE;
    // Check if pieces exist on 'travel' squares
    // Check if castle squares are attacked
    // Check if king is currently in check (cant castle out of check)
    if( this->has_castle_right((is_white ? WHITE_SHORT_CASTLE : BLACK_SHORT_CASTLE)) &&
        (this->piece_on(Square(sq+EAST)) == NO_PIECE) &&
        (this->piece_on(Square(sq+EAST+EAST)) == NO_PIECE) &&
        !this->is_attacked(sq) &&
        !this->is_attacked(Square(sq+EAST)) &&
        !this->is_attacked(Square(sq+EAST+EAST)) )
    {
      moves.push_back(Move(sq,Square(sq+EAST+EAST),Move::CASTLE));
    }

    if( this->has_castle_right((is_white ? WHITE_LONG_CASTLE : BLACK_LONG_CASTLE)) &&
        (this->piece_on(Square(sq+WEST)) == NO_PIECE) &&
        (this->piece_on(Square(sq+WEST+WEST)) == NO_PIECE) &&
        (this->piece_on(Square(sq+WEST+WEST+WEST)) == NO_PIECE) &&
        !this->is_attacked(sq) &&
        !this->is_attacked(Square(sq+WEST)) &&
        !this->is_attacked(Square(sq+WEST+WEST)) )
    {
      moves.push_back(Move(sq,Square(sq+WEST+WEST),Move::CASTLE));
    }

  }
  return moves;
}

bool Position::pseudo_legal_move_is_legal(const Position& pos,const Move& m) {
  Position* p = Position::copy(pos);

  p->make_move(m); // Opponent's move!
  // Swap side to move so we can analyze from our perspective
  p->side_to_move = p->to_defend();

  Piece king_t = Position::to_piece(p->to_attack(), KING);
  Square king_square = Position::to_square(p->pieces(king_t));
  bool king_attacked = p->is_attacked(king_square);
  delete p;
  if( king_attacked ) return false;
  else return true;
}

void Position::make_move(const Move& m) {
  /*
     Updates 'this' position to reflect the move
     specified by <m>.
     Assumes that move is legal and valid.
     (Does not check for legality or validity of move
      and inherits the assumptions of
      a valid position as such.)

  */
  Move::MoveFlag flag = m.flag();
  Piece piece_to_move = this->pieceBySquare[m.initial_square()];
  this->remove(m.initial_square());
  this->remove(m.final_square());
  this->en_passant_target_square = NO_SQUARE;
  this->half_move_clock += 1;
  if( this->to_attack() == WHITE ) this->full_move_clock+=1;
  // Capture -> Halfmove is now 0
  if(this->pieceBySquare[m.final_square()] != NO_PIECE) this->half_move_clock = 0;
  switch(flag) {
    case Move::NO_FLAG:
    {
      PieceType piece_t = Position::type(piece_to_move);
      Color   c_to_move = Position::color(piece_to_move);
      if( piece_t == PAWN ) {
        // Pawn move, Halfclock is now 0
        this->half_move_clock = 0;
        // Check for double pawn push
        Compass pawn_move_dir = (Position::color(piece_to_move) == WHITE) ? NORTH:SOUTH;
        if( Square(m.initial_square() + 2*pawn_move_dir) == m.final_square() ) {
          this->en_passant_target_square = Square(m.initial_square() + pawn_move_dir);
        }
      } else if( piece_t == ROOK ) {
        Square short_rook_sq = this->to_attack() == WHITE ? h1 : h8;
        Square long_rook_sq = this->to_attack() == WHITE ? a1 : a8;
        if( m.initial_square() ==  short_rook_sq ) {
          this->revoke_castle_right((c_to_move == WHITE ? WHITE_SHORT_CASTLE : BLACK_SHORT_CASTLE));
        } else if ( m.initial_square() == long_rook_sq ) {
          this->revoke_castle_right((c_to_move == WHITE ? WHITE_LONG_CASTLE : BLACK_LONG_CASTLE));
        }
      } else if( piece_t == KING ) {
        CastleRight right = c_to_move == WHITE ? WHITE_CASTLE : BLACK_CASTLE;
        this->revoke_castle_right(right);
      }
      this->put(m.final_square(),piece_to_move);
      break;
    }
    case Move::CASTLE:
    {
      // Castle cannot possibly capture a piece.
      Piece king_t = Position::to_piece(this->to_attack(), KING);
      Piece rook_t = Position::to_piece(this->to_attack(), ROOK);
      bool is_short_castle = ((SQUARE_TO_BB(m.final_square()) & FILE_G_BB) != 0);
      Square rook_init_sq = Square(m.initial_square() + (is_short_castle ? (3*EAST):(4*WEST)) );
      Square rook_final_sq = Square(m.final_square() + (is_short_castle ? (WEST):(EAST)));
      this->remove(rook_init_sq); // Remove ROOK
      this->put(m.final_square(), king_t ); // Put KING
      this->put(rook_final_sq, rook_t ); // Put ROOK
      break;
    }
    case Move::EN_PASSANT:
      this->put(m.final_square(),piece_to_move);
      break;
    case Move::PROMOTE_KNIGHT:
      this->put(m.final_square(), Position::to_piece(this->to_attack(), KNIGHT) );
      break;
    case Move::PROMOTE_BISHOP:
      this->put(m.final_square(), Position::to_piece(this->to_attack(), BISHOP) );
      break;
    case Move::PROMOTE_ROOK:
      this->put(m.final_square(), Position::to_piece(this->to_attack(), ROOK) );
      break;
    case Move::PROMOTE_QUEEN:
      this->put(m.final_square(), Position::to_piece(this->to_attack(), QUEEN) );
      break;
    default:
      // ERROR?!
      std::cerr << "Impossible MoveFlag in Position::make_move()" << std::endl;
      break;
  }
  this->side_to_move = this->to_defend();
}
