import os
import time
import matplotlib.pyplot as plot
os.system("g++ *.cpp -o test -std=c++17 -O2")

times = []
totalTime = time.time()
for i in range(1000):
    startTime = time.time()
    os.system("./test 6 > out")
    result = time.time() - startTime
    print(i, result)
    times += [result]
    if (time.time() - totalTime > 300):
        print("Failed")
        break
times.sort()
print(sum(times) / len(times))
print(min(times), max(times))
print("totalTime", time.time() - totalTime)
plot.hist(times, 100)
plot.savefig("executionTimeHistogram")