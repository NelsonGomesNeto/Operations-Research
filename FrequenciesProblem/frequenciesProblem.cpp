#include <stdio.h>
#include <set>
#include <ilcplex/ilocplex.h>
using namespace std;

int main()
{
  IloEnv env;
  IloModel frequenciesProblem(env, "Frequencies Problem");
  IloCplex cplex(frequenciesProblem);

  // Statement Data:
    int antennae, edges, frequencies; scanf("%d %d %d", &antennae, &edges, &frequencies);
    bool reaches[antennae][antennae]; memset(reaches, false, sizeof(reaches));
    for (int i = 0; i < edges; i ++)
    {
      int u, v; scanf("%d %d", &u, &v);
      reaches[u][v] = reaches[v][u] = true;
    }

  // Decision Variables:
    IloBoolVarArray usedFrequencies(env, frequencies);
    IloArray<IloBoolVarArray> antennaeFrequencies(env, antennae);
    for (int i = 0; i < antennae; i ++) antennaeFrequencies[i] = IloBoolVarArray(env, frequencies);

  // Restrictions:
    // Inteference:
      for (int u = 0; u < antennae; u ++)
        for (int v = u + 1; v < antennae; v ++)
          for (int f = 0; f < frequencies; f ++)
            if (reaches[u][v])
              frequenciesProblem.add(antennaeFrequencies[u][f] + antennaeFrequencies[v][f] <= 1);
    // Attribution:
      for (int i = 0; i < antennae; i ++)
      {
        IloExpr useSum(env);
        for (int f = 0; f < frequencies; f ++) useSum += antennaeFrequencies[i][f];
        frequenciesProblem.add(useSum == 1);
      }
    // Only use usable frequencies:
      for (int f = 0; f < frequencies; f ++)
        for (int i = 0; i < antennae; i ++)
          frequenciesProblem.add(antennaeFrequencies[i][f] <= usedFrequencies[f]);

  // Objective Function:
    IloExpr totalUsed(env);
    for (int i = 0; i < frequencies; i ++) totalUsed += usedFrequencies[i];
    frequenciesProblem.add(IloMinimize(env, totalUsed));

  // Get Solution:
    cplex.solve();
    printf("minFrequencies: %.0lf\n", cplex.getObjValue());
    IloNumArray solutionFrequencies(env, frequencies); cplex.getValues(solutionFrequencies, usedFrequencies);
    IloArray<IloNumArray> solutionAntFreq(env, antennae);
    for (int i = 0; i < antennae; i ++)
    {
      solutionAntFreq[i] = IloNumArray(env, frequencies);
      cplex.getValues(solutionAntFreq[i], antennaeFrequencies[i]);
    }
    for (int f = 0; f < frequencies; f ++)
      if (solutionFrequencies[f])
      {
        printf("Frequency %d on:\n", f + 1);
        for (int i = 0; i < antennae; i ++)
          if (solutionAntFreq[i][f])
            printf("\tAntenna %d\n", i + 1);
      }

  env.end();
  return(0);
}