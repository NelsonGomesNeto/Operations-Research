#include "Sudoku.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <random>
#include <time.h>

Sudoku::Sudoku(int sqrtSize) {
  this->sqrtSize = sqrtSize;
  this->grid = new int***[sqrtSize];
  for (int i = 0; i < sqrtSize; i ++) {
    this->grid[i] = new int**[sqrtSize];
    for (int j = 0; j < sqrtSize; j ++) {
      this->grid[i][j] = new int*[sqrtSize];
      for (int k = 0; k < sqrtSize; k ++) {
        this->grid[i][j][k] = new int[sqrtSize];
        for (int l = 0; l < sqrtSize; l ++) this->grid[i][j][k][l] = 0;
      }
    }
  }

  this->size = sqrtSize * sqrtSize;
  this->board = new int*[size];
  for (int i = 0; i < size; i ++) {
    this->board[i] = new int[size];
    for (int j = 0; j < size; j ++) this->board[i][j] = 0;
  }

  this->lines = new bool*[size], this->nextInLines = new int*[size],
  this->columns = new bool*[size], this->nextInColumns = new int*[size];
  for (int i = 0; i < size; i ++) {
    this->lines[i] = new bool[size + 1], this->nextInLines[i] = new int[size + 1];
    this->columns[i] = new bool[size + 1], this->nextInColumns[i] = new int[size + 1];
    for (int j = 1; j <= size; j ++)
      this->lines[i][j] = this->columns[i][j] = false,
      this->nextInLines[i][j] = this->nextInColumns[i][j] = j % size + 1;
  }
  this->grids = new bool**[sqrtSize], this->nextInGrids = new int**[sqrtSize];
  for (int i = 0; i < sqrtSize; i ++) {
    this->grids[i] = new bool*[sqrtSize], this->nextInGrids[i] = new int*[sqrtSize];
    for (int j = 0; j < sqrtSize; j ++) {
      this->grids[i][j] = new bool[size + 1], this->nextInGrids[i][j] = new int[size + 1];
      for (int k = 1; k <= size; k ++)
        this->grids[i][j][k] = false, this->nextInGrids[i][j][k] = k % size + 1;
    }
  }

  srand(time(NULL));
}

void Sudoku::printBoard() {
  int digits = 0, aux = this->size; while (aux) aux /= 10, digits ++;
  for (int i = 0; i < this->size; i ++)
    for (int j = 0; j < this->size; j ++)
      printf("%*d%c", digits, this->board[i][j], j < this->size - 1 ? ' ' : '\n');
}

void Sudoku::printGrids() {
  // int digits = 0, aux = this->sqrtSize; while (aux) aux /= 10, digits ++;
  // for (int i = 0; i < this->sqrtSize; i ++)
  //   for (int j = 0; j < this->sqrtSize; j ++)
  // TODO
}

bool Sudoku::generatePuzzle(int i, int j) {
  if (i == this->size) return(true);
  int number = rand() % this->size + 1, ii = i / this->sqrtSize, jj = j / this->sqrtSize;
  for (int n = this->nextInLines[i][number], a = 0; a < this->size; n = this->nextInLines[i][n], a ++) {
    if (this->lines[i][n] || this->columns[j][n] || this->grids[ii][jj][n]) continue;

    int aux = this->nextInLines[i][((n - 1) < 1) ? this->size : n - 1];
    this->board[i][j] = n, this->lines[i][n] = this->columns[j][n] = this->grids[ii][jj][n] = true;
    // this->nextInLines[i][((n - 1) < 1) ? this->size : n - 1] = n % this->size + 1, this->nextInColumns[j][((n - 1) < 1) ? this->size : n - 1] = n % this->size + 1, this->nextInGrids[ii][jj][((n - 1) < 1) ? this->size : n - 1] = n % this->size + 1;

    if (generatePuzzle(i + (j + 1 == this->size), (j + 1) % this->size)) return(true);

    this->board[i][j] = n, this->lines[i][n] = this->columns[j][n] = this->grids[ii][jj][n] = false;
    // this->nextInLines[i][((n - 1) < 1) ? this->size : n - 1] = this->nextInColumns[j][((n - 1) < 1) ? this->size : n - 1] = this->nextInGrids[ii][jj][((n - 1) < 1) ? this->size : n - 1] = aux;
  }
  return(false);
}