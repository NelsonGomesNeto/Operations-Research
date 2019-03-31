#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <utility>
using namespace std;

class Sudoku {
private:
  int **board, ****grid, size, sqrtSize;
  bool **lines, **columns, ***grids; // used i-th number
  int **nextInLines, **nextInColumns, ***nextInGrids;
  int **visited;
  pair<int, int> *numbersCount; // first numberCount, second number
  bool generatePuzzleDFS(int i = 0, int j = 0);
  void fillDiagonals();
public:
  Sudoku(int sqrtSize);
  void printBoard();
  void printGrids();
  void generatePuzzle();
  bool isValid();
};

#endif