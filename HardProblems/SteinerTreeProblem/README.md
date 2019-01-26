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
* Xi | i ∈ E (*edges*):  *Xi is the color of the i edge.*

##### Constraints
* i = (a, b) and j = (a, c). ∀ i, j ∈ E, Xi ≠ Xj

##### Objective
* min ( ∑ cost[Xi], from i = 0 to ||E|| )
