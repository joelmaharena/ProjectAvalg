import glob
import math
import subprocess
import sys
import time
 
from greedy_tcp import greedy_tour
 
src = "val.cpp"
subprocess.run(["g++", "-O2", "-std=c++17", "-o", "tsp_bin", src], check=True)
 
 
def tour_length(points, tour):
    # Kattis: each edge is the Euclidean distance rounded to the nearest integer,
    # and the tour returns from the last city to the first.
    total = 0
    for i in range(len(tour)):
        a = points[tour[i]]
        b = points[tour[(i + 1) % len(tour)]]
        total += int(math.hypot(a[0] - b[0], a[1] - b[1]) + 0.5)
    return total
 
 
print(f"{'test':22s} {'N':>5s} {'time':>7s} {'lines':>6s} {'yours':>12s} {'greedy':>12s} {'vs greedy':>10s}")
for path in sorted(glob.glob("tests/*.in")):
    text = open(path).read()
    numbers = text.split()
    n = int(numbers[0])
    points = [(float(numbers[1 + 2 * i]), float(numbers[2 + 2 * i])) for i in range(n)]
 
    start = time.perf_counter()
    result = subprocess.run(["./tsp_bin"], input=text, capture_output=True, text=True)
    seconds = time.perf_counter() - start
 
    lines = result.stdout.split()
    name = path.split("/")[-1][:-3]
 
    if len(lines) != n:
        print(f"{name:22s} {n:5d} {seconds:6.3f}s {len(lines):6d}   WRONG LINE COUNT (expected {n})")
        continue
 
    yours = tour_length(points, [int(x) for x in lines])
    greedy = tour_length(points, greedy_tour(points))
    better = 100 * (greedy - yours) / greedy if greedy > 0 else 0.0
 
    print(f"{name:22s} {n:5d} {seconds:6.3f}s {len(lines):6d} {yours:12d} {greedy:12d} {better:9.1f}%")
 
