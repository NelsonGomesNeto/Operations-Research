# Maximum Balanced Biclique
### What's the problem?
> Find a balanced biclique (complete bipartite subgraph with the same vertex number in each part) in G with the maximum number of vertex.

![Problem](https://github.com/NelsonGomesNeto/Operations-Research/blob/master/HardProblems/MaximumBalancedBicliqueProblem/balancedBiclique.PNG)

## Modeling with Linear Programming

##### Decision Variables
* leftV[i] = True -> i-th vertex on the left is chosen
* rightV[i] = True -> i-th vertex on the right is chosen

##### Constraints
*  ∑ leftV = ∑  rightV
* ∀ E[i] ∉  G,  leftV[E[i].left] rightV[E[i].right] <= 1

##### Objective
* max(∑leftV + ∑rightV)
