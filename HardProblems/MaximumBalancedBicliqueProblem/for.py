import os

for i in range(1, 20 + 1):
    os.system("python3 ./generator.py > testCases/%d.in" % i)
    os.system("./maximumBalancedBicliqueProblem.run < testCases/%d.in > testCases/%d.out" % (i, i))