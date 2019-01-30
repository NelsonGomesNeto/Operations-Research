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
    int v, e, maxColors; scanf("%d %d", &v, &e); maxColors = e;
    vector<pair<int, int>> graph[v], simplefiedGraph[v]; // {vertex, edge number}
    for (int i = 0, u, v; i < e; i ++)
    {
      scanf("%d %d", &u, &v); u --, v --;
      graph[u].push_back({v, i}), graph[v].push_back({u, i}), simplefiedGraph[u].push_back({v, i});
    }

  // Decision Variables:
    IloArray<IloBoolVarArray> edgesColors(env, e);
    for (int i = 0; i < e; i ++) edgesColors[i] = IloBoolVarArray(env, maxColors);

  // Restrictions:
    // Each edge can only have a single color:
      for (int i = 0; i < e; i ++)
      {
        IloExpr sum(env);
        for (int c = 0; c < maxColors; c ++) sum += edgesColors[i][c];
        minimumCostEdgeColoringProblem.add(sum == 1);
      }
    // Adjacent edges must have different colors:
      for (int i = 0; i < v; i ++)
        for (int j = 0; j < graph[i].size(); j ++)
          for (int k = j + 1; k < graph[i].size(); k ++)
            for (int c = 0; c < maxColors; c ++)
              minimumCostEdgeColoringProblem.add(edgesColors[graph[i][j].second][c] + edgesColors[graph[i][k].second][c] <= 1);

  // Objective Function:
    IloExpr coloringCost(env);
    for (int i = 0; i < e; i ++)
      for (int c = 0; c < maxColors; c ++)
        coloringCost += (c + 1) * edgesColors[i][c];
    minimumCostEdgeColoringProblem.add(IloMinimize(env, coloringCost));

  // Get Solution:
    try
    {
      cplex.solve();
    }
    catch(IloException e)
    {
      std::cerr << e.getMessage() << '\n';
    }
    
    printf("MinimumColoringCost: %.0lf\n", cplex.getObjValue());
    IloArray<IloNumArray> edgesColorsSolution(env, e);
    for (int i = 0; i < e; i ++)
    {
      edgesColorsSolution[i] = IloNumArray(env, maxColors);
      cplex.getValues(edgesColorsSolution[i], edgesColors[i]);
    }
    for (int i = 0; i < v; i ++)
    {
      printf("%d:", i + 1);
      for (auto j: simplefiedGraph[i])
      {
        int chosenColor = -1;
        for (int c = 0; c < maxColors; c ++) if (edgesColorsSolution[j.second][c]) chosenColor = c + 1;
        printf(" (%d, %d)", j.first + 1, chosenColor);
      }
      printf("\n");
    }
  
  env.end();
  return(0);
}