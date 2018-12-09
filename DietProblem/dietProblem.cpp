#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main()
{
  IloEnv env;
  int vitamins, ingredients; scanf("%d %d", &vitamins, &ingredients); // amount of vitamins and ingredients
  int vitPerIng[vitamins][ingredients], minVit[vitamins], ingPrice[ingredients];
  for (int i = 0; i < vitamins; i ++)
  {
    for (int j = 0; j < ingredients; j ++) scanf("%d", &vitPerIng[i][j]);
    scanf("%d", &minVit[i]);
  }
  for (int i = 0; i < ingredients; i ++) scanf("%d", &ingPrice[i]);

  IloModel dietProblem(env, "Diet Problem");
  IloCplex cplex(dietProblem);

  // Decision Variables
  IloIntVarArray x(env, ingredients, 0, IloInfinity);

  // Restrictions
  for (int i = 0; i < vitamins; i ++)
  {
    IloExpr mv(env);
    for (int j = 0; j < ingredients; j ++) mv += vitPerIng[i][j] * x[j];
    dietProblem.add(mv >= minVit[i]);
  }

  // Objective Function
  IloExpr cost(env); for (int i = 0; i < ingredients; i ++) cost += ingPrice[i] * x[i];
  dietProblem.add(IloMinimize(env, cost));

  cplex.solve();
  printf("minCost: %.3lf\n", cplex.getObjValue());
  IloNumArray solutions(env, ingredients); cplex.getValues(solutions, x);
  for (int i = 0; i < ingredients; i ++)
    printf("ingredient %d: %.0lf\n", i + 1, solutions[i]);
  
  env.end();

  return(0);
}