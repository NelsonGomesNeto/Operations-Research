# Steiner Tree Problem
### What is Steiner Tree?
> Given a graph and a subset of vertices in the graph, a steiner tree spans though the given subset. The Steiner Tree may contain some vertices which are not in given subset but are used to connect the vertices of subset.
> The given set of vertices is called Terminal Vertices and other vertices that are used to construct Steiner tree are called Steiner vertices.
> The Steiner Tree Problem is to find the minimum cost Steiner Tree. See below for an example.

##### Problem
![Problem](https://github.com/NelsonGomesNeto/Operations-Research/blob/master/HardProblems/SteinerTreeProblem/problem.png)
##### Solution
![Solution](https://github.com/NelsonGomesNeto/Operations-Research/blob/master/HardProblems/SteinerTreeProblem/solution.png)

##### Applications of Steiner Tree
Any situation where the task is minimize cost of connection among some important locations like VLSI Design, Computer Networks, etc.

## Modeling with Linear Programming

##### Decision variable
* Xi | i ∈ Edges. *Xi indicates if the i-th edge belongs to the final tree*
* Ui | i ∈ V. *Ui indicates if the i-th vertice belongs to the final tree*
* Fij | i ∈ Edges, j ∈ {0, 1}. *Fi0 is the flow in the i-th edge to the source (of that edge) and Fi1 the flow to the target*


##### Constraints
* ∀ v ∈ T, ∃ w ∈ Edges | w = (v, *) and X[w] = True. *Each terminal must have at least one edge in the final tree*
* ∀ v ∈ T, U[v] = True. *All terminals must be in the final tree*
* ∀ v ∉ T, ∃ e1, e2 ∈ Edges | X[e1] = X[e2] = True. *Each non-terminal must have zero, two or more edges in the final tree*
* ||U|| = ||X|| + 1. *Tree definition* 

* ∀ e: Xeu + X ev = 2be
* ∀ v: ∑xeu ≤ 2 - 2/||U||

##### Objective
* min ( ∑ cost[Xi], from i = 0 to ||Edges|| )

