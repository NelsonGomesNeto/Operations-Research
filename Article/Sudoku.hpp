#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <utility>
#include <vector>
#include <ilcplex/ilocplex.h>
using namespace std;

class Sudoku {
private:
  int **board, size, sqrtSize;
  bool **lines, **columns, ***grids; // used i-th number
  vector<int> **allowedNumbers;
  int **visited, **fixes;
  pair<int, int> *numbersCount; // first numberCount, second number
  pair<int, pair<int, int>> *emptyCells; int emptyCellsSize;
  int iterationsLimit, alpha;
  void generateBasePuzzle();
  void scrambleCells();
  void removeCells(int amount);
  bool generatePuzzleDFS(int i = 0, int j = 0);
  bool generatePuzzleDFSEmptyCells(int e = 0);
  bool generatePuzzleCPLEXUtil();
  void fillRestrictions();
  void fillDiagonals();
  bool fillAllowedNumbers(int i = 0, int j = 0, bool avoidDone = true);
  void fillEmptyCells();
  void fixBoard(int i = 0, int j = 0);
public:
  Sudoku(int sqrtSize, int alpha = 100, int iterationsLimit = 1000000);
  void printBoard();
  void printGrids();
  void generatePuzzle();
  void generatePuzzleCPLEX();
  bool isValid();
};

#endif