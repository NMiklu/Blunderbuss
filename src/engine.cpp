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

Piece Engine::engine_color(const Position& pos) {
  return pos.side_to_move;
}

Piece Engine::opponent_color(const Position& pos) {
  return Engine::engine_color(pos) == white_p ? black_p : white_p;
}

Bitboard Engine::engine_pieces(const Position& pos) {
  return pos.pieces(Engine::engine_color());
}

Bitboard Engine::opponent_pieces(const Position& pos) {
  return pos.pieces(Engine::opponent_color());
}



void Engine::propagate_compass_on_position(const Position& pos, Bitboard& bb, Square initial_sq, Compass direction) {
  enum Square next_square = initial_sq + direction;
  Bitboard opponent_bb = opponent_pieces(pos);
  Bitboard engine_bb = engine_pieces(pos);
  while( !Engine::square_is_edge(next_square) ) {
    Bitboard next_sq_bb = SQUARE_TO_BB(next_square);
    if( next_sq_bb & engine_bb) return bb; // Do not capture your own piece!
    if( opponent_bb & next_sq_bb ) break;
    bb |= next_sq_bb;
    next_square += direction;
  }
  bb |= SQUARE_TO_BB(next_square); // Make sure to include the edge square or 'first' enemy piece
}

Bitboard Engine::pseudo_legal_bishop_move_bitboard(const Position& pos, enum Square sq ) {
  if( sq == NO_SQUARE ) return EMPTY_BB;
  Bitboard pattern = EMPTY_BB;

  if( Engine::check_direction(sq, NORTH_EAST) ) {
    Engine::propagate_compass_on_position(pos,pattern,sq,NORTH_EAST);
  }
  if( Engine::check_direction(sq, NORTH_WEST) ) {
    Engine::propagate_compass_on_position(pos,pattern,sq,NORTH_WEST);
  }
  if( Engine::check_direction(sq,SOUTH_EAST) ) {
    Engine::propagate_compass_on_position(pos,pattern,sq,SOUTH_EAST);
  }
  if( Engine::check_direction(sq,SOUTH_WEST) ) {
    Engine::propagate_compass_on_position(pos,pattern,sq,SOUTH_WEST);
  }

  Bitboard my_pieces = engine_pieces();
  return pattern & (~my_piece);
}
Bitboard Engine::pseudo_legal_rook_move_bitboard( const Position& pos, enum Square sq ) {
  if( sq == NO_SQUARE ) return EMPTY_BB;
  Bitboard pattern = EMPTY_BB;

  if( Engine::check_direction(sq,NORTH) ) {
    Engine::propagate_compass_on_position(pos,pattern,sq,NORTH);
  }
  if( Engine::check_direction(sq,SOUTH) ) {
    Engine::propagate_compass_on_position(pos,pattern,sq,SOUTH);
  }
  if( Engine::check_direction(sq,EAST) ) {
    Engine::propagate_compass_on_position(pos,pattern,sq,EAST);
  }
  if( Engine::check_direction(sq,WEST) ) {
    Engine::propagate_compass_on_position(pos,pattern,sq,WEST);
  }

  return pattern;
}

Bitboard Engine::pseudo_legal_knight_move_bitboard( const Position& pos, enum Square sq) {
  /*
    +---+---+---+---+---+---+---+
    |   |   |NNW|   |NNE|   |   |
    +---+---+---+---+---+---+---+
    |   |NWW|   |   |   |NEE|   |
    +---+---+---+---+---+---+---+
    |   |   |   | K |   |   |   |
    +---+---+---+---+---+---+---+
    |   |SWW|   |   |   |SEE|   |
    +---+---+---+---+---+---+---+
    |   |   |SSW|   |SSE|   |   |
    +---+---+---+---+---+---+---+
  */
  if( sq == NO_SQUARE ) return EMPTY_BB;
  Bitboard pattern = EMPTY_BB;

  if( Engine::check_direction(sq,NORTH_NORTH_EAST) ) {
    pattern |= SQUARE_TO_BB(sq+NORTH_NORTH_EAST);
  }
  if( Engine::check_direction(sq,NORTH_NORTH_WEST) ) {
    pattern |= SQUARE_TO_BB(sq+NORTH_NORTH_WEST);
  }
  if( Engine::check_direction(sq,NORTH_EAST_EAST) ) {
    pattern |= SQUARE_TO_BB(sq+NORTH_EAST_EAST);
  }
  if( Engine::check_direction(sq,SOUTH_EAST_EAST) ) {
    pattern |= SQUARE_TO_BB(sq+SOUTH_EAST_EAST);
  }
  if( Engine::check_direction(sq,SOUTH_SOUTH_EAST) ) {
    pattern |= SQUARE_TO_BB(sq+SOUTH_SOUTH_EAST);
  }
  if( Engine::check_direction(sq,SOUTH_SOUTH_WEST) ) {
    pattern |= SQUARE_TO_BB(sq+SOUTH_SOUTH_WEST);
  }
  if( Engine::check_direction(sq,SOUTH_WEST_WEST) ) {
    pattern |= SQUARE_TO_BB(sq+SOUTH_WEST_WEST);
  }
  if( Engine::check_direction(sq,NORTH_WEST_WEST) ) {
    pattern |= SQUARE_TO_BB(sq+NORTH_WEST_WEST);
  }

  pattern &= ~(Engine::engine_pieces(pos)); // Dont want to capture own pieces!!!

  return pattern;
}
Bitboard Engine::pseudo_legal_king_move_bitboard( const Position& pos, enum Square sq) {
  // DOES NOT ACCOUNT FOR CASTLING!!!
  if( sq == NO_SQUARE ) return EMPTY_BB;
  Bitboard pattern = EMPTY_BB;

  if( Engine::check_direction(sq,NORTH) )  pattern |= SQUARE_TO_BB(sq+NORTH);
  if( Engine::check_direction(sq,EAST) )  pattern |= SQUARE_TO_BB(sq+EAST);
  if( Engine::check_direction(sq,WEST) )  pattern |= SQUARE_TO_BB(sq+WEST);
  if( Engine::check_direction(sq,SOUTH) )  pattern |= SQUARE_TO_BB(sq+SOUTH);
  if( Engine::check_direction(sq,NORTH_WEST) )  pattern |= SQUARE_TO_BB(sq+NORTH_WEST);
  if( Engine::check_direction(sq,NORTH_EAST) )  pattern |= SQUARE_TO_BB(sq+NORTH_EAST);
  if( Engine::check_direction(sq,SOUTH_EAST) )  pattern |= SQUARE_TO_BB(sq+SOUTH_EAST);
  if( Engine::check_direction(sq,SOUTH_WEST) )  pattern |= SQUARE_TO_BB(sq+SOUTH_WEST);

  pattern &= ~(Engine::engine_pieces(pos)); // Dont want to capture our own pieces!

  return pattern;
}
Bitboard Engine::pseudo_legal_queen_move_bitboard( const Position& pos, enum Square sq) {
  if( sq == NO_SQUARE ) return EMPTY_BB;
  Bitboard pattern = EMPTY_BB;

  if( Engine::check_direction(sq,NORTH) ) {
    Engine::propagate_compass_on_position(pos, pattern,sq,NORTH);
  }
  if( Engine::check_direction(sq,EAST) ) {
    Engine::propagate_compass_on_position(pos, pattern,sq,EAST);
  }
  if( Engine::check_direction(sq,WEST) ) {
    Engine::propagate_compass_on_position(pos, pattern,sq,WEST);
  }
  if( Engine::check_direction(sq,SOUTH) ) {
    Engine::propagate_compass_on_position(pos, pattern,sq,SOUTH);
  }
  if( Engine::check_direction(sq,NORTH_EAST) ) {
    Engine::propagate_compass_on_position(pos, pattern,sq,NORTH_EAST);
  }
  if( Engine::check_direction(sq,NORTH_WEST) ) {
    Engine::propagate_compass_on_position(pos, pattern,sq,NORTH_WEST);
  }
  if( Engine::check_direction(sq,SOUTH_EAST) ) {
    Engine::propagate_compass_on_position(pos, pattern,sq,SOUTH_EAST);
  }
  if( Engine::check_direction(sq,SOUTH_WEST) ) {
    Engine::propagate_compass_on_position(pos, pattern,sq,SOUTH_WEST);
  }
  
  return pattern;
}

Bitboard Engine::pseudo_legal_pawn_move_bitboard( const Position& pos,  enum Square sq ) {
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
  if( Engine::engine_color() == white_p ) { // White pieces

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

Square Engine::single_pop_bitboard_to_square( Bitboard one_pop_bb ) {
  if(one_pop_bb == 0) return NO_SQUARE;
  enum Square sq = a1;
  while( one_pop_bb & RANK_1_BB == 0 ) {
    sq+=NORTH;
    one_pop_bb << 8;
  }
  while(one_pop_bb & FILE_A_BB == 0 ) {
    sq+=EAST;
    one_pop_bb << 1;
  }
  return sq;
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

std::vector<Move> Engine::generate_pseudo_legal_special_moves(const Position& pos) {
  std::vector<Move> special_moves;

  // Castling
  if( Engine::engine_color() == white_p ) {
    if( pos.castle(white_short_castle) ) {
      special_moves.push_back(Move(NO_SQUARE,NO_SQUARE,SHORT_CASTLE));
    }
    if( pos.castle(white_long_castle) ) {
      special_moves.push_back(Move(NO_SQUARE,NO_SQUARE,LONG_CASTLE));
    }
  } else {
    if( pos.castle(black_short_castle) ) {
      special_moves.push_back(Move(NO_SQUARE,NO_SQUARE,SHORT_CASTLE));
    }
    if( pos.castle(black_long_castle) ) {
      special_moves.push_back(Move(NO_SQUARE,NO_SQUARE,LONG_CASTLE));
    }
  }

  // en Passant
  // ASSUMPTION: If the En Passant Target Square is specified in position object,
  //              then we know that the previous move was a double pawn push!!!
  enum Square ep_target = pos.en_passant_target();
  Bitboard engine_pawns = Engine::engine_pieces() & pos.pieces(pawn_t);
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
std::vector<Move> Engine::generate_pseudo_legal_moves(const Position& pos, Piece p, Square sq) {
  /*
    returns vector of pseudo legal moves for the piece at specified square
    at given position pos. 
    DOES NOT CHECK IF THE KING WILL BE ATTACKED AFTER MOVE
    DOES NOT GENERATE ANY CASTLING MOVES
    DOES NOT GENERATE EN PASSANT MOVES
  */
  
  Bitboard move_pattern = EMPTY_BB;
  switch(p) {
    case pawn_p:
      move_pattern = Engine::pseudo_legal_pawn_move_bitboard(pos,sq);
      break;
    case knight_p:
      move_pattern = Engine::pseudo_legal_knight_move_bitboard(pos,sq);
      break;
    case bishop_p:
      move_pattern = Engine::pseudo_legal_bishop_move_bitboard(pos,sq);
      break;
    case rook_p:
      move_pattern = Engine::pseudo_legal_rook_move_bitboard(pos,sq);
      break;
    case queen_p:
      move_pattern = Engine::pseudo_legal_queen_move_bitboard(pos,sq);
      break;
    case king_p:
      move_pattern = Engine::pseudo_legal_king_move_bitboard(pos,sq);
      break;
    default:
      break;
  }

  std::vector<Square> final_squares = Engine::bitboard_to_squares(move_pattern);
  std::vector<Move> pseudo_legal_moves;
  for( int i = 0; i < final_squares.size(); i++) {
    Move m(sq,final_squares(i),NO_FLAG);
    pseudo_legal_moves.push_back(m);
  }

  return pseudo_legal_moves;
}

bool Engine::apply_move_to_position(Position& pos, const Move& m) {
  /* Attempts to apply Move <m> to Position <pos>;
      returns true if successful
      false otherwise.
      - DOES NOT CHECK IF THE MOVE IS LEGAL!!!
  */

  enum Square init_sq = m.initial_square();
  enum Square final_sq = m.final_square();
  bool isPawnMove = false;
  
  switch( m.flag() ){

    case NO_FLAG:
      /* not promotion, castle, nor en passant */
      enum Piece tau = pos.piece_at_square(init_sq);
      if( tau == none_p ) return false;
      else if( tau == pawn_p ) { // Check if move enables en_passant!
        isPawnMove = true;
        enum Compass pawn_move_direction= (Engine::engine_color(pos) == white_p ) ? (NORTH):(SOUTH);
        bool double_push = (final_sq == init_sq + (2*pawn_move_direction));
        if( double_push ) {
          bool check_west_square = (SQUARE_TO_BB(final_sq) & FILE_A_BB) == 0;
          bool check_east_square = (SQUARE_TO_BB(final_sq) & FILE_H_BB) == 0;
          Bitboard opponent_pawns = Engine::opponent_pieces(pos) & pos.pieces(pawn_p);
          if( check_west_square ) {
            if( SQUARE_TO_BB(final_sq + WEST) & opponent_pawns ) {
              pos.en_passant_target_square = init_sq + pawn_move_direction;
            }
          }
          if( check_east_square ) {
            if( SQUARE_TO_BB(final_sq + EAST) & opponent_pawns) {
              pos.en_passant_target_square = init_sq + pawn_move_direction;
            }
          }
        }
      } else if( tau == king_p ) {
        // Must revoke castling rights
        pos.revoke_castle(Engine::engine_color(pos));
      }

      pos.remove(init_sq);
      pos.remove(final_sq);
      pos.set(final_sq,Engine::engine_color(pos));
      pos.set(final_sq,tau);
      break;
    case PROMOTE_KNIGHT:
      pos.remove(init_sq);
      pos.set(final_sq,Engine::engine_color(pos));
      pos.set(final_sq,knight_p);
      break;
    case PROMOTE_BISHOP:
      pos.remove(init_sq);
      pos.set(final_sq,Engine::engine_color(pos));
      pos.set(final_sq,bishop_p);
      break;
    case PROMOTE_ROOK:
      pos.remove(init_sq);
      pos.set(final_sq,Engine::engine_color(pos));
      pos.set(final_sq,rook_p);
      break;
    case PROMOTE_QUEEN:
      pos.remove(init_sq);
      pos.set(final_sq,Engine::engine_color(pos));
      pos.set(final_sq,queen_p);
      break;
    case LONG_CASTLE:
      if( Engine::engine_color(pos) == white_p) {
        pos.remove(e1); // king initial sq
        pos.remove(a1); // rook initial sq
        pos.set(c1,king_p);
        pos.set(c1,white_p);
        pos.set(d1,rook_p);
        pos.set(d1,white_p);
        // Remove castle rights
        pos.revoke_castle(white_p);
      } else {
        pos.remove(e8); // king initial sq
        pos.remove(a8); // rook initial sq
        pos.set(c8,king_p);
        pos.set(c8,black_p);
        pos.set(d8,rook_p);
        pos.set(d8,black_p);
        pos.revoke_castle(black_p);
      }
      break;
    case SHORT_CASTLE:
      if( Engine::engine_color(pos) == white_p ) {
        pos.remove(e1);
        pos.remove(h1);
        pos.set(g1,king_p);
        pos.set(g1,white_p);
        pos.set(e1,rook_p);
        pos.set(e1,white_p);
        pos.revoke_castle(white_p);
      } else {
        pos.remove(e8);
        pos.remove(h8);
        pos.set(g8,king_p);
        pos.set(g8,white_p);
        pos.set(e8,rook_p);
        pos.set(e8,white_p);
        pos.revoke_castle(black_p);
      }
      break;
    case EN_PASSANT:
      isPawnMove = true
      enum Compass enemy_pawn_move_direction = (Engine::engine_color(pos) == white_p ) ? SOUTH:NORTH;
      pos.remove(init_sq); // remove our pawn
      pos.remove(final_sq + enemy_pawn_move_direction); // remove opponent pawn
      pos.set(final_sq,Engine::engine_color(pos));
      pos.set(final_sq,pawn_p);
      break;
    default:
      return false;
  }

  // Swap side to move!
  pos.side_to_move = (pos.side_to_move == white_p) ? black_p : white_p;
  pos.full_move_counter++;

  bool isCapture = (SQUARE_TO_BB(final_sq) & Engine::opponent_pieces(pos)) > 0;
  if( isCapture || isPawnMove ) {
    pos.half_move_clock = 0;
  } else {
    pos.half_move_clock++;
  }

  return true;
}

bool Engine::legal_move(const Position& pos, const Move& m) {
  // Only need to check if after the move happens, we're not in check!
  Position* duplicate = Position::copy(pos);
  if( !Engine::apply_move_to_position(*copy, m) ) {
    free(duplicate);
    return false;
  }

  Bitboard engine_king = duplicate->pieces(Engine::engine_color(pos)) & duplicate->pieces(king_p);
  Square engine_king_square = Engine::single_pop_bitboard_to_square(engine_king);

  Bitboard diagonal_attackers = duplicate->pieces(Engine::opponent_color(pos))
                                & duplicate->pieces(queen_p)
                                & duplicate->pieces(king_p) 
                                & duplicate->pieces(bishop_p);
  Bitboard king_diagonals = engine_king;
  if( Engine::check_direction(engine_king_square, NORTH_EAST)) {
    Engine::propagate_compass_on_position(*duplicate,king_diagonals,engine_king_square,NORTH_EAST);
  }
  if( Engine::check_direction(engine_king_square, NORTH_WEST)) {
    Engine::propagate_compass_on_position(*duplicate,king_diagonals,engine_king_square,NORTH_WEST);
  }
  if( Engine::check_direction(engine_king_square, SOUTH_EAST)) {
    Engine::propagate_compass_on_position(*duplicate,king_diagonals,engine_king_square,SOUTH_EAST);
  }
  if( Engine::check_direction(engine_king_square, SOUTH_WEST)) {
    Engine::propagate_compass_on_position(*duplicate,king_diagonals,engine_king_square,SOUTH_WEST);
  }
  if( king_diagonals & diagonal_attackers ) return false;

  Bitboard flat_attackers = duplicate->pieces(Engine::opponent_color(pos)) 
                            & duplicate->pieces(rook_p)
                            & duplicate->pieces(king_p)
                            & duplicate->pieces(queen_p);
  Bitboard king_flats = engine_king;
  if( Engine::check_direction(engine_king_square,NORTH)) {
    Engine::propagate_compass_on_position(*duplicate,king_flats,engine_king_square,NORTH);
  }

  if( Engine::check_direction(engine_king_square,EAST)) {
    Engine::propagate_compass_on_position(*duplicate,king_flats,engine_king_square,EAST);
  }
  if( Engine::check_direction(engine_king_square,WEST)) {
    Engine::propagate_compass_on_position(*duplicate,king_flats,engine_king_square,WEST);
  }
  if( Engine::check_direction(engine_king_square,SOUTH)) {
    Engine::propagate_compass_on_position(*duplicate,king_flats,engine_king_square,SOUTH);
  }
  if( king_flats & flat_attackers ) return false;

  //TODO Check for knight attackers
  //TODO Check for pawn attackers



  free(duplicate);
  return false;
}

bool Engine::check_direction( Square sq, Compass dir ) {
  /* Returns true if a piece may move one step in the direction <dir>
      without hitting an edge */
  if( sq == NO_SQUARE ) return false;
  Bitboard sq_bb = SQUARE_TO_BB(sq);
  switch(dir) {
    case NORTH:
      return (sq_bb & NORTH_BB > 0);
    case EAST:
      return (sq_bb & EAST_BB > 0);
    case WEST:
      return (sq_bb & WEST_BB > 0);
    case SOUTH:
      return (sq_bb & SOUTH_BB > 0);
    case NORTH_EAST:
      return (sq_bb & NORTH_EAST_BB > 0);
    case NORTH_WEST:
      return (sq_bb & NORTH_WEST_BB > 0);
    case SOUTH_EAST:
      return (sq_bb & SOUTH_EAST_BB > 0);
    case SOUTH_WEST:
      return (sq_bb & SOUTH_WEST_BB > 0);
    case NORTH_NORTH_EAST:
      return (sq_bb & NORTH_NORTH_EAST_BB > 0);
    case NORTH_NORTH_WEST:
      return (sq_bb & NORTH_NORTH_WEST_BB > 0);
    case NORTH_EAST_EAST:
      return (sq_bb & NORTH_EAST_EAST_BB > 0);
    case SOUTH_EAST_EAST:
      return (sq_bb & SOUTH_EAST_EAST > 0);
    case SOUTH_SOUTH_EAST:
      return (sq_bb & SOUTH_SOUTH_EAST > 0);
    case SOUTH_SOUTH_WEST:
      return (sq_bb & SOUTH_SOUTH_WEST > 0);
    case SOUTH_WEST_WEST:
      return (sq_bb & SOUTH_WEST_WEST > 0);
    case NORTH_WEST_WEST:
      return (sq_bb & NORTH_WEST_WEST > 0);
    default:
      return false; // Error!?
  }
  return false;
}

std::vector<Move> Engine::generate_legal_moves(const Position& pos) {
  //TODO
  return NULL;
}