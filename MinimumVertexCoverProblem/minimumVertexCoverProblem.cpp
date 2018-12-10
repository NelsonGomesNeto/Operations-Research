#include <stdio.h>
#include <vector>
#include <ilcplex/ilocplex.h>
using namespace std;

int main()
{
  IloEnv env;
  IloModel minimumVertexCoverProblem(env, "Minimum Vertex Cover Problem");
  IloCplex cplex(minimumVertexCoverProblem);

  // Statement Data:
    int n, m; scanf("%d %d", &n, &m);
    int installationCost[n]; for (int i = 0; i < n; i ++) scanf("%d", &installationCost[i]);
    vector<int> graph[n];
    for (int i = 0; i < m; i ++)
    {
      int u, v; scanf("%d %d", &u, &v);
      graph[u].push_back(v); graph[v].push_back(u);
    }

  // Decision Variables:
    IloBoolVarArray x(env, n);

  // Restrictions:
    // Service:
      for (int i = 0; i < n; i ++)
      {
        IloExpr service(env);
        service += x[i];
        for (auto j: graph[i]) service += x[j];
        minimumVertexCoverProblem.add(service >= 1);
      }
  
  // Objective Function:
    IloExpr totalCost(env);
    for (int i = 0; i < n; i ++) totalCost += x[i] * installationCost[i];
    minimumVertexCoverProblem.add(IloMinimize(env, totalCost));

  // Get Solution:
    cplex.solve();
    printf("minCost: %.0lf\n", cplex.getObjValue());
    IloNumArray solution(env, n); cplex.getValues(solution, x);
    for (int i = 0; i < n; i ++)
      if (solution[i])
        printf("School in neighborhood %d\n", i);

  env.end();
  return(0);
}