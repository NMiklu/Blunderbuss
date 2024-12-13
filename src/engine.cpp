#include "engine.h"
#include "representation.h"
#include <vector>


bool Engine::square_is_edge( enum Square sq ) {
  if( SQUARE_TO_BB(sq) & EDGE_BB ) return true;
  return false;
}
bool Engine::square_is_corner( enum Square sq ) {
  if( SQUARE_TO_BB(sq) & CORNER_BB ) return true;
  return false;
}

Piece Engine::engine_color() {
  return this->pos->side_to_move;
}

Piece Engine::opponent_color() {
  return Engine::engine_color() == white_p ? black_p : white_p;
}

Bitboard Engine::engine_pieces() {
  return this->pos->pieces(Engine::engine_color());
}

Bitboard Engine::opponent_pieces() {
  return this->pos->pieces(Engine::opponent_color());
}



void Engine::propagate_compass_on_bitboard(Bitboard& bb, Square initial_sq, Compass direction) {
  enum Square next_square = initial_sq + direction;
  Bitboard opponent_bb = opponent_pieces();
  Bitboard engine_bb = engine_pieces();
  while( !Engine::square_is_edge(next_square) && !(opponent_bb & SQUARE_TO_BB(next_square)) ) {
    if( SQUARE_TO_BB(next_square) & engine_bb) return bb; // Do not capture your own piece!
    bb |= SQUARE_TO_BB(next_square);
    next_square += direction;
  }
  bb |= SQUARE_TO_BB(next_square); // Make sure to include the edge square or 'first' enemy piece
}

Bitboard Engine::generate_bishop_move_pattern_bitboard( enum Square sq ) {
  if( sq == NO_SQUARE ) return EMPTY_BB;
  Bitboard pattern = EMPTY_BB;
  Bitboard sq_bb = SQUARE_TO_BB(sq);
  bool NE = false;
  bool NW = false;
  bool SE = false;
  bool SW = false;
  if( Engine::square_is_edge(sq) ) {
    if( Engine::square_is_corner(sq) ) {
      if( sq == a1 ) NE = true;
      else if( sq ==  h1 ) NW = true;
      else if( sq == a8 ) SE = true;
      else if( sq == h8 ) SW = true;
    } else { // Square is not a corner, but is an edge!
      if( sq_bb & FILE_A_BB ) {
        NE = true;
        SE = true;
      }
      else if( sq_bb & FILE_H_BB) {
        NW = true;
        SW = true;
      }
      else if( sq_bb & RANK_1_BB) {
        NE = true;
        NW = true;
      }
      else if( sq_BB & RANK_8_BB ) {
        SE = true;
        SW = true;
      }
    }

  } else { // Square is not on the edge of the board!
    NE = true;
    NW = true;
    SE = true;
    SW = true;
  }

  if( NE ) {
    Engine::propagate_compass_on_bitboard(pattern,sq,NORTH_EAST);
  }
  if( NW ) {
    Engine::propagate_compass_on_bitboard(pattern,sq,NORTH_WEST);
  }
  if( SE ) {
    Engine::propagate_compass_on_bitboard(pattern,sq,SOUTH_EAST);
  }
  if( SW ) {
    Engine::propagate_compass_on_bitboard(pattern,sq,SOUTH_WEST);
  }

  Bitboard my_pieces = engine_pieces();
  return pattern & (~my_piece);
}
Bitboard Engine::generate_rook_move_pattern_bitboard( enum Square sq ) {
  if( sq == NO_SQUARE ) return EMPTY_BB;
  Bitboard pattern = EMPTY_BB;
  Bitboard sq_bb = SQUARE_TO_BB(sq);
  bool N = true;
  bool W = true;
  bool E = true;
  bool S = true;
  if( Engine::square_is_edge(sq) ) {
    if( sq_bb & FILE_A_BB) {
      W = false;
    }
    if( sq_bb & FILE_H_BB) {
      E = false;
    }
    if( sq_bb & RANK_1_BB) {
      S = false;
    }
    if( sq_bb & RANK_8_BB) {
      N = false;
    }
  }
  if( N ) {
    Engine::propagate_compass_on_bitboard(pattern,sq,NORTH);
  }
  if( S ) {
    Engine::propagate_compass_on_bitboard(pattern,sq,SOUTH);
  }
  if( E ) {
    Engine::propagate_compass_on_bitboard(pattern,sq,EAST);
  }
  if( W ) {
    Engine::propagate_compass_on_bitboard(pattern,sq,WEST);
  }

  return pattern;
}

Bitboard Engine::generate_knight_move_pattern_bitboard(enum Square sq) {
  /*
    Below diagram relates the according squares to
    how they're named in this function:
    +---+---+---+---+---+---+---+
    |   |   |sq8|   |sq1|   |   |
    +---+---+---+---+---+---+---+
    |   |sq7|   |   |   |sq2|   |
    +---+---+---+---+---+---+---+
    |   |   |   | K |   |   |   |
    +---+---+---+---+---+---+---+
    |   |sq6|   |   |   |sq3|   |
    +---+---+---+---+---+---+---+
    |   |   |sq5|   |sq4|   |   |
    +---+---+---+---+---+---+---+
  */
  if( sq == NO_SQUARE ) return EMPTY_BB;
  Bitboard pattern = EMPTY_BB;
  Bitboard sq_bb = SQUARE_TO_BB(sq);
  bool sq1 = true;
  bool sq2 = true;
  bool sq3 = true;
  bool sq4 = true;
  bool sq5 = true;
  bool sq6 = true;
  bool sq7 = true;
  bool sq8 = true;

  if( sq_bb & RANK_1_BB){
    sq3 = false;
    sq4 = false;
    sq5 = false;
    sq6 = false;
  }
  else if( sq_bb & RANK_2_BB ) {
    sq5 = false;
    sq4 = false;
  }
  else if(sq_bb & RANK_7_BB) {
    sq1 = false;
    sq8 = false;
  } 
  else if( sq_bb & RANK_8_BB) {
    sq1 = false;
    sq2 = false;
    sq7 = false;
    sq8 = false;
  }

  if( sq_bb & FILE_A_BB ) {
    sq8 = false;
    sq7 = false;
    sq6 = false;
    sq5 = false;
  }
  else if (sq_bb & FILE_B_BB) {
    sq7 = false;
    sq6 = false;
  }
  else if( sq_bb & FILE_H_BB) {
    sq1 = false;
    sq2 = false;
    sq3 = false;
    sq4 = false;
  }
  else if( sq_bb & FILE_G_BB) {
    sq2 = false;
    sq3 = false;
  }

  if( sq1 ) {
    pattern |= SQUARE_TO_BB(sq+NORTH+NORTH_EAST);
  }
  if( sq2 ) {
    pattern |= SQUARE_TO_BB(sq+EAST+NORTH_EAST);
  }
  if( sq3 ) {
    pattern |= SQUARE_TO_BB(sq+EAST+SOUTH_EAST);
  }
  if( sq4 ) {
    pattern |= SQUARE_TO_BB(sq+SOUTH+SOUTH_EAST);
  }
  if( sq5 ) {
    pattern |= SQUARE_TO_BB(sq+SOUTH+SOUTH_WEST);
  }
  if( sq6 ) {
    pattern |= SQUARE_TO_BB(sq+WEST+SOUTH_WEST);
  }
  if( sq7 ) {

    pattern |= SQUARE_TO_BB(sq+WEST+NORTH_WEST);
  }
  if( sq8 ) {
    pattern |= SQUARE_TO_BB(sq+NORTH+NORTH_WEST);
  }

  return pattern;
}
Bitboard Engine::generate_king_move_pattern_bitboard(enum Square sq) {
  // DOES NOT ACCOUNT FOR CASTLING!!!
  if( sq == NO_SQUARE ) return EMPTY_BB;
  Bitboard pattern = EMPTY_BB;
  Bitboard sq_bb = SQUARE_TO_BB(sq);
  bool N = true;
  bool NE = true;
  bool NW = true;
  bool E = true;
  bool W = true;
  bool SE = true;
  bool SW = true;
  bool S = true;

  if( sq_bb & RANK_1_BB ) {
    S = false;
    SW = false;
    SE = false;
  }
  else if( sq_bb & RANK_8_BB) {
    N = false;
    NE = false;
    NW = false;
  }

  if( sq_bb & FILE_A_BB) {
    W = false;
    NW = false;
    SW = false;
  }
  else if( sq_bb & FILE_H_BB) {
    E = false;
    NE = false;
    SE = false;
  }

  if( N )  pattern |= SQUARE_TO_BB(sq+NORTH);
  if( E )  pattern |= SQUARE_TO_BB(sq+EAST);
  if( W )  pattern |= SQUARE_TO_BB(sq+WEST);
  if( S )  pattern |= SQUARE_TO_BB(sq+SOUTH);
  if( NW )  pattern |= SQUARE_TO_BB(sq+NORTH_WEST);
  if( NE )  pattern |= SQUARE_TO_BB(sq+NORTH_EAST);
  if( SE )  pattern |= SQUARE_TO_BB(sq+SOUTH_EAST);
  if( SW )  pattern |= SQUARE_TO_BB(sq+SOUTH_WEST);

  return pattern;
}
Bitboard Engine::generate_queen_move_pattern_bitboard(enum Square sq) {
  if( sq == NO_SQUARE ) return EMPTY_BB;
  Bitboard pattern = EMPTY_BB;
  Bitboard sq_bb = SQUARE_TO_BB(sq);
  bool N = true;
  bool NE = true;
  bool NW = true;
  bool E = true;
  bool W = true;
  bool SE = true;
  bool SW = true;
  bool S = true;

  if( sq_bb & RANK_1_BB ) {
    S = false;
    SW = false;
    SE = false;
  }
  else if( sq_bb & RANK_8_BB) {
    N = false;
    NE = false;
    NW = false;
  }

  if( sq_bb & FILE_A_BB) {
    W = false;
    NW = false;
    SW = false;
  }
  else if( sq_bb & FILE_H_BB) {
    E = false;
    NE = false;
    SE = false;
  }

  if( N )  Engine::propagate_compass_on_bitboard(pattern,sq,NORTH);
  if( E )  Engine::propagate_compass_on_bitboard(pattern,sq,EAST);
  if( W )  Engine::propagate_compass_on_bitboard(pattern,sq,WEST);
  if( S )  Engine::propagate_compass_on_bitboard(pattern,sq,SOUTH);
  if( NE ) Engine::propagate_compass_on_bitboard(pattern,sq,NORTH_EAST);
  if( NW ) Engine::propagate_compass_on_bitboard(pattern,sq,NORTH_WEST);
  if( SE ) Engine::propagate_compass_on_bitboard(pattern,sq,SOUTH_EAST);
  if( SW ) Engine::propagate_compass_on_bitboard(pattern,sq,SOUTH_WEST);
  
  return pattern;
}

Bitboard Engine::generate_pawn_move_pattern_bitboard( enum Square sq ) {
  //TODO
  // NOTE: Do not include moving to the 'promotion' rank nor
  // does this include en passant. these moves are covered by
  // the special move generator...
  // TODO -> PAWN CAPTURES!

  if( sq == NO_SQUARE ) return EMPTY_BB;
  Bitboard pattern = EMPTY_BB;
  Bitboard sq_bb = SQUARE_TO_BB(sq);
  Bitboard opponent_p = opponent_pieces();
  Bitboard engine_p   = engine_pieces();
  if( this->pos->side_to_move == white_p ) { // White pieces

    // Move
    bool blocking_1 = ((opponent_p & engine_p) & SQUARE_TO_BB(sq+NORTH)) > 0;
    bool blocking_2 = ((opponent_p & engine_p) & SQUARE_TO_BB(sq+NORTH+NORTH)) > 0;
    if( sq_bb & RANK_7_BB ) return EMPTY_BB; // SPECIAL MOVE
    else if( sq_bb & RANK_2_BB) {
      if( !blocking_2 ) pattern |= SQUARE_TO_BB(sq+NORTH+NORTH);
    }
    if( !blocking_1 ) pattern |= SQUARE_TO_BB(sq+NORTH);

    // Capture
    Bitboard attack_left = SQUARE_TO_BB(sq+NORTH_WEST);
    Bitboard attack_right = SQUARE_TO_BB(sq+NORTH_EAST);
    bool can_attack_left = (attack_left & opponent_p) > 0;
    bool can_attack_right = (attack_right & opponent_p) > 0;

    if( can_attack_left ) pattern |= attack_left;
    if( can_attack_right ) pattern |= attack_right;

  } else { // Black pieces
    // Move
    bool blocking_1 = ((opponent_p & engine_p) & SQUARE_TO_BB(sq+SOUTH)) > 0;
    bool blocking_2 = ((opponent_p & engine_p) & SQUARE_TO_BB(sq+SOUTH+SOUTH)) > 0;
    if( sq_bb & RANK_2_BB ) return EMPTY_BB; // SPECIAL MOVE
    else if( sq_bb & RANK_2_BB) {
      if( !blocking_2 ) pattern |= SQUARE_TO_BB(sq+SOUTH+SOUTH);
    }

    if( !blocking_1 ) pattern |= SQUARE_TO_BB(sq+SOUTH);

    // Capture
    Bitboard attack_left = SQUARE_TO_BB(sq+SOUTH_WEST);
    Bitboard attack_right = SQUARE_TO_BB(sq+SOUTH_EAST);
    bool can_attack_left = (attack_left & opponent_p) > 0;
    bool can_attack_right = (attack_right & opponent_p) > 0;
    
    if(can_attack_left) pattern |= attack_left;
    if(can_attack_right) pattern |= attack_right;
  }

  return pattern;
}

std::vector<Square> Engine::bitboard_to_squares(Bitboard bb) {
  std::vector<Square> selected;
  for( int i = 0; i < NO_SQUARE; i += EAST) {
    enum Square sq = static_cast<Square>(i);
    if( SQUARE_TO_BB(sq) & bb ) {
      selected.push_back(sq);
    }
  }
  return selected;
}

std::vector<Move> Engine::generate_pseudo_legal_special_moves() {
  std::vector<Move> special_moves;

  // Castling
  if( Engine::engine_color() == white_p ) {
    if( this->pos->castle(white_short_castle) ) {
      special_moves.push_back(Move(NO_SQUARE,NO_SQUARE,SHORT_CASTLE));
    }
    if( this->pos->castle(white_long_castle) ) {
      special_moves.push_back(Move(NO_SQUARE,NO_SQUARE,LONG_CASTLE));
    }
  } else {
    if( this->pos->castle(black_short_castle) ) {
      special_moves.push_back(Move(NO_SQUARE,NO_SQUARE,SHORT_CASTLE));
    }
    if( this->pos->castle(black_long_castle) ) {
      special_moves.push_back(Move(NO_SQUARE,NO_SQUARE,LONG_CASTLE));
    }
  }

  // en Passant
  // ASSUMPTION: If the En Passant Target Square is specified in position object,
  //              then we know that the previous move was a double pawn push!!!
  enum Square ep_target = this->pos->en_passant_target();
  Bitboard engine_pawns = Engine::engine_pieces() & this->pos->pieces(pawn_t);
  if( ep_target != NO_SQUARE ) {
    // Last move was a double push! Check if any of your pawns are attacking that square!
    enum Square ep_attacker_east = NO_SQUARE;
    enum Square ep_attacker_west = NO_SQUARE;
    if( Engine::engine_color() == white_p ) {
      ep_attacker_east = (ep_target == h7) ? NO_SQUARE : ep_target + SOUTH_EAST;
      ep_attacker_west = (ep_target == a7) ? NO_SQUARE : ep_target + SOUTH_WEST;
    } else { // Black pieces
      ep_attacker_east = (ep_target == h2) ? NO_SQUARE : ep_target + NORTH_EAST;
      ep_attacker_west = (ep_target == a2) ? NO_SQUARE : ep_target + NORTH_WEST;
    }
    if( ep_attacker_east != NO_SQUARE && (SQUARE_TO_BB(ep_attacker_east) & engine_pawns)) {
      special_moves.push_back(Move(ep_attacker_east,ep_target,EN_PASSANT));
    }
    if( ep_attacker_west != NO_SQUARE && (SQUARE_TO_BB(ep_attacker_west) & engine_pawns)) {
      special_moves.push_back(Move(ep_attacker_west,ep_target,EN_PASSANT));
    }
  }


  // Promotions
  // Check if engine pawns are on the n-1 rank
  // Check if pieces are blocking the nth rank
  if( Engine::engine_color() == white_p ) {
    Bitboard promoting_pawns = engine_pawns & RANK_7_BB;
    if( promoting_pawns > 0 ) {
      Bitboard blocking_pieces = (Engine::engine_pieces() & Engine::opponent_pieces() ) & RANK_8_BB;
      Bitboard can_promote_squares = (promoting_pawns << 8) & (~blocking_pieces);
      std::vector<Square> promote_squares = Engine::bitboard_to_squares(can_promote_squares);
      for(int i = 0; i < promote_squares.size(); i++ ) {
        special_moves.push_back(Move(promote_squares(i)+SOUTH,promote_squares(i),PROMOTE_KNIGHT));
        special_moves.push_back(Move(promote_squares(i)+SOUTH,promote_squares(i),PROMOTE_BISHOP));
        special_moves.push_back(Move(promote_squares(i)+SOUTH,promote_squares(i),PROMOTE_ROOK));
        special_moves.push_back(Move(promote_squares(i)+SOUTH,promote_squares(i),PROMOTE_QUEEN));
      }
    }
  } else { // Black pieces
    Bitboard promoting_pawns = engine_pawns & RANK_2_BB;
    if( promoting_pawns > 0) {
      Bitboard blocking_pieces = (Engine::engine_pieces() & Engine::opponent_pieces() ) & RANK_1_BB;
      Bitboard can_promote_squares = (promoting_pawns >> 8) & (~blocking_pieces);
      std::vector<Squares> promote_squares = Engine::bitboard_to_squares(can_promote_squares);
      for(int i = 0; i < promote_squares.size(); i++ ) {
        special_moves.push_back(Move(promote_squares(i)+NORTH,promote_squares(i),PROMOTE_KNIGHT));
        special_moves.push_back(Move(promote_squares(i)+NORTH,promote_squares(i),PROMOTE_BISHOP));
        special_moves.push_back(Move(promote_squares(i)+NORTH,promote_squares(i),PROMOTE_ROOK));
        special_moves.push_back(Move(promote_squares(i)+NORTH,promote_squares(i),PROMOTE_QUEEN));
      }
    }
  }

  return special_moves;
}
std::vector<Move> Engine::generate_pseudo_legal_moves(enum Piece p, enum Square sq) {
  /*
    returns vector of pseudo legal moves for the piece at specified square
    at given position pos. 
    DOES NOT CHECK IF THE KING WILL BE ATTACKED AFTER MOVE
    DOES NOT GENERATE ANY CASTLING MOVES
    DOES NOT GENERATE EN PASSANT MOVES
  */
  
  Bitboard move_pattern = EMPTY_BB;
  if( sq == NO_SQUARE ) move_pattern = EMPTY_BB;
  switch(p) {
    case pawn_p:
      move_pattern = Engine::generate_pawn_move_pattern_bitboard(sq);
      break;
    case knight_p:
      move_pattern = Engine::generate_knight_move_pattern_bitboard(sq);
      break;
    case bishop_p:
      move_pattern = Engine::generate_bishop_move_pattern_bitboard(sq);
      break;
    case rook_p:
      move_pattern = Engine::generate_rook_move_pattern_bitboard(sq);
      break;
    case queen_p:
      move_pattern = Engine::generate_queen_move_pattern_bitboard(sq);
      break;
    case king_p:
      move_pattern = Engine::generate_king_move_pattern_bitboard(sq);
      break;
    default:
      break;
  }

  Piece engine_color = this->pos->side_to_move;
  std::vector<Square> final_squares = Engine::bitboard_to_squares(move_pattern);
  std::vector<Move> pseudo_legal_moves;
  for( int i = 0; i < final_squares.size(); i++) {
    Move m(sq,final_squares(i),NO_FLAG);
    pseudo_legal_moves.push_back(m);
  }

  return pseudo_legal_moves;
}

bool Engine::legal_move(Move m) {
  
  return false;
}

std::vector<Move> Engine::generate_legal_moves( const Position& pos ) {
  //TODO
  return NULL;
}