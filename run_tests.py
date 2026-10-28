
import glob
import math
import subprocess
import sys
import time
 
from greedy_tcp import greedy_tour
 
src = "tsp.cpp" # the cpp file holding the opt algorithm

# compile the C++ code
subprocess.run(["g++", "-O2", "-std=c++17", "-o", "tsp_bin", src], check=True)
 
 
def tour_length(points, tour):
    # In Kattis each edge is the Euclidean distance rounded to the nearest integer,
    # and the tour returns from the last city to the first.
    total = 0
    for i in range(len(tour)):
        a = points[tour[i]]
        b = points[tour[(i + 1) % len(tour)]]
        total += int(math.hypot(a[0] - b[0], a[1] - b[1]) + 0.5)
    return total
 
 
print(f"{'test'} {'N'} {'time'} {'lines'} {'yours'} {'greedy'} {'vs greedy'}")
for path in sorted(glob.glob("tests/*.in")): # loop through all test files
    text = open(path).read()
    
    # Save all points as input to the C++ program, 
    # and also parse them in Python to compute the greedy tour length.
    numbers = text.split()
    n = int(numbers[0])
    points = [(float(numbers[1 + 2 * i]), float(numbers[2 + 2 * i])) for i in range(n)]
 
    start = time.perf_counter() # start the timer
    result = subprocess.run(["./tsp_bin"], input=text, capture_output=True, text=True)
    seconds = time.perf_counter() - start # stop the timer
 
    lines = result.stdout.split() # Get the output from the C++ program, 
    # which should be a list of integers representing the tour.
    name = path.split("/")[-1][:-3] # Get the test name from the file path, 
    # removing the directory and extension.
 
    if len(lines) != n:
        # If the number of lines in the output does not match the expected number of points, 
        # print an error message and continue to the next test.
        print(f"{name} {n} {seconds}s {len(lines)}   WRONG LINE COUNT (expected {n})")
        continue
 
    # Calculate the tour lengths for both our solution and the greedy solution,
    yours = tour_length(points, [int(x) for x in lines])
    greedy = tour_length(points, greedy_tour(points))
    # Calculate the percentage improvement of our solution over the greedy solution.
    better = 100 * (greedy - yours) / greedy if greedy > 0 else 0.0
 
    print(f"{name} {n} {seconds}s {len(lines)} {yours} {greedy} {better}%")
 
