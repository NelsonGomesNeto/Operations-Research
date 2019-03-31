#include "Sudoku.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <random>
#include <time.h>
#include <algorithm>

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

  this->visited = new int*[size];
  for (int i = 0; i < size; i ++) {
    this->visited[i] = new int[size];
    for (int j = 0; j < size; j ++) this->visited[i][j] = 0;
  }

  this->numbersCount = new pair<int, int>[size + 1]; this->numbersCount[0] = {0, 0};
  for (int i = 1; i <= size; i ++) this->numbersCount[i] = {-size, i};

  srand(time(NULL));
}

void Sudoku::printBoard() {
  int digits = 0, aux = this->size; while (aux) aux /= 10, digits ++;
  for (int i = 0; i < this->size; i ++)
  {
    for (int j = 0; j < this->size; j ++)
    {
      printf("%*d%c", digits, this->board[i][j], j < this->size - 1 ? ' ' : '\n');
      if (j > 0 && j < this->size - 1 && (j + 1) % this->sqrtSize == 0) printf("| ");
    }
    if (i > 0 && i < this->size - 1 && (i + 1) % this->sqrtSize == 0)
      for (int k = 0, end = this->size * (digits + 1) + (this->sqrtSize - 1)*2; k < end; k ++) printf("%c", k < end - 1 ? '-' : '\n');
  }
}

void Sudoku::printGrids() {
  // int digits = 0, aux = this->sqrtSize; while (aux) aux /= 10, digits ++;
  // for (int i = 0; i < this->sqrtSize; i ++)
  //   for (int j = 0; j < this->sqrtSize; j ++)
  // TODO
}

void Sudoku::fillDiagonals() {
  for (int i = 1; i <= this->size; i ++) this->numbersCount[i] = {-this->size, i};
  for (int i = 0; i < this->size; i ++) for (int j = 1; j <= this->size; j ++) this->lines[i][j] = this->columns[i][j] = false, this->board[i][j - 1] = this->visited[i][j - 1] = 0;
  for (int i = 0; i < this->sqrtSize; i ++) for (int j = 0; j < this->sqrtSize; j ++) for (int k = 1; k <= this->size; k ++) this->grids[i][j][k] = false;

  for (int k = 0; k < this->sqrtSize; k ++)
    for (int i = 0; i < this->sqrtSize; i ++)
      for (int j = 0; j < this->sqrtSize; j ++) {
        int number = rand() % this->size + 1, l = k*this->sqrtSize + i, c = k*this->sqrtSize + j;
        for (int n = number % this->size + 1; ; n = n % this->size + 1) {
          if (this->grids[k][k][n]) continue;
          this->board[l][c] = n;
          this->lines[l][n] = this->columns[c][n] = this->grids[k][k][n] = true;
          this->numbersCount[n].first ++;
          break;
        }
      }
}

int iterations;
void Sudoku::generatePuzzle() {
  int r = 0;
  do {
    this->fillDiagonals();
    // printf("Repeating %d\n", r ++);
    iterations = 0;
  } while (!this->generatePuzzleDFS());
  printf("%s\n", this->isValid() ? "Valid" : "Invalid");
}

// bool generatePuzzleAStar() {

// }

bool Sudoku::generatePuzzleDFS(int i, int j) {
  if (i == this->size) return(true);
  // if (iterations > 10000000) return(false);
  if (this->board[i][j]) return(this->generatePuzzleDFS(i + (j + this->sqrtSize == this->size), (j + this->sqrtSize) % this->size));
  // this->visited[i][j] ++;
  // if (this->visited[i][j] > 100000) return(false);
  // iterations ++;

  int number = rand() % this->size + 1, ii = i / this->sqrtSize, jj = j / this->sqrtSize;
  int rr = rand() % 100;//((rand() % 10000) > 3000);
  // double pp = 1.0 - (double) (i*this->size + j) / ((this->size - 1)*(this->size - 1));
  // int rep = 0 + log2(this->size * pp);
  int rep;
  if (rr < 50) rep = 2;
  else rep = 1;

  for (int n = number, a = 0, aa = 0; a < this->size; n = n % this->size + 1, a ++) {
    if (this->lines[i][n] || this->columns[j][n] || this->grids[ii][jj][n]) continue; else aa ++;

    this->board[i][j] = n, this->lines[i][n] = this->columns[j][n] = this->grids[ii][jj][n] = true;
    if (this->generatePuzzleDFS(i + (j + 1 == this->size), (j + 1) % this->size)) return(true);
    this->board[i][j] = 0, this->lines[i][n] = this->columns[j][n] = this->grids[ii][jj][n] = false;

    if (aa == rep) return(false);
  }
  return(false);
}

bool Sudoku::isValid() {
  bool linesCount[this->size + 1], columnsCount[this->size + 1], gridsCount[this->size + 1];
  for (int i = 0; i < this->size; i ++) {
    memset(linesCount, false, sizeof(linesCount)), memset(columnsCount, false, sizeof(columnsCount));
    for (int j = 0; j < this->size; j ++) {
      if (linesCount[this->board[i][j]] || columnsCount[this->board[j][i]]) return(false);
      linesCount[this->board[i][j]] = columnsCount[this->board[j][i]] = true;
    }
  }
  for (int i = 0; i < sqrtSize; i ++)
    for (int j = 0; j < sqrtSize; j ++) {
      memset(gridsCount, false, sizeof(gridsCount));
      for (int k = 0; k < sqrtSize; k ++)
        for (int m = 0; m < sqrtSize; m ++) {
          int ii = i*sqrtSize + k, jj = j*sqrtSize + m;
          if (gridsCount[this->board[ii][jj]]) return(false);
          gridsCount[this->board[ii][jj]] = true;
        }
    }
  return(true);
}