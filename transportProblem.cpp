#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>
using namespace std;

void read(int a[], int n) { for (int i = 0; i < n; i ++) scanf("%d", &a[i]); }

int main()
{
	IloEnv env;
	int f, d; scanf("%d %d", &f, &d);
	int factories[f], deposits[d]; read(factories, f); read(deposits, d);
	int costs[f][d]; for (int i = 0; i < f; i ++) for (int j = 0; j < d; j ++) scanf("%d", &costs[i][j]);

	IloModel transportProblem(env, "Transport Problem");
	IloCplex cplex(transportProblem);

	IloArray<IloIntVarArray> x(env, f);
	for (int i = 0; i < f; i ++)
		x[i] = IloIntVarArray(env, d, 0, IloInfinity);

	// in
	for (int i = 0; i < f; i ++)
	{
		IloExpr in(env);
		for (int j = 0; j < d; j ++)
			in += x[i][j];
		transportProblem.add(in <= factories[i]);
	}

	// out
	for (int j = 0; j < d; j ++)
	{
		IloExpr out(env);
		for (int i = 0; i < f; i ++)
			out += x[i][j];
		transportProblem.add(out <= deposits[j]);
	}

	IloExpr objective(end);
	for (int i = 0; i < f; i ++)
		for (int j = 0; j < d; j ++)
			objective += x[i][j] * costs[i][j];
	transportProblem.add(IloMinimize(env, objective));

	cplex.solve();
	printf("minCost: %d\n", cplex.getObjValue());

	IloArray<IloIntArray> solutions(env, f);
	for (int i = 0; i < f; i ++)
	{
		solutions[i] = IloIntArray(env, d);
		cplex.getValues(solutions[i], x[i]);
	}
	
	for (int i = 0; i < f; i ++)
	{
		printf("From factory %d:\n", i);
		for (int j = 0; j < d; j ++)
			printf("\t%d to deposit %d\n", solutions[i][j], j);
	}

	env.end();

	return(0);
}