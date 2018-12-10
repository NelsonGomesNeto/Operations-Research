#include <stdio.h>
#include <set>
#include <ilcplex/ilocplex.h>
using namespace std;

int main()
{
  IloEnv env;
  IloModel maximumCliqueProblem(env, "Maximum Clique Problem");
  IloCplex cplex(maximumCliqueProblem);

  // Statement Data:
    int n, m; scanf("%d %d", &n, &m);
    set<int> graph[n], revGraph[n];
    for (int i = 0; i < m; i ++)
    {
      int u, v; scanf("%d %d", &u, &v);
      graph[u].insert(v); graph[v].insert(u);
    }
    for (int i = 0; i < n; i ++)
      for (int j = i + 1; j < n; j ++)
        if (!graph[i].count(j))
        {
          revGraph[i].insert(j);
          revGraph[j].insert(i);
        }

  // Decision Variables:
    IloBoolVarArray x(env, n);

  // Restrictions:
    // Clique:
      for (int i = 0; i < n; i ++)
        for (int j: revGraph[i])
          maximumCliqueProblem.add(x[i] + x[j] <= 1);

  // Objective Function:
    IloExpr totalVertices(env);
    for (int i = 0; i < n; i ++) totalVertices += x[i];
    maximumCliqueProblem.add(IloMaximize(env, totalVertices));

  // Get Solution:
    cplex.solve();
    IloNumArray solution(env); cplex.getValues(solution, x);
    int cliqueSize = 0; for (int i = 0; i < n; i ++) cliqueSize += solution[i];
    printf("maxClique: %d\n", cliqueSize);
    for (int i = 0; i < n; i ++)
      if (solution[i])
        printf("Vertex %d\n", i + 1);

  env.end();
  return(0);
}