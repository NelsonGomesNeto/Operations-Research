# Steiner Tree Problem
### What is Steiner Tree?
> Given a graph and a subset of vertices in the graph, a steiner tree spans though the given subset. The Steiner Tree may contain some vertices which are not in given subset but are used to connect the vertices of subset.
> The given set of vertices is called Terminal Vertices and other vertices that are used to construct Steiner Tree are called Steiner vertices.
> The Steiner Tree Problem is to find the minimum cost Steiner Tree. See below for an example.

##### Problem
![Problem](https://github.com/NelsonGomesNeto/Operations-Research/blob/master/HardProblems/SteinerTreeProblem/problem.png)
##### Solution
![Solution](https://github.com/NelsonGomesNeto/Operations-Research/blob/master/HardProblems/SteinerTreeProblem/solution.png)

##### Applications of Steiner Tree
Any situation where the task is minimize cost of connection among some important locations like VLSI Design, Computer Networks, etc.

## Modeling with Linear Programming

##### Decision variables
* Xi | i ∈ E (*edges*). *Xi indicates if the i-th edge belongs to the final tree*
* Ui | i ∈ V (*vertices*). *Ui indicates if the i-th vertice belongs to the final tree*
* Fij | i ∈ E, j ∈ {0, 1}. *Fi0 is the flow in the i-th edge to the source (of that edge) and Fi1 the flow to the target (of that edge)*


##### Restrictions
* Every terminal must have at least one edge in the final tree
  * ∀ v ∈ T (*terminals*), ∃ w ∈ E | w = (v, *); X[w] = True.
* Every terminal must be in the final tree
  * ∀ v ∈ T; U[v] = True.
* Each non-terminal must have zero, two or more edges in the final tree
  * ∀ v ∉ T, ∃ EST ⊂ E | w ∈ EST iff w = (v, *); U[v] <= 2 * ∑ X[w]
* Edge existence implies on vertices existences
  * ∀ w ∈ E | w = (u, v); 2 * X[w] <= U[u] + U[v]
* Tree definition
  * ∑ U = 1 + ∑ X
* Cycles mustn't existence
  * The flow throught every chosen edge must be exactly 2
    * ∀ w ∈ E | w = (u, v); U[u] + U[v] = 2 * X[w]
  * The flow sum throught every vertex must be lower or equal to then 2 - 2 / |V|
    * ∀ v ∈ V, ∃ EV ⊂ E | w ∈ EV iff w = (v, \*), ∃ EREV ⊂ E | y ∈ EREV iff w = (\*, v); ∑ F[w][0] + ∑ F[y][1] ≤ 2 - 2 / |V|

##### Objective Function
* Minimize the cost of the Steiner Tree
  * min(∑ cost[i] * X[i] | i ∈ E)