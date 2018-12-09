#include <stdio.h>
#include <ilcplex/ilocplex.h>

int main()
{
  IloEnv env;

  IloModel feedProblem(env, "Feed Problem");
  IloCplex cplex(feedProblem);

  // Decision Variables
  IloIntVarArray x(env, 2, 0, IloInfinity); // 0: AMGS, 1: RE

  // Restrictions
  IloExpr expr(env);
  expr = 1*x[0] + 4*x[1]; feedProblem.add(expr <= 1e4);
  expr = 5*x[0] + 2*x[1]; feedProblem.add(expr <= 3e4);

  // Objective Function
  expr = 11*x[0] + 12*x[1]; feedProblem.add(IloMaximize(env, expr));

  cplex.solve();
  printf("maxProfit: %.3lf\n", cplex.getObjValue());
  IloNumArray solution(env, 2); cplex.getValues(solution, x);
  printf("AMGS: %.0lf, RE: %.0lf\n", solution[0], solution[1]);

  return(0);
}