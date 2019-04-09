import matplotlib.pyplot as plotter
import numpy

backtracking = numpy.load("./../Backtracking/data.npy")
prune = numpy.load("./../Prune/data.npy")
weightedPrune = numpy.load("./../Weighted Prune/data.npy")
data = [backtracking, prune, weightedPrune]
print(backtracking)
print(prune)
print(weightedPrune)
plotter.figure(figsize=(11, 4))
for d in data:
    plotter.plot(d[0])
plotter.xticks([0, 1, 2, 3], ["3² x 3²", "4² x 4²", "5² x 5²", "6² x 6²"])
plotter.title("Comparação")
plotter.xlabel("Tamanhos")
plotter.ylabel("Tempo de execução (s)")
plotter.legend(["Backtracking", "Poda", "Poda com peso"])
plotter.savefig("done")
