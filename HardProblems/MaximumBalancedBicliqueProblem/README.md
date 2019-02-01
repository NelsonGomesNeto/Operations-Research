# Maximum Balanced Biclique
### What's the problem?
> Find a balanced biclique (complete bipartite subgraph with the same number of vertices in each side) in G with the maximum number of vertices.

![Problem](https://github.com/NelsonGomesNeto/Operations-Research/blob/master/HardProblems/MaximumBalancedBicliqueProblem/balancedBiclique.PNG)

## Modeling with Linear Programming

##### Decision Variables
* leftV[i] = True -> i-th vertex on the left is chosen
* rightV[i] = True -> i-th vertex on the right is chosen

##### Restrictions
* Each side must have the same number of vertices
  * ∑ leftV = ∑ rightV
* Every missing edge mustn't have both vertices chosen
  * ∀ E[i] ∉ G, leftV[E[i].left] rightV[E[i].right] <= 1

##### Objective Function
* Maximize the number of vertices chosen
  * max(∑ leftV + ∑ rightV)
