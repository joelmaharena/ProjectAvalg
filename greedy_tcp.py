# GreedyTour
#    tour[0] = 0
#    used[0] = true
#    for i = 1 to n-1
#       best = -1
#       for j = 0 to n-1
#          if not used[j] and (best = -1 or dist(tour[i-1],j) < dist(tour[i-1],best))
#             best = j
#       tour[i] = best
#       used[best] = true
#    return tour

import math


def dist(a, b):
    return math.hypot(a[0] - b[0], a[1] - b[1])

def greedy_tour(points):
    n = len(points)
    tour = [0] * n
    used = [False] * n
    used[0] = True

    for i in range(1, n):
        best = -1
        for j in range(n):
            if not used[j] and (best == -1 or dist(points[tour[i - 1]], points[j]) < dist(points[tour[i - 1]], points[best])):
                best = j
        tour[i] = best
        used[best] = True

    return tour

    