#!/bin/sh
g++ ../../src/misc.cpp ../../src/position.cpp position_tests.cpp \
  -Wall -Werror -Wpedantic -lm -g -o position_tests.o
