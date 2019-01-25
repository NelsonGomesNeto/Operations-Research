#include <stdio.h>
#include <vector>
#include <ilcplex/ilocplex.h>
using namespace std;

int main()
{
  IloEnv env;
  IloModel minimumCostEdgeColoringProblem(env, "Minimum Cost Edge Coloring Problem");
  IloCplex cplex(minimumCostEdgeColoringProblem);

  // Statement Data:
    int v, e; scanf("%d %d", &v, &e);
    vector<pair<int, int>> graph[v], simplefiedGraph[v]; // {vertex, edge number}
    for (int i = 0, u, v; i < e; i ++)
    {
      scanf("%d %d", &u, &v); u --, v --;
      graph[u].push_back({v, i}), graph[v].push_back({u, i}), simplefiedGraph[u].push_back({v, i});
    }

  // Decision Variables:
    IloIntVarArray edgeColor(env, e, 1, e);

  // Restrictions:
    // Adjacent edges must have different colors:
      IloExpr sameColorCount(env);
      for (int i = 0; i < v; i ++)
        for (int j = 0; j < graph[i].size(); j ++)
          for (int k = j + 1; k < graph[i].size(); k ++)
            minimumCostEdgeColoringProblem.add(edgeColor[graph[i][j].second] - edgeColor[graph[i][k].second] != 0);

  // Objective Function:
    IloExpr coloringCost(env);
    for (int i = 0; i < e; i ++) coloringCost += edgeColor[i];
    minimumCostEdgeColoringProblem.add(IloMinimize(env, coloringCost));

  // Get Solution:
    cplex.solve();
    printf("MinimumColoringCost: %.0lf\n", cplex.getObjValue());
    IloNumArray edgeColorSolution(env, e); cplex.getValues(edgeColorSolution, edgeColor);
    for (int i = 0; i < v; i ++)
    {
      printf("%d:", i + 1);
      for (auto j: simplefiedGraph[i]) printf(" (%d, %d)", j.first + 1, (int) edgeColorSolution[j.second]);
      printf("\n");
    }
        
  
  env.end();
  return(0);
}