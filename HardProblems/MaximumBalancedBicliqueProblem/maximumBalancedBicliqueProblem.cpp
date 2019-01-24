#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main()
{
  IloEnv env;
  IloModel maximumBalancedBicliqueProblem(env, "Maximum Balanced Biclique Problem");
  IloCplex cplex(maximumBalancedBicliqueProblem);

  // Statement Data:
    int v1, v2, e; scanf("%d %d %d", &v1, &v2, &e);
    int edges[e][2]; for (int i = 0; i < e; i ++) { scanf("%d %d", &edges[i][0], &edges[i][1]); edges[i][0] --, edges[i][1] --; }

  // Decision Variables:
    IloBoolVarArray vLeft(env, v1), vRight(env, v2), edgesVar(env, e);

  // Restrictions:
    IloExpr vLeftSum(env), vRightSum(env);
    for (int i = 0; i < v1; i ++) vLeftSum += vLeft[i];
    for (int i = 0; i < v2; i ++) vRightSum += vRight[i];
    // Bipartite:
      maximumBalancedBicliqueProblem.add(vLeftSum == vRightSum);
    // Clique:
      IloExpr edgeWithVerticesCount(env);
      for (int i = 0; i < e; i ++)
      {
        edgeWithVerticesCount += edgesVar[i] + vLeft[edges[i][0]] + vRight[edges[i][1]];
      }
      IloExpr neededEdges(env); for (int i = 0; i < v1; i ++) neededEdges += 2 * vLeft[i];
      maximumBalancedBicliqueProblem.add(neededEdges == edgeWithVerticesCount - (2 * e));
      // derived from complete edge formula: n * (n - 1) / 2

  // Objective Function:
    IloExpr totalVertices(env); totalVertices = 2 * vLeftSum;
    maximumBalancedBicliqueProblem.add(IloMaximize(env, totalVertices));

  // Get Solution:
    cplex.solve();
    printf("MaximumVertices: %.0lf\n", cplex.getObjValue());
      // printf("Here\n");
    // IloNumArray solutions(env, ingredients); cplex.getValues(solutions, x);
    // for (int i = 0; i < ingredients; i ++)
    //   printf("ingredient %d: %.0lf\n", i + 1, solutions[i]);
  
  env.end();
  return(0);
}