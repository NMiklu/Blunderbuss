#!/bin/sh
g++ ../tools/bitboard_debug.cpp ../../src/util.cpp \
  ../../src/position.cpp position_tests.cpp \
  -Wall -Wpedantic -Werror -lm -g -o position_tests.o
