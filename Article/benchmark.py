import os
import time
import matplotlib.pyplot as plotter
# os.system("g++ *.cpp -o test -std=c++17 -O2")
os.system("sudo ./run.sh")
configToTest = [(70, 100000000), (80, 100000000), (90, 100000000), (100, 100000000), (110, 100000000), (120, 100000000), (130, 100000000)]
sudokuSize = 6
toGenerate = 1000

def configToString(config):
    return(str(config[0]) + ", %.0e" % config[1])

allTimes = []
for config in configToTest:
    times = []
    for i in range(toGenerate):
        startTime = time.time()
        os.system("./test %d %d %d > out" % (sudokuSize, config[0], config[1]))
        result = time.time() - startTime
        print(i, config, result)
        times += [result]
        # if (time.time() - totalTime > 300):
        #     print("Failed")
        #     break
    times.sort()
    # print(sum(times) / len(times))
    # print(min(times), max(times))
    allTimes += [times]
# plotter.hist(times, 100)
legendStrings = []
for config in configToTest:
    legendStrings += [configToString(config)]
plotter.figure(figsize=(8, 4))
plotter.boxplot(allTimes, labels=legendStrings)
plotter.title("%d²x%d² Sudoku (%d gerados)" % (sudokuSize, sudokuSize, toGenerate))
plotter.xlabel("Configurações (alpha, iterationsLimit)")
plotter.ylabel("Tempo de execução (s)")
plotter.savefig("executionTimeHistogram")