#include <stdio.h>
#include <iostream>
#include <math.h>
#include "Sudoku.hpp"

int main(int argc, char *argv[]) {
  Sudoku *sudoku;
  if (argc == 2)  sudoku = new Sudoku(atoi(argv[1]));
  else sudoku = new Sudoku(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
  if (argc == 5) sudoku->generatePuzzleCPLEX();
  else sudoku->generatePuzzle();
  sudoku->printBoard();
  return(0);
}