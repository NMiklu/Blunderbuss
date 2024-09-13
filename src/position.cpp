#include "representation.h"
#include "position.h"
#include "misc.h"
#include <string>
#include <sstream>

#include <iostream> //DEBUG REMOVE LATER

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

  /* TODO
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
          LERF_Square sq = static_cast<LERF_Square>( (8*rank_idx) + file_idx );
          PieceEnum color = (isWhitePiece ? white_p : black_p );
          PieceEnum piece = static_cast<PieceEnum>(Utility::index_char_in_str(value,(isWhitePiece ? white_piece : black_piece)) + 2);
          this->insert(sq,color,piece);
          file_idx++;
        } else {
          // Is not a white piece, black piece, nor a digit
          return false;
        }
      }
    }
  }
  



  /* TODO
    <Side to move> ::= {'w' | 'b'}
  */
  /* TODO
    <Castling ability> ::= '-' | ['K'] | ['Q'] | ['k'] | ['q'] (1..4)
    <En passant target square> ::= '-' | <epsquare>
    <epsquare>   ::= <fileLetter><eprank>
    <fileLetter> ::= 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h'
    <eprank>     ::= '3' | '6'
  */
  /* TODO
    <Halfmove Clock> ::= <digit> {<digit>}
    <digit> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
  */
  /* TODO
    <Fullmove counter> ::= <digit19> {<digit>}
    <digit19>          ::= '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
    <digit>            ::= '0' | <digit19>
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
  this->castle_ability |= (white_short_castle);
  this->castle_ability |= (white_long_castle);
  this->castle_ability |= (black_short_castle);
  this->castle_ability |= (black_long_castle);
}
