#!/bin/sh
g++ ../tools/bitboard_debug.cpp ../../src/position.cpp position_tests.cpp \
  -Wall -Werror -Wpedantic -lm -g -DPOSITION_TEST -o position_tests.o
