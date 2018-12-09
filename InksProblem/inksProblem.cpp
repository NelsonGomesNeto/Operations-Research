#include <stdio.h>
#include <vector>
#include <ilcplex/ilocplex.h>
using namespace std;

struct Solution { vector<double> compPercent; double price; };
struct Ink { vector<double> minCompPercent; double liters; };

int main()
{
  IloEnv env;
  IloModel inksProblem(env, "Inks Problem");
  IloCplex cplex(inksProblem);
  // Reading Statement:
    int inksSize, componentsSize, solutionsSize; scanf("%d %d %d", &inksSize, &componentsSize, &solutionsSize);
    double components[componentsSize]; for (int i = 0; i < componentsSize; i ++) scanf("%lf", &components[i]);
    Solution solutions[solutionsSize];
    for (int i = 0; i < solutionsSize; i ++)
    {
      for (int j = 0; j < componentsSize; j ++)
      {
        double c; scanf("%lf", &c);
        solutions[i].compPercent.push_back(c);
      }
      scanf("%lf", &solutions[i].price);
    }
    Ink inks[inksSize];
    for (int i = 0; i < inksSize; i ++)
    {
      for (int j = 0; j < componentsSize; j ++)
      {
        double c; scanf("%lf", &c);
        inks[i].minCompPercent.push_back(c);
      }
      scanf("%lf", &inks[i].liters);
    }
  printf("%d %d %d\n", inksSize, componentsSize, solutionsSize);
  for (int i = 0; i < componentsSize; i ++) printf("%.3lf%c", components[i], i < componentsSize - 1 ? ' ' : '\n');
  for (int i = 0; i < solutionsSize; i ++)
  {
    for (int j = 0; j < componentsSize; j ++) printf("%.3lf ", solutions[i].compPercent[j]);
    printf("%.3lf\n", solutions[i].price);
  }
  for (int i = 0; i < inksSize; i ++)
  {
    for (int j = 0; j < componentsSize; j ++) printf("%.3lf ", inks[i].minCompPercent[j]);
    printf("%.3lf\n", inks[i].liters);
  }
  
  // Decision Variables:
    IloArray<IloIntVarArray> xSolutions(env, inksSize), xComponents(env, inksSize);
    for (int i = 0; i < inksSize; i ++)
    {
      xSolutions[i] = IloIntVarArray(env, solutionsSize, 0, IloInfinity);
      xComponents[i] = IloIntVarArray(env, componentsSize, 0, IloInfinity);
    }
  
  // Restrictions:
    // Production:
      for (int j = 0; j < inksSize; j ++)
      {
        IloExpr production(env);
        for (int i = 0; i < solutionsSize; i ++) production += xSolutions[j][i];
        for (int i = 0; i < componentsSize; i ++) production += xComponents[j][i];
        inksProblem.add(production == inks[j].liters);
      }
    // Proportion:
      for (int j = 0; j < inksSize; j ++)
      {
        for (int k = 0; k < componentsSize; k ++)
        {
          IloExpr proportion(env);
          for (int i = 0; i < solutionsSize; i ++) proportion += xSolutions[j][i] * solutions[i].compPercent[k];
          proportion += xComponents[j][k];
          inksProblem.add(proportion >= (inks[j].liters * inks[j].minCompPercent[k]));
        }
      }
  
  // Objective Function:
    IloExpr cost(env);
    for (int j = 0; j < inksSize; j ++)
    {
      for (int i = 0; i < solutionsSize; i ++) cost += xSolutions[j][i] * solutions[i].price;
      for (int i = 0; i < componentsSize; i ++) cost += xComponents[j][i] * components[i];
    }
    inksProblem.add(IloMinimize(env, cost));

  // Get solution:
  cplex.solve();
  printf("minCost: %.3lf\n", cplex.getObjValue());
  IloArray<IloNumArray> solutionsAnswer(env, inksSize), componentsAnswer(env, inksSize);
  for (int i = 0; i < inksSize; i ++)
  {
    solutionsAnswer[i] = IloNumArray(env, solutionsSize), componentsAnswer[i] = IloNumArray(env, componentsSize);
    cplex.getValues(solutionsAnswer[i], xSolutions[i]);
    cplex.getValues(componentsAnswer[i], xComponents[i]);
  }
  for (int j = 0; j < inksSize; j ++)
  {
    printf("Ink %d:\n", j + 1);
    for (int i = 0; i < solutionsSize; i ++) printf("\tSolution %d: %.0lf\n", i + 1, solutionsAnswer[j][i]);
    for (int i = 0; i < componentsSize; i ++) printf("\tComponent %d: %.0lf\n", i + 1, componentsAnswer[j][i]);
  }

  env.end();
  return(0);
}