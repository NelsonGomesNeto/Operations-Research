#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <utility>
#include <vector>
// #include <ilcplex/ilocplex.h>
using namespace std;

class Sudoku {
private:
  int **board, size, sqrtSize;
  bool **lines, **columns, ***grids; // used i-th number
  vector<int> **allowedNumbers;
  int **visited, **fixes;
  pair<int, int> *numbersCount; // first numberCount, second number
  pair<int, pair<int, int>> *emptyCells; int emptyCellsSize;
  // void generateBasePuzzle();
  bool generatePuzzleDFS(int i = 0, int j = 0);
  bool generatePuzzleDFSEmptyCells(int e = 0);
  bool generatePuzzleCPLEX();
  void fillDiagonals();
  bool fillAllowedNumbers(int i = 0, int j = 0, bool avoidDone = true);
  void fillEmptyCells();
  void fixBoard(int i = 0, int j = 0);
public:
  Sudoku(int sqrtSize);
  void printBoard();
  void printGrids();
  void generatePuzzle();
  bool isValid();
};

#endif