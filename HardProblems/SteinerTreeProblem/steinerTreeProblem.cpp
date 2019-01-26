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
    int obligatedVertices[t]; bool isObligated[v]; memset(isObligated, false, sizeof(isObligated));
    for (int i = 0; i < t; i ++) { scanf("%d", &obligatedVertices[i]); obligatedVertices[i] --; isObligated[obligatedVertices[i]] = true; }

  // Decision Variables:
    IloBoolVarArray edges(env, e), vertices(env, v), originAndDestination(env, v);

  // Restrictions:
    // Obligated vertices:
      for (int i = 0; i < t; i ++)
      {
        IloExpr connectionsCount(env);
        for (auto edge: graph[obligatedVertices[i]]) connectionsCount += edges[edge.edgeNumber];
        steinerTreeProblem.add(connectionsCount >= 1);
      }
    // Origin and Destination must be only 2 vertices:
      IloExpr verticesCount(env);
      for (int i = 0; i < v; i ++) verticesCount += originAndDestination[i];
      steinerTreeProblem.add(verticesCount == 2);
    // Origin and Destination can't be a chosen vertex:
      for (int i = 0; i < v; i ++)
        steinerTreeProblem.add(vertices[i] + originAndDestination[i] <= 1);
    // Vertices must have 2 edges, except for Origin and Destination which must have 1 edge
      for (int i = 0; i < v; i ++)
      {
        IloExpr connectionsCount(env);
        for (auto edge: graph[i]) connectionsCount += edges[edge.edgeNumber];
        steinerTreeProblem.add(connectionsCount == 2 * vertices[i] + 1 * originAndDestination[i]);
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
      printf("%d(%c):", i + 1, isObligated[i] ? 'T' : 'v');
      for (auto edge: simplifiedGraph[i]) if (edgesSolution[edge.edgeNumber]) printf(" (%d, %d)", edge.to + 1, edge.cost);
      printf("\n");
    }
  
  env.end();
  return(0);
}