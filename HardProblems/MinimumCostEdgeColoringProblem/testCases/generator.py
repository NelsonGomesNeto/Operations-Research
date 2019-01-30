from random import randint

maxVertices = 15
n = randint(2, maxVertices)
m = randint(1, min(17, n * (n - 1) // 2))
print(n, m)
edges = set()
while (len(edges) < m):
    u, v = randint(1, n), randint(1, n)
    while (tuple((v, u)) in edges or u == v):
        u, v = randint(1, n), randint(1, n)
    edges.add(tuple((u, v)))
for edge in edges:
    print(*edge)