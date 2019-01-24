#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

struct Item { int value, weight; };

int main()
{
  IloEnv env;
  IloModel knapsackProblem(env, "Knapsack Problem");
  IloCplex cplex(knapsackProblem);

  // Statement Data:
    int n, weight; scanf("%d %d", &n, &weight);
    Item items[n];
    for (int i = 0; i < n; i ++) scanf("%d", &items[i].value);
    for (int i = 0; i < n; i ++) scanf("%d", &items[i].weight);

  // Decision Variables:
    IloBoolVarArray choseItems(env, n);

  // Restricions:
    // Weight:
      IloExpr totalWeight(env);
      for (int i = 0; i < n; i ++) totalWeight += items[i].weight * choseItems[i];
      knapsackProblem.add(totalWeight <= weight);

  // Objective Function:
    IloExpr totalValue(env);
    for (int i = 0; i < n; i ++) totalValue += items[i].value * choseItems[i];
    knapsackProblem.add(IloMaximize(env, totalValue));

  // Get Solution:
    cplex.solve();
    printf("maxValue: %.0lf\n", cplex.getObjValue());
    IloNumArray solution(env, n); cplex.getValues(solution, choseItems);
    for (int i = 0; i < n; i ++)
      if (solution[i])
        printf("\tItem %d\n", i + 1);

  env.end();
  return(0);
}