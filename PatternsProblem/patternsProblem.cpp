#include <stdio.h>
#include <vector>
#include <ilcplex/ilocplex.h>
using namespace std;

struct StockPrice { int body, lid; };
struct Pattern { int bodies, lids, printTime; };
struct SheetsSize { int available; vector<Pattern> patterns; };

int main()
{
  IloEnv env;
  IloModel patternsProblem(env, "Patterns Problem");
  IloCplex cplex(patternsProblem);

  // Statement Data:
    int sheetsSizes, timeout, canPrice; StockPrice stockPrice;
    scanf("%d %d %d %d %d", &sheetsSizes, &timeout, &canPrice, &stockPrice.body, &stockPrice.lid);
    SheetsSize sizes[sheetsSizes]; 
    for (int i = 0; i < sheetsSizes; i ++)
    {
      int patterns; scanf("%d %d", &sizes[i].available, &patterns);
      for (int j = 0, b, l, t; j < patterns; j ++)
      {
        scanf("%d %d %d", &b, &l, &t);
        sizes[i].patterns.push_back({b, l, t});
      }
    }

  // Decision Variables:
    IloArray<IloIntVarArray> x(env, sheetsSizes); IloIntVar y(env, 0, IloInfinity);
    for (int i = 0; i < sheetsSizes; i ++)
      x[i] = IloIntVarArray(env, sizes[i].patterns.size(), 0, IloInfinity);
  
  // Restrictions:
    // time:
      IloExpr totalTime(env);
      for (int i = 0; i < sheetsSizes; i ++)
        for (int j = 0; j < sizes[i].patterns.size(); j ++)
          totalTime += x[i][j] * sizes[i].patterns[j].printTime;
      patternsProblem.add(totalTime <= timeout);
    // max used sheets:
      for (int i = 0; i < sheetsSizes; i ++)
      {
        IloExpr usedSheets(env);
        for (int j = 0; j < sizes[i].patterns.size(); j ++) usedSheets += x[i][j];
        patternsProblem.add(usedSheets <= sizes[i].available);
      }
    // producted cans:
      IloExpr totalBodies(env), totalLids(env);
      for (int i = 0; i < sheetsSizes; i ++)
        for (int j = 0; j < sizes[i].patterns.size(); j ++)
        {
          totalBodies += x[i][j] * sizes[i].patterns[j].bodies;
          totalLids += x[i][j] * sizes[i].patterns[j].lids;
        }
      patternsProblem.add(y <= totalBodies); patternsProblem.add(y <= totalLids / 2);

  // Objective Function:
    IloExpr profit(env);
    profit += (canPrice * y) + (stockPrice.body * y) + (stockPrice.lid * 2 * y);
    for (int i = 0; i < sheetsSizes; i ++)
      for (int j = 0; j < sizes[i].patterns.size(); j ++)
      {
        profit -= stockPrice.body * x[i][j] * sizes[i].patterns[j].bodies;
        profit -= stockPrice.lid * x[i][j] * sizes[i].patterns[j].lids;
      }
    patternsProblem.add(IloMaximize(env, profit));

  // Get Solution:
    cplex.solve();
    printf("maxProfit: %.3lf\n", cplex.getObjValue());
    IloInt solutionCans = cplex.getValue(y);
    IloArray<IloNumArray> solutions(env, sheetsSizes);
    for (int i = 0; i < sheetsSizes; i ++)
    {
      solutions[i] = IloNumArray(env, sizes[i].patterns.size());
      cplex.getValues(solutions[i], x[i]);
    }
    printf("\tcans: %.0lf\n", solutionCans);
    for (int i = 0; i < sheetsSizes; i ++)
    {
      printf("\tsheet size %d:\n", i + 1);
      for (int j = 0; j < sizes[i].patterns.size(); j ++)
        printf("\t\tpattern %d: %.0lf\n", j + 1, solutions[i][j]);
    }

  env.end();
  return(0);
}