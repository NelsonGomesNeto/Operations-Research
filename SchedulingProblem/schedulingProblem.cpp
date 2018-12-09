#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main()
{
  IloEnv env;
  IloModel schedulingProblem(env, "Scheduling Problem");
  IloCplex cplex(schedulingProblem);

  // Statement Data:
    int consecutiveWorkDays; scanf("%d", &consecutiveWorkDays);
    int demands[7]; for (int i = 0; i < 7; i ++) scanf("%d", &demands[i]);

  // Decision Variables:
    IloIntVarArray x(env, 7, 0, IloInfinity);

  // Restrictions:
    for (int i = 0; i < 7; i ++)
    {
      IloExpr nurses(env);
      for (int j = 0; j < 5; j ++) nurses += x[(i + j) % 7];
      schedulingProblem.add(nurses >= demands[(i + 5 - 1) % 7]);
    }
  
  // Objective Function:
    IloExpr totalNurses(env);
    for (int i = 0; i < 7; i ++) totalNurses += x[i];
    schedulingProblem.add(IloMinimize(env, totalNurses));

  // Get Solution:
    cplex.solve();
    IloNumArray solutions(env, 7); cplex.getValues(solutions, x);
    int total = 0; for (int i = 0; i < 7; i ++) total += solutions[i];
    printf("Total nurses: %d\n", total);
    for (int i = 0; i < 7; i ++) printf("\tDay %d: %.0lf\n", i + 1, solutions[i]);

  env.end();
  return(0);
}