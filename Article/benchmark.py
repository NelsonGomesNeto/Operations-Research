import os
import time
import matplotlib.pyplot as plotter
import numpy
# os.system("g++ *.cpp -o test -std=c++17 -O2")
os.system("sudo ./run.sh")
# configToTest = [(70, 1000000), (80, 1000000), (90, 1000000), (100, 1000000), (110, 1000000), (120, 1000000), (130, 1000000), (140, 1000000)]
# configToTest = [(50, 1000000), (60, 1000000), (70, 1000000), (80, 1000000), (90, 1000000), (100, 1000000)]
configToTest = [(3, 100, 1000000), (4, 100, 1000000), (5, 100, 1000000), (6, 100, 1000000)]
sudokuSize = 5
toGenerate = 1

def configToString(config):
    return "%d² x %d²" % (config[0], config[0])
    return "%.1f" % (config[0] / 100) + ", %.0e" % config[1]

allTimes = []
for config in configToTest:
    times = []
    for i in range(toGenerate):
        startTime = time.time()
        # os.system("./test %d %d %d %d > out" % (sudokuSize, config[0], config[1], config[2]))
        os.system("./test %d %d %d 0 > out" % (config[0], config[1], config[2]))
        result = time.time() - startTime
        print(i, config, result)
        times += [result]
        # if (time.time() - totalTime > 300):
        #     print("Failed")
        #     break
    times.sort()
    # print(sum(times) / len(times))
    # print(min(times), max(times))
    allTimes += [sum(times) / len(times)]
# plotter.hist(times, 100)

data = numpy.mat(allTimes)
numpy.save("data", data)
f = open("data.config", "w")
print(configToTest, file=f)
f.close()

legendStrings = []
for config in configToTest:
    legendStrings += [configToString(config)]
plotter.figure(figsize=(11, 4))
# plotter.boxplot(allTimes, labels=legendStrings)
plotter.plot(allTimes)
plotter.xticks(list(range(len(configToTest))), legendStrings)
# plotter.title("%d²x%d² Sudoku (%d gerados)" % (sudokuSize, sudokuSize, toGenerate))
plotter.title("MILP")
# plotter.xlabel("Configurações (alpha, iterationsLimit)")
plotter.xlabel("Tamanhos")
plotter.ylabel("Tempo de execução (s)")
plotter.savefig("executionTimeBoxPlot")