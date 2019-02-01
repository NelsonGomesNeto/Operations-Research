# Edge Coloring of a Graph
### What's the problem?
> Each color has a cost and no adjacent edges can have the same color. Choose the color for each edge such that the cost is minimum.

![Problem](https://github.com/NelsonGomesNeto/Operations-Research/blob/master/HardProblems/MinimumCostEdgeColoringProblem/edgeColoring.png)

## Modeling with Linear Programming

##### Decision variables
* Xic | i ∈ E (*edges*), c ∈ C (*colors*) -> *The i-th edge has the j-th color*

##### Restrictions
* Each edge must have only a single color:
  * ∀ i ∈ E: ∑ Xi == 1
* Adjacent edges must have different colors
  * ∀ i, j ∈ E and ∀ c ∈ C | i = (a, b) and j = (a, c): Xic + Xjc <= 1

##### Objective Function
* Minimize the total cost of coloring the edges
  * min(∑ cost[Xi])
