from random import random, randint

v = 25

l, r = randint(1, v), randint(1, v)
e = randint(1, l * r)
print(l, r, e)
edges = set()
while (len(edges) < e):
    edges.add(tuple((randint(1, l), randint(1, r))))
for edge in edges:
    print(*edge)