#include "representation.h"
#include "position.h"
#include "misc.h"
#include <string>
#include <sstream>
#include <cmath>

#include <iostream> //DEBUG REMOVE LATER

Bitboard Position::pieces( Piece x ) const {
  /* 
    Returns 64 bit int-type representing 
    the pieces specified by Piece <x>
   */
  return pieceBB[x];
}

void Position::pretty( std::ostream& os ) const {
  /*
    Outputs ASCII chess board to ostream <os>
  */
  char board[64][3];
  for( int i = 0; i < 64; i++) {
    Square square = static_cast<Square>(i);
    board[i][0] = ((this->pieceBB[white_p] & SQUARE_TO_BB(square)) ? 'W':' ');
    board[i][0] = ((this->pieceBB[black_p] & SQUARE_TO_BB(square)) ? 'B':board[i][0]);
    board[i][1] = ' ';
    board[i][2] = '\0';
    
    for( int j = 2; j<8; j++) {
      if( this->pieceBB[j] & SQUARE_TO_BB(square)) {
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
void Position::set( Square sq, Piece piece_p ) {
  /*
    Overwrites <piece> onto the given square <sq>.
  */
  
  Bitboard squareBB = SQUARE_TO_BB(sq);
  this->pieceBB[piece_p] |= squareBB;
}
void Engine::remove(Square sq) {
  /* clears Square <sq> from any pieces*/
  Bitboard tau = (~SQUARE_TO_BB(sq))
  for( int i = 0; i < 8; i++ ) {
    this->pieceBB[i] &= tau;
  }
}
bool Position::castle(uint8_t bits) {
  /* returns true if the bits specified by <bits> are set
      in the castle data.
      false otherwise 
      bits are best specified by CastleMask enum -> refer to representation.h*/
  return this->castle_ability & bits == bits;
}
bool Position::revoke_castle(Piece color) {
  /* Revokes the castle rights in 'this' position of the side
    specified by <color> (which should be white_p or black_p) 
    returns true if successful
    false otherwise
  */
  switch(color) {
    case white_p:
      this->castle_ability &= (~(white_long_castle | white_short_castle));
      break;
    case black_p:
      this->castle_ability &= (~(black_long_castle | black_short_castle));
      break;
    default:
      return false;
  }
  return true;
}

bool Position::halfmove() {
  if( this->half_move_clock >= Position::MAX_HALF_MOVE ) {
    return false;
  }
  this->half_move_clock+=1;
  return true;
}
bool Position::fullmove() {
  this->side_to_move = (this->side_to_move == white_p ? black_p : white_p);
  this->full_move_counter+=1;
  return true;
}
bool Position::fen(std::string fen_string ) {
  /*
    Sets 'this' object to represent the chess position
    specified by <fen_string> according to 
    Forsyth-Edwards Notation
    https://www.chessprogramming.org/Forsyth-Edwards_Notation
    Returns true if representation is updated.
    False if the fen is invalid.
    WARNING -> does NOT revert changes to 'this' that were 
                manipulated before failure.
  */
  /*
    <FEN> ::= <Piece Placement>
          ' ' <Side to move>
          ' ' <Castling ability>
          ' ' <En passant target square>
          ' ' <Halfmove clock>
          ' ' <Fullmove counter>
  */
  std::vector<std::string> fenv = Utility::tokenize(fen_string,' ');
  if( fenv.size() != 6 ) return false;

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
  const std::string digit17     = "1234567";
  const std::string white_piece = "PNBRQK";
  const std::string black_piece = "pnbrqk";

  std::vector<std::string> piece_placement = Utility::tokenize(fenv[0],'/');
  if( piece_placement.size() != 8 ) return false;

  for( int rank_idx = 7; rank_idx >= 0; rank_idx-- ) {
    int pp_idx = 7-(rank_idx);
    std::string ranki = piece_placement[pp_idx];
    int file_idx = 0;
    for( long unsigned int ranki_idx = 0; ranki_idx < ranki.size(); ranki_idx++ ) {
      char value = ranki[ranki_idx];
      bool isDigit17 = Utility::find_char_in_str( value, digit17);
      if( isDigit17 || value == '8' ) {
        file_idx += (atoi( (const char*) &value ));
      } else {
        bool isWhitePiece = Utility::find_char_in_str( value, white_piece);
        bool isBlackPiece = Utility::find_char_in_str( value, black_piece);
        if( isWhitePiece || isBlackPiece ) {
          Square sq = static_cast<Square>( (8*rank_idx) + file_idx );
          Piece color = (isWhitePiece ? white_p : black_p );
          Piece piece = static_cast<Piece>(Utility::index_char_in_str(value,(isWhitePiece ? white_piece : black_piece)) + 2);
          this->set(sq,color);
          this->set(sq,piece);
          file_idx++;
        } else {
          // Is not a white piece, black piece, nor a digit
          return false;
        }
      }
    }
  }

  /* 
    <Side to move> ::= {'w' | 'b'}
  */
  const std::string side = fenv[1];
  if( side == "w" ) {
    this->side_to_move = white_p;
  } else if ( side == "b" ) {
    this->side_to_move = black_p;
  } else {
    // Invalid color option
    return false;
  }

  /*
    <Castling ability> ::= '-' | ['K'] | ['Q'] | ['k'] | ['q'] (1..4)
  */

  const std::string castling_token = fenv[2];
  std::istringstream is(castling_token);
  char value;
  while( is >> value ) {
    switch(value) {
      case '-':
        this->castle_ability = 0;
        break;
      case 'K':
        this->castle_ability |= white_short_castle;
        break;
      case 'Q':
        this->castle_ability |= white_long_castle;
        break;
      case 'k':
        this->castle_ability |= black_short_castle;
        break;
      case 'q':
        this->castle_ability |= black_long_castle;
        break;
      default:
        return false;
    }
  }

  /*
    <En passant target square> ::= '-' | <epsquare>
    <epsquare>   ::= <fileLetter><eprank>
    <fileLetter> ::= 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h'
    <eprank>     ::= '3' | '6'
  */
  const std::string epts = fenv[3];
  if( epts == "-" ) {
    this->en_passant_target_square = NO_SQUARE;
  } else if (epts.size() == 2) {
    char rank = epts[0];
    char file = epts[1];
    if( ((rank == '3' || rank == '6') && file >= 'a' && file <= 'h') ) {
      int  rankidx = (int)rank - 49;
      int  fileidx = (int)file - 97;
      this->en_passant_target_square = static_cast<Square>((8*rankidx) + fileidx);
    } else {
      return false;
    }
  } else {
    return false;
  }


  /*
    <Halfmove Clock> ::= <digit> {<digit>}
    <digit> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
  */
  const std::string half_move = fenv[4];
  const std::string digit09 = "0123456789";
  this->half_move_clock = 0;
  for( size_t i = 0; i < half_move.size(); i++ ) {
    if( !Utility::find_char_in_str(half_move[i],digit09) )
      return false;
  }
  this->half_move_clock = Utility::str_to_decimal_uint8(half_move);


  /*
    <Fullmove counter> ::= <digit19> {<digit>}
    <digit19>          ::= '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
    <digit>            ::= '0' | <digit19>
  */
  const std::string fullmove_str = fenv[5];
  const std::string digit19 = "123456789";
  if( fullmove_str[0] == '0' || (!Utility::find_char_in_str(fullmove_str[0],digit19)))
    return false;
  for( size_t i = 0; i < fullmove_str.size(); i++ ) {
    if(!Utility::find_char_in_str(fullmove_str[i],digit09))
      return false;
  }
  this->full_move_counter = Utility::str_to_decimal_uint8(fullmove_str);

  return true;
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
  this->en_passant_target_square = NO_SQUARE;
  this->side_to_move    = white_p;
  this->castle_ability |= (white_short_castle);
  this->castle_ability |= (white_long_castle);
  this->castle_ability |= (black_short_castle);
  this->castle_ability |= (black_long_castle);
  this->half_move_clock = 0;
  this->full_move_counter = 1;
}
bool Position::check_rep() const {
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
  */

  // There are white and black pieces on top of each other!
  if( (pieceBB[white_p] & pieceBB[black_p]) != 0 ) return false;

  for( int i = 2; i < 8; i++ ) {
    if( !(pieceBB[i] & pieceBB[white_p] || pieceBB[i] & pieceBB[black_p]) )
      // piece-type 'i' isn't claimed by the white or black pieces
      return false;
    for( int j = i+1; j < 8; j++ ) {
      if((pieceBB[i] & pieceBB[j]) != 0)
        // Multiple piece types on a single square
        return false;
    }
  }

  if( this->castle_ability >= 16 )
    // Invalid castle value
    return false;
  if( this->half_move_clock > 50 )
    //Impossible half_move_clock
    return false;
  if( this->full_move_counter == 0 )
    // Invalid move clock
    return false;
  int sv = static_cast<int>(this->en_passant_target_square);
  if( !((sv >= 16 && sv <= 23) || (sv >= 40 || sv <= 47) || (sv == 64)) )
    // Invalid en passant square
    return false;
  if( !((this->side_to_move == white_p)||(this->side_to_move == black_p)))
    // Invalid piece-type designated side to move
    return false;

  return true;
}

Sqaure Position::en_passant_target() const {
  return this->en_passant_target_square;
}

Piece Position::piece_at_square( Square sq ) {
  Bitboard tau = SQUARE_TO_BB(sq);
  for( int i = pawn_p; i < none_p; i++ ){
    if( this->pieceBB[i] & tau )
      return static_cast<Piece>(i);
  }
  return none_p;
}

Position* Position::copy(const Position& pos) {
  Position* copy_ = new Position::Position();
  for( unsigned short i = 0; i < 8; i++ ) {
    copy_->pieceBB[i] = pos.pieceBB[i];
  }
  copy_->en_passant_target_square = pos.en_passant_target_square;
  copy_->castle_ability = pos.castle_ability;
  copy_->half_move_clock = pos.half_move_clock;
  copy_->full_move_counter = pos.full_move_counter;
  return copy_;
}
