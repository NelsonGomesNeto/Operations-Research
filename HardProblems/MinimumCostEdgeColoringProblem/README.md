# Edge Coloring of a Graph
### What's the problem?
> In graph theory, edge coloring of a graph is an assignment of “colors” to the edges of the graph so that no two adjacent edges have the same color with an optimal number of colors. Two edges are said to be adjacent if they are connected to the same vertex. There is no known polynomial time algorithm for edge-coloring every graph with an optimal number of colors. Nevertheless, a number of algorithms have been developed that relax one or more of these criteria, they only work on a subset of graphs, or they do not always use an optimal number of colors, or they do not always run in polynomial time.

![Problem](https://github.com/NelsonGomesNeto/Operations-Research/blob/master/HardProblems/MinimumCostEdgeColoringProblem/edgeColoring.png)

## Modeling with Linear Programming

##### Decision variable
* Xi | i ∈ E (*edges*):  *Xi is the color of the i edge.*

##### Constraints
* i = (a, b) and j = (a, c). ∀ i, j ∈ E, Xi ≠ Xj

##### Objective
* min ( ∑ cost[Xi], from i = 0 to ||E|| )
