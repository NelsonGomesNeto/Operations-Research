#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

struct Farm { double area, water; };
struct Cultivation { double maxArea, waterPerAcre, profitPerAcre; };

int main()
{
  IloEnv env;
  int f, c; scanf("%d %d", &f, &c);
  Farm farms[f]; for (int i = 0; i < f; i ++) scanf("%lf %lf", &farms[i].area, &farms[i].water);
  Cultivation cultivations[c]; for (int i = 0; i < c; i ++) scanf("%lf %lf %lf", &cultivations[i].maxArea, &cultivations[i].waterPerAcre, &cultivations[i].profitPerAcre);

  IloModel plantingProblem(env, "Planting Problem");
  IloCplex cplex(plantingProblem);

  // Decision Variables
    IloArray<IloNumVarArray> x(env, f);
    for (int i = 0; i < f; i ++) x[i] = IloNumVarArray(env, c, 0, IloInfinity);

  // Restrictions
    // maxArea (cultivation):
      for (int j = 0; j < c; j ++)
      {
        IloExpr area(env);
        for (int i = 0; i < f; i ++) area += x[i][j];
        plantingProblem.add(area <= cultivations[j].maxArea);
      }
    // Water:
      for (int i = 0; i < f; i ++)
      {
        IloExpr water(env);
        for (int j = 0; j < c; j ++) water += x[i][j] * cultivations[j].waterPerAcre;
        plantingProblem.add(water <= farms[i].water);
      }
    // maxArea (farm):
      for (int i = 0; i < f; i ++)
      {
        IloExpr area(env);
        for (int j = 0; j < c; j ++) area += x[i][j];
        plantingProblem.add(area <= farms[i].area);
      }
    // Proportion:
      IloExpr baseProportion(env);
      for (int j = 0; j < c; j ++) baseProportion += x[0][j];
      baseProportion /= farms[0].area;
      for (int i = 1; i < f; i ++)
      {
        IloExpr proportion(env);
        for (int j = 0; j < c; j ++) proportion += x[i][j];
        proportion /= farms[i].area;
        plantingProblem.add(baseProportion == proportion);
      }
  
  // Objective Function
    IloExpr profit(env);
    for (int j = 0; j < c; j ++)
      for (int i = 0; i < f; i ++)
        profit += x[i][j] * cultivations[j].profitPerAcre;
    plantingProblem.add(IloMaximize(env, profit));

  cplex.solve();
  printf("maxProfit: %.3lf\n", cplex.getObjValue());
  IloArray<IloNumArray> solutions(env, f);
  for (int i = 0; i < c; i ++)
  {
    solutions[i] = IloNumArray(env, c);
    cplex.getValues(solutions[i], x[i]);
  }
  for (int i = 0; i < f; i ++)
  {
    printf("Farm %d\n", i + 1);
    for (int j = 0; j < c; j ++) printf("\tCultivation %d: %.3lf\n", j + 1, solutions[i][j]);
  }

  env.end();
  return(0);
}