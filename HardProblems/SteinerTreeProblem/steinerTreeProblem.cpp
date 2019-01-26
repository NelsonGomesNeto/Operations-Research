#include <stdio.h>
#include <vector>
#include <ilcplex/ilocplex.h>
using namespace std;

struct Edge { int to, cost, edgeNumber; };

int main()
{
  IloEnv env;
  IloModel steinerTreeProblem(env, "Steiner Tree Problem");
  IloCplex cplex(steinerTreeProblem);

  // Statement Data:
    int v, e; scanf("%d %d", &v, &e);
    vector<Edge> graph[v], simplifiedGraph[v]; // {vertex, cost}
    int edgeCosts[e];
    for (int i = 0, u, v, c; i < e; i ++)
    {
      scanf("%d %d %d", &u, &v, &c); u --, v --;
      graph[u].push_back({v, c, i}), graph[v].push_back({u, c, i}), simplifiedGraph[u].push_back({v, c, i}), edgeCosts[i] = c;
    }
    int t; scanf("%d", &t);
    int obligatedVertices[t]; for (int i = 0; i < t; i ++) { scanf("%d", &obligatedVertices[i]); obligatedVertices[i] --; }

  // Decision Variables:
    IloBoolVarArray edges(env, e);

  // Restrictions:
    // Obligated vertices
      for (int i = 0; i < t; i ++)
      {
        IloExpr connectionsCount(env);
        for (auto e: graph[obligatedVertices[i]]) connectionsCount += edges[e.edgeNumber];
        steinerTreeProblem.add(connectionsCount >= 1);
      }

  // Objective Function:
    IloExpr treeCost(env);
    for (int i = 0; i < e; i ++) treeCost += edgeCosts[i] * edges[i];
    steinerTreeProblem.add(IloMinimize(env, treeCost));

  // Get Solution:
    cplex.solve();
    printf("TreeCost: %.0lf\n", cplex.getObjValue());
    IloNumArray edgesSolution(env, e); cplex.getValues(edgesSolution, edges);
    for (int i = 0; i < v; i ++)
    {
      printf("%d:", i + 1);
      for (auto e: simplifiedGraph[i]) if (edgesSolution[e.edgeNumber]) printf(" (%d, %d)", e.to + 1, e.cost);
      printf("\n");
    }
  
  env.end();
  return(0);
}