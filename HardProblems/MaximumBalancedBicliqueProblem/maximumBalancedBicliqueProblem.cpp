#include <stdio.h>
#include <set>
#include <ilcplex/ilocplex.h>
using namespace std;

int main()
{
  IloEnv env;
  IloModel maximumBalancedBicliqueProblem(env, "Maximum Balanced Biclique Problem");
  IloCplex cplex(maximumBalancedBicliqueProblem);

  // Statement Data:
    int v1, v2, e, rev; scanf("%d %d %d", &v1, &v2, &e); rev = v1*v2 - e;
    set<pair<int, int>> edges, revEdges; int u, v;
    for (int i = 0; i < e; i ++)
    {
      scanf("%d %d", &u, &v); u --, v --;
      edges.insert({u, v});
    }
    for (int i = 0; i < v1; i ++)
      for (int j = 0; j < v2; j ++)
        if (!edges.count({i, j}))
          revEdges.insert({i, j});

  // Decision Variables:
    IloBoolVarArray vLeft(env, v1), vRight(env, v2);

  // Restrictions:
    IloExpr vLeftSum(env), vRightSum(env);
    for (int i = 0; i < v1; i ++) vLeftSum += vLeft[i];
    for (int i = 0; i < v2; i ++) vRightSum += vRight[i];
    // Balanced:
      maximumBalancedBicliqueProblem.add(vLeftSum == vRightSum);
    // Clique:
      for (auto edge: revEdges)
        maximumBalancedBicliqueProblem.add(vLeft[edge.first] + vRight[edge.second] <= 1);

  // Objective Function:
    maximumBalancedBicliqueProblem.add(IloMaximize(env, vLeftSum + vRightSum));

  // Get Solution:
    cplex.solve();
    printf("MaximumVertices: %.0lf\n", cplex.getObjValue());
    IloNumArray vLeftSolution(env, v1), vRightSolution(env, v2);
    cplex.getValues(vLeftSolution, vLeft); cplex.getValues(vRightSolution, vRight);
    printf("Left:"); for (int i = 0; i < v1; i ++) if (vLeftSolution[i]) printf(" %d", i + 1);
    printf("\nRight:"); for (int i = 0; i < v2; i ++) if (vRightSolution[i]) printf(" %d", i + 1);
    printf("\n");
    // printf("\nEdges:\n"); for (auto edge: edges) if (vLeftSolution[edge.first] && vRightSolution[edge.second]) printf("\t%d %d\n", edge.first + 1, edge.second + 1);
  
  env.end();
  return(0);
}
