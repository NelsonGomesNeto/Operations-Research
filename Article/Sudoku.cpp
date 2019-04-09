#include "Sudoku.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <random>
#include <time.h>
#include <algorithm>

Sudoku::Sudoku(int sqrtSize, int alpha, int iterationsLimit) {
  this->sqrtSize = sqrtSize;
  this->alpha = alpha;
  this->iterationsLimit = iterationsLimit;

  this->size = sqrtSize * sqrtSize;
  this->board = new int*[size];
  for (int i = 0; i < size; i ++) {
    this->board[i] = new int[size];
    for (int j = 0; j < size; j ++) this->board[i][j] = 0;
  }

  this->lines = new bool*[size], this->columns = new bool*[size];
  for (int i = 0; i < size; i ++) {
    this->lines[i] = new bool[size + 1], this->columns[i] = new bool[size + 1];
    for (int j = 1; j <= size; j ++) this->lines[i][j] = this->columns[i][j] = false;
  }
  this->grids = new bool**[sqrtSize];
  for (int i = 0; i < sqrtSize; i ++) {
    this->grids[i] = new bool*[sqrtSize];
    for (int j = 0; j < sqrtSize; j ++) {
      this->grids[i][j] = new bool[size + 1];
      for (int k = 1; k <= size; k ++) this->grids[i][j][k] = false;
    }
  }

  this->visited = new int*[size], this->fixes = new int*[size];
  for (int i = 0; i < size; i ++) {
    this->visited[i] = new int[size], this->fixes[i] = new int[size];
    for (int j = 0; j < size; j ++) this->visited[i][j] = this->fixes[i][j] = 0;
  }

  this->numbersCount = new pair<int, int>[size + 1]; this->numbersCount[0] = {0, 0};
  for (int i = 1; i <= size; i ++) this->numbersCount[i] = {-size, i};

  this->allowedNumbers = new vector<int>*[size];
  for (int i = 0; i < size; i ++) {
    this->allowedNumbers[i] = new vector<int>[size];
    for (int j = 0; j < size; j ++) this->allowedNumbers[i][j].clear();
  }

  this->emptyCells = new pair<int, pair<int, int>>[size*size];
  srand(time(NULL));
  // srand(0);
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

void Sudoku::fillEmptyCells() {
  this->emptyCellsSize = 0;
  for (int i = 0; i < this->size; i ++)
    for (int j = 0; j < this->size; j ++) {
      if (this->board[i][j]) continue;
      this->emptyCells[this->emptyCellsSize ++] = {(int) this->allowedNumbers[i][j].size(), {i, j}};
    }

  // for (int j = 0; j < this->size; j ++)
  //   for (int i = 0; i < this->size; i ++) {
  //     if (this->board[i][j]) continue;
  //     this->emptyCells[this->emptyCellsSize ++] = {(int) this->allowedNumbers[i][j].size(), {i, j}};
  //   }

  // bool vstd[this->size][this->size]; memset(vstd, false, sizeof(vstd));
  // int i = this->size >> 1, j = this->size >> 1;
  // for (int k = 0; k < this->size; k ++) {
  //   for (int m = 0; m <= k; m ++) { // up
  //     if (!this->board[i][j] && !vstd[i][j]) this->emptyCells[this->emptyCellsSize ++] = {(int) this->allowedNumbers[i][j].size(), {i, j}}, vstd[i][j] = true;
  //     i --;
  //     if (i < 0) { i = 0; break; }
  //   }
  //   for (int m = 0; m <= k; m ++) { // right
  //     if (!this->board[i][j] && !vstd[i][j]) this->emptyCells[this->emptyCellsSize ++] = {(int) this->allowedNumbers[i][j].size(), {i, j}}, vstd[i][j] = true;
  //     j ++;
  //     if (j >= this->size) { j = this->size - 1; break; }
  //   }
  //   for (int m = 0; m <= k + 1; m ++) { // down
  //     if (!this->board[i][j] && !vstd[i][j]) this->emptyCells[this->emptyCellsSize ++] = {(int) this->allowedNumbers[i][j].size(), {i, j}}, vstd[i][j] = true;
  //     i ++;
  //     if (i >= this->size) { i = this->size - 1; break; }
  //   }
  //   for (int m = 0; m <= k + 1; m ++) { // left
  //     if (!this->board[i][j] && !vstd[i][j]) this->emptyCells[this->emptyCellsSize ++] = {(int) this->allowedNumbers[i][j].size(), {i, j}}, vstd[i][j] = true;
  //     j --;
  //     if (j < 0) { j = 0; break; }
  //   }
  // }

  // for (int i = 0; i < this->size; i ++) {
  //   for (int j = i; j < this->size; j ++) {
  //     if (this->board[i][j]) continue;
  //     this->emptyCells[this->emptyCellsSize ++] = {(int) this->allowedNumbers[i][j].size(), {i, j}};
  //   }
  //   for (int j = i + 1; j < this->size; j ++) {
  //     if (this->board[j][i]) continue;
  //     this->emptyCells[this->emptyCellsSize ++] = {(int) this->allowedNumbers[j][i].size(), {j, i}};
  //   }
  // }

  // for (int ii = 0; ii < this->sqrtSize; ii ++)
  //   for (int jj = 0; jj < this->sqrtSize; jj ++)
  //     for (int iii = 0; iii < this->sqrtSize; iii ++)
  //       for (int jjj = 0; jjj < this->sqrtSize; jjj ++) {
  //         int i = ii*this->sqrtSize + iii, j = jj*this->sqrtSize + jjj;
  //         if (this->board[i][j]) continue;
  //         this->emptyCells[this->emptyCellsSize ++] = {(int) this->allowedNumbers[i][j].size(), {i, j}};
  //       }

  // random_shuffle(this->emptyCells, this->emptyCells + this->emptyCellsSize);
  // sort(this->emptyCells, this->emptyCells + this->emptyCellsSize);
  // reverse(this->emptyCells, this->emptyCells + this->emptyCellsSize);
}

void Sudoku::fillRestrictions() {
  for (int i = 0; i < this->size; i ++) {
    for (int n = 1; n <= this->size; n ++) this->lines[i][n] = false;
    for (int j = 0; j < this->size; j ++)
      if (this->board[i][j])
        this->lines[i][this->board[i][j]] = true;
  }
  for (int j = 0; j < this->size; j ++) {
    for (int n = 1; n <= this->size; n ++) this->columns[j][n] = false;
    for (int i = 0; i < this->size; i ++)
      if (this->board[i][j])
        this->columns[j][this->board[i][j]] = true;
  }
  for (int k = 0; k < this->sqrtSize; k ++)
    for (int m = 0; m < this->sqrtSize; m ++) {
      for (int n = 1; n <= this->size; n ++) this->grids[k][m][n] = false;
      for (int i = 0; i < this->sqrtSize; i ++)
        for (int j = 0; j < this->sqrtSize; j ++)
          if (this->board[k*this->sqrtSize + i][m*this->sqrtSize + j])
            this->grids[k][m][this->board[k*this->sqrtSize + i][m*this->sqrtSize + j]] = true;
    }
}

bool Sudoku::fillAllowedNumbers(int i, int j, bool avoidDone) {
  for (; i < this->size; i ++, j = 0)
    for (; j < this->size; j ++) {
      if (avoidDone && this->board[i][j]) continue;
      this->allowedNumbers[i][j].clear();
      int ii = i / this->sqrtSize, jj = j / this->sqrtSize;
      for (int k = 1; k <= this->size; k ++) {
        if (this->lines[i][k] || this->columns[j][k] || this->grids[ii][jj][k]) continue;
        this->allowedNumbers[i][j].push_back(k);
      }
      if (avoidDone && this->allowedNumbers[i][j].size() == 0) return(false);
      // random_shuffle(this->allowedNumbers[i][j].begin(), this->allowedNumbers[i][j].end());
    }
  return(true);
}

void Sudoku::fixBoard(int i, int j) {
  // this->fillAllowedNumbers(0, 0, false);
  // int fixed = 0;
  // int ni = i, nj = j;
  // i = 0, j = 0;
  // for (; i < this->size; i ++, j = 0)
  //   for (; j < this->size; j ++) {
  //     if (this->board[i][j] && this->allowedNumbers[i][j].size() > 1) {
  //       int ii = i / this->sqrtSize, jj = j / this->sqrtSize;
  //       // for (int k: this->allowedNumbers[i][j])
  //       for (int kk = rand() % this->allowedNumbers[i][j].size(), a = 0; a < this->allowedNumbers[i][j].size(); a ++, kk = (kk + 1) % this->allowedNumbers[i][j].size()) {
  //         int k = this->allowedNumbers[i][j][kk];
  //         if (k != this->board[i][j] && !this->lines[i][k] && !this->columns[j][k] && !this->grids[ii][jj][k]) {
  //           this->lines[i][k] = this->columns[j][k] = this->grids[ii][jj][k] = true;
  //           int aux = this->board[i][j];
  //           this->board[i][j] = k;
  //           k = aux;
  //           this->lines[i][k] = this->columns[j][k] = this->grids[ii][jj][k] = false;
  //           fixed ++;
  //           break;
  //         }
  //       }
  //     }
  //   }
  // if (fixed) printf("Fixing %d at %d %d\n", fixed, ni, nj);

  int k = rand() % this->sqrtSize, amount = 1;
  for (int i = 0; i < amount; i ++) {
    int li = k*this->sqrtSize + rand() % this->sqrtSize, lj = k*this->sqrtSize + rand() % this->sqrtSize;
    int hi = k*this->sqrtSize + rand() % this->sqrtSize, hj = k*this->sqrtSize + rand() % this->sqrtSize;
    int l = this->board[li][lj], h = this->board[hi][hj];
    this->lines[li][l] = this->columns[lj][l] = this->grids[k][k][l] = false;
    this->lines[hi][h] = this->columns[hj][h] = this->grids[k][k][h] = false;
    this->lines[li][h] = this->columns[lj][h] = this->grids[k][k][h] = true;
    this->lines[hi][l] = this->columns[hj][l] = this->grids[k][k][l] = true;
    this->board[li][lj] = h, this->board[hi][hj] = l;
  }
}

void Sudoku::fillDiagonals() {
  for (int i = 1; i <= this->size; i ++) this->numbersCount[i] = {-this->size, i};
  for (int i = 0; i < this->size; i ++)
    for (int j = 1; j <= this->size; j ++) {
      this->lines[i][j] = this->columns[i][j] = false,
      this->board[i][j - 1] = this->visited[i][j - 1] = this->fixes[i][j - 1] = 0;
    }
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

void Sudoku::generateBasePuzzle() {
  for (int j = 0, a = 1; j < this->sqrtSize; j ++)
    for (int i = 0; i < this->sqrtSize; i ++, a ++)
      this->board[i][j] = a;
  for (int k = 1; k < this->sqrtSize; k ++) {
    for (int i = 0; i < this->sqrtSize; i ++)
      this->board[k*this->sqrtSize + i][this->sqrtSize - 1] = this->board[(k-1)*this->sqrtSize + i][0];
    for (int j = 0; j < this->sqrtSize - 1; j ++)
      for (int i = 0; i < this->sqrtSize; i ++)
        this->board[k*this->sqrtSize + i][j] = this->board[(k-1)*this->sqrtSize + i][j + 1];
  }
  for (int k = 0; k < this->sqrtSize; k ++)
    for (int m = 1; m < this->sqrtSize; m ++) {
      for (int j = 0; j < this->sqrtSize; j ++)
        this->board[k*this->sqrtSize + this->sqrtSize - 1][m*this->sqrtSize + j] = this->board[k*this->sqrtSize][(m-1)*this->sqrtSize + j];
      for (int i = 0; i < this->sqrtSize - 1; i ++)
        for (int j = 0; j < this->sqrtSize; j ++)
          this->board[k*this->sqrtSize + i][m*this->sqrtSize + j] = this->board[k*this->sqrtSize + i + 1][(m-1)*this->sqrtSize + j];
    }
}

void Sudoku::scrambleCells() {
  int li = rand() % this->size, lj = rand() % this->size, hi = rand() % this->size, hj = rand() % this->size;
  
}

void Sudoku::removeCells(int amount) {
  while (amount --) {
    int i = rand() % this->size, j = rand() % this->size;
    if (this->board[i][j] == 0) amount ++;
    this->board[i][j] = 0;
  }
}

void Sudoku::generatePuzzleCPLEX() {
  this->fillDiagonals();
  this->generatePuzzleCPLEXUtil();
  printf("%s\n", this->isValid() ? "Valid" : "Invalid");
}

int iterations, r = 0;
void Sudoku::generatePuzzle() {
  int start = 0;
  do {
    if (r ++ == 0) this->fillDiagonals();
    else this->fixBoard();
    this->fillAllowedNumbers();
    this->fillEmptyCells();
    iterations = 0;
  } while (!this->generatePuzzleDFSEmptyCells(start));
  printf("%s\n", this->isValid() ? "Valid" : "Invalid");
}

bool Sudoku::generatePuzzleDFSEmptyCells(int e) {
  if (e == this->emptyCellsSize) return(true);
  if (iterations ++ > this->iterationsLimit) { return(false); }

  int i = this->emptyCells[e].second.first, j = this->emptyCells[e].second.second;
  int ii = i / this->sqrtSize, jj = j / this->sqrtSize;

  if (j == 0 && !this->fillAllowedNumbers(i, j)) return(false);
  bool isNewRecord = false;
  // if (this->visited[i][j] < 1 && e % 100 == 0) isNewRecord = true;
  if (this->visited[i][j] < 1) { /*printf("%d of %d\n", e, this->emptyCellsSize);*/ iterations = 0; }
  this->visited[i][j] = 1;

  // int rep = this->allowedNumbers[i][j].size();
  int rep = 2;
  if (!isNewRecord)
  {
    int rr = rand() % 100;
    // if (rep > 1 && rr < alpha*((double) e / this->emptyCellsSize)) rep = 2;
    if (rep > 1 && rr*this->emptyCellsSize < alpha*e) rep = 2;
    // if (rep > 1 && rr < alpha) rep = 2;
    // if (rep > 1 && rr < 70) rep = 2;
    else rep = 1;
  }

  for (int k = rand() % this->allowedNumbers[i][j].size(), a = 0, aa = 0, n = 0; a < this->allowedNumbers[i][j].size(); a ++, k = (k + 1) % this->allowedNumbers[i][j].size()) {
    n = this->allowedNumbers[i][j][k];
    if (this->lines[i][n] || this->columns[j][n] || this->grids[ii][jj][n]) continue; else aa ++;

    this->board[i][j] = n, this->lines[i][n] = this->columns[j][n] = this->grids[ii][jj][n] = true;
    if (this->generatePuzzleDFSEmptyCells(e + 1)) return(true);
    this->board[i][j] = 0, this->lines[i][n] = this->columns[j][n] = this->grids[ii][jj][n] = false;

    if (aa == rep) return(false);
  }
  return(false);
}

bool Sudoku::generatePuzzleCPLEXUtil() {
  try {
    IloEnv env;
    IloModel sudoku(env, "Sudoku");
    IloCplex cplex(sudoku);
    // Numbers will be indexed by 0

    // Decision Variables:
      IloArray<IloArray<IloBoolVarArray>> boardNumber(env, this->size);
      for (int i = 0; i < this->size; i ++) {
        boardNumber[i] = IloArray<IloBoolVarArray>(env, this->size);
        for (int j = 0; j < this->size; j ++) {
          boardNumber[i][j] = IloBoolVarArray(env, this->size);
        }
      }

    // Restrictions:
      // Already Set:
        for (int i = 0; i < this->size; i ++)
          for (int j = 0; j < this->size; j ++)
            if (this->board[i][j])
              sudoku.add(boardNumber[i][j][this->board[i][j] - 1] == 1);
      // Cells:
        for (int i = 0; i < this->size; i ++)
          for (int j = 0; j < this->size; j ++)
            sudoku.add(IloSum(boardNumber[i][j]) == 1);
      // Lines and Columns:
      for (int i = 0; i < this->size; i ++) {
        for (int n = 0; n < this->size; n ++) {
          IloExpr numberInLineCount(env), numberInColumnCount(env);
          for (int j = 0; j < this->size; j ++) {
            numberInLineCount += boardNumber[i][j][n],
            numberInColumnCount += boardNumber[j][i][n];
          }
          sudoku.add(numberInLineCount == 1);
          sudoku.add(numberInColumnCount == 1);
        }
      }
      // Grids:
      for (int k = 0; k < this->sqrtSize; k ++)
        for (int m = 0; m < this->sqrtSize; m ++) {
          for (int n = 0; n < this->size; n ++) {
            IloExpr numberInGridCount(env);
            for (int ii = 0; ii < this->sqrtSize; ii ++)
              for (int jj = 0; jj < this->sqrtSize; jj ++)
                numberInGridCount += boardNumber[k*this->sqrtSize + ii][m*this->sqrtSize + jj][n];
            sudoku.add(numberInGridCount == 1);
          }
        }
    
    // Objective Function:
      IloExpr totalSum(env);
      for (int i = 0; i < this->size; i ++)
        for (int j = 0; j < this->size; j ++)
          totalSum += IloSum(boardNumber[i][j]);
      sudoku.add(totalSum == this->size*this->size);
      sudoku.add(IloMaximize(env, totalSum));

    // Get Solution:
      // cplex.setParam(IloCplex::MIPEmphasis, IloCplex::MIPeMPHASIS);
      // cplex.setParam(IloCplex::Param::Simplex::Limits::UpperObj, this->size*this->size);
      cplex.solve();
      printf("Total Numbers: %0.lf\n", cplex.getObjValue());
      IloArray<IloArray<IloNumArray>> boardSolution(env, this->size);
      for (int i = 0; i < this->size; i ++) {
        boardSolution[i] = IloArray<IloNumArray>(env, this->size);
        for (int j = 0; j < this->size; j ++) {
          boardSolution[i][j] = IloNumArray(env, this->size);
          cplex.getValues(boardSolution[i][j], boardNumber[i][j]);
          for (int n = 0; n < this->size; n ++)
            if (boardSolution[i][j][n])
              this->board[i][j] = n + 1;
        }
      }
  } catch (const IloException& e) {
    cout << e << endl;
  }
}

bool Sudoku::generatePuzzleDFS(int i, int j) {
  if (i == this->size) return(true);
  // if (iterations > 10000000) return(false);
  if (this->board[i][j]) return(this->generatePuzzleDFS(i + (j + this->sqrtSize == this->size), (j + this->sqrtSize) % this->size));
  if (((j == 0 && i % 3 == 0) || (j == this->size / 3 && i % 3 == 1) || (j == 2 * this->size / 3 && i % 3 == 2)) && !this->fillAllowedNumbers(i, j)) return(false);
  if (this->allowedNumbers[i][j].size() == 0) return(false);
  // this->visited[i][j] ++;
  // if (this->visited[i][j] > 100000) return(false);
  // iterations ++;

  int ii = i / this->sqrtSize, jj = j / this->sqrtSize;
  int rr = rand() % 100;//((rand() % 10000) > 3000);
  // double pp = 1.0 - (double) (i*this->size + j) / ((this->size - 1)*(this->size - 1));
  // int rep = 0 + log2(this->size * pp);
  int rep = 1;
  if (rr < 70) rep = 2;
  else rep = 1;

  for (int k = rand() % this->allowedNumbers[i][j].size(), a = 0, aa = 0, n = 0; a < this->allowedNumbers[i][j].size(); a ++, k = (k + 1) % this->allowedNumbers[i][j].size()) {
    n = this->allowedNumbers[i][j][k];
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