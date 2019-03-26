#include <stdio.h>
#include <math.h>
#include "Sudoku.hpp"

int main(int argc, char *argv[]) {
  Sudoku *sudoku = new Sudoku(atoi(argv[1]));
  sudoku->generatePuzzle();
  sudoku->printBoard();
  return(0);
}