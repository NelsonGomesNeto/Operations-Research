#ifndef SUDOKU_HPP
#define SUDOKU_HPP

class Sudoku {
private:
  int **board, ****grid, size, sqrtSize;
  bool **lines, **columns, ***grids; // used i-th number
  int **nextInLines, **nextInColumns, ***nextInGrids;
public:
  Sudoku(int sqrtSize);
  void printBoard();
  void printGrids();
  bool generatePuzzle(int i = 0, int j = 0);
};

#endif