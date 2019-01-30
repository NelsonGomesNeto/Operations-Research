from random import randint

maxVertices = 25
n = randint(2, maxVertices)
t = randint(2, n)
terminals = set()
while (len(terminals) < t):
    terminals.add(randint(1, n))
m = randint(t - 1, n * (n - 1) // 2)
edges = set()
prev = -1
for terminal in terminals:
    if (prev != -1):
        edges.add(tuple((prev, terminal)))
    prev = terminal
while (len(edges) < m):
    u, v = randint(1, n), randint(1, n)
    while (tuple((v, u)) in edges or u == v):
        u, v = randint(1, n), randint(1, n)
    edges.add(tuple((u, v)))
print(n, m)
for edge in edges:
    print(*edge, randint(0, 100))
print(t)
print(" ".join(str(terminal) for terminal in terminals))