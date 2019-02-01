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
    int edgeCosts[e], edgesData[e][2];
    for (int i = 0, u, v, c; i < e; i ++)
    {
      scanf("%d %d %d", &u, &v, &c); u --, v --;
      graph[u].push_back({v, c, i}), graph[v].push_back({u, c, i}), simplifiedGraph[u].push_back({v, c, i}), edgeCosts[i] = c, edgesData[i][0] = u, edgesData[i][1] = v;
    }
    int t; scanf("%d", &t);
    int terminalVertices[t]; bool isTerminal[v]; memset(isTerminal, false, sizeof(isTerminal));
    for (int i = 0; i < t; i ++) { scanf("%d", &terminalVertices[i]); terminalVertices[i] --; isTerminal[terminalVertices[i]] = true; }

  // Decision Variables:
    IloBoolVarArray edges(env, e), vertices(env, v);
    IloArray<IloNumVarArray> flows(env, e);
    for (int i = 0; i < e; i ++) flows[i] = IloNumVarArray(env, 2, 0, IloInfinity);

  // Restrictions:
    // Terminal vertices:
      // Exist:
        for (int i = 0; i < t; i ++) steinerTreeProblem.add(vertices[terminalVertices[i]] == true);
      // Terminals must have at least one edge:
        for (int i = 0; i < t; i ++)
        {
          IloExpr connectionsCount(env);
          for (auto edge: graph[terminalVertices[i]]) connectionsCount += edges[edge.edgeNumber];
          steinerTreeProblem.add(connectionsCount >= 1);
        }
    // Non-terminals must have 2+ edges (if they belongs to the tree):
      for (int i = 0; i < v; i ++)
        if (!isTerminal[i])
        {
          IloExpr connectionsCount(env);
          for (auto edge: graph[i]) connectionsCount += edges[edge.edgeNumber];
          steinerTreeProblem.add(connectionsCount >= 2 * vertices[i]);
        }
    // Edges and Vertices must match:
      for (int i = 0; i < e; i ++) steinerTreeProblem.add(2 * edges[i] <= vertices[edgesData[i][0]] + vertices[edgesData[i][1]]);
    // Tree definition: V = E + 1
      IloExpr verticesCount(env), edgesCount(env);
      for (int i = 0; i < v; i ++) verticesCount += vertices[i];
      for (int i = 0; i < e; i ++) edgesCount += edges[i];
      steinerTreeProblem.add(verticesCount == edgesCount + 1);
    // Cycle:
      // Edges sends flow of 2:
        for (int i = 0; i < e; i ++) steinerTreeProblem.add(flows[i][0] + flows[i][1] == 2 * edges[i]);
      // Vertices receives flow < 2
        IloExprArray flowSums(env, v); for (int i = 0; i < v; i ++) flowSums[i] = IloExpr(env);
        for (int i = 0; i < e; i ++) flowSums[edgesData[i][0]] += flows[i][0], flowSums[edgesData[i][1]] += flows[i][1];
        for (int i = 0; i < v; i ++) steinerTreeProblem.add(flowSums[i] <= 2 - 2.0 / v);

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
      printf("%d(%c):", i + 1, isTerminal[i] ? 'T' : 'v');
      for (auto edge: simplifiedGraph[i]) if (edgesSolution[edge.edgeNumber]) printf(" (%d, %d)", edge.to + 1, edge.cost);
      printf("\n");
    }
  
  env.end();
  return(0);
}