#include <stdio.h>
#include <vector>
#include <ilcplex/ilocplex.h>
using namespace std;

int main()
{
  IloEnv env;
  IloModel maximumFlowProblem(env, "Maximum Flow Problem");
  IloCplex cplex(maximumFlowProblem);

  // Statment Data:
    int n, m; scanf("%d %d", &n, &m); int source = 0, target = n - 1;
    int capacities[n][n]; memset(capacities, 0, sizeof(capacities));
    vector<int> in[n], out[n];
    for (int i = 0; i < m; i ++)
    {
      int u, v; scanf("%d %d", &u, &v);
      scanf("%d", &capacities[u][v]);
      out[u].push_back(v); in[v].push_back(u);
    }

  // Decision Variables:
    IloArray<IloIntVarArray> flows(env, n);
    for (int i = 0; i < n; i ++) flows[i] = IloIntVarArray(env, n, 0, IloInfinity);

  // Restrictions:
    // Flow <= Capacity:
      for (int i = 0; i < n; i ++)
        for (int j = 0; j < n; j ++)
          maximumFlowProblem.add(flows[i][j] <= capacities[i][j]);
    // Flow conservation:
      for (int i = 1; i < n - 1; i ++) // avoiding source and target
      {
        IloExpr inSum(env), outSum(env);
        for (auto j: in[i]) inSum += flows[j][i];
        for (auto j: out[i]) outSum += flows[i][j];
        maximumFlowProblem.add(inSum == outSum);
      }

  // Objective Function:
    IloExpr totalFlow(env);
    for (auto i: in[target]) totalFlow += flows[i][target];
    maximumFlowProblem.add(IloMaximize(env, totalFlow));

  // Get Solution:
    cplex.solve();
    printf("maxFlow: %.0lf\n", cplex.getObjValue());
    IloArray<IloNumArray> solutions(env, n);
    for (int i = 0; i < n; i ++)
    {
      solutions[i] = IloNumArray(env, n);
      cplex.getValues(solutions[i], flows[i]);
      printf("%d:\n", i);
      for (int j = 0; j < n; j ++)
        if (capacities[i][j])
          printf("\t%.0lf to %d\n", solutions[i][j], j + 1);
    }

  env.end();
  return(0);
}