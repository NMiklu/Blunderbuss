#include "representation.h"


Move::Move(enum Square init, enum Square final, MoveFlag flag) {
  uint16_t tau = static_cast<int>(init);
  uint16_t phi = static_cast<int>(final);
  // << shift logical left
  tau << 10;
  phi << 4;
  tau &= INITIAL_SQUARE;
  phi &= FINAL_SQUARE;
  this->_move_data |= tau;
  this->_move_data |= phi;
  this->_move_data |= flag;
}

enum Square Move::get_inital_square() {
  uint16_t temp = this->_move_data;
  temp >> 10;
  int cast_item = (int) temp;
  enum Square sq = static_cast<Square>(cast_item);
  return sq;
}
enum Square Move::get_final_square() {
  uint16_t temp = this->_move_data;
  temp << 6;
  temp >> 10;
  int cast_item = (int) temp;
  enum Square sq = static_cast<Square>(cast_item);
  return sq;
}
MoveFlag Move::get_flag() {
  return static_cast<MoveFlag>((this->_move_data & SPECIAL_FLAG));
}