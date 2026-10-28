import os
import random
import sys
 
LIMIT = 1_000_000  # max absolute coordinate allowed by Kattis
 
 
# Point generators. Each one takes a random r from a seed and a size n
# and returns a list of (x, y) tuples. 

# Generates points spread randomly between limits given.
def uniform(r, n, lo=-LIMIT, hi=LIMIT):
    return [(r.uniform(lo, hi), r.uniform(lo, hi)) for _ in range(n)]
 
# Generate points spread around a few cluster centres. 
def clustered(r, n, n_clusters, spread):
    margin = 3 * spread  # keep centres away from the edge so points stay in range
    # create centers randomly
    centres = [(r.uniform(-LIMIT + margin, LIMIT - margin),
                r.uniform(-LIMIT + margin, LIMIT - margin))
               for _ in range(n_clusters)]
    pts = []
    for _ in range(n):
        cx, cy = r.choice(centres)
        # Use Gauss distribution to get points randomly spread around centers
        pts.append((cx + r.gauss(0, spread), cy + r.gauss(0, spread)))
    return pts
 

# Create points in a straight line, where y-value is bound to 0
def on_line(r, n):
    return [(r.uniform(-LIMIT, LIMIT), 0.0) for _ in range(n)]
 
# Generate few distinct points, then pick randomly from these points meaning duplicates must happen
def with_duplicates(r, n, n_unique):
    spots = uniform(r, n_unique)
    return [r.choice(spots) for _ in range(n)]
 
# The test set: (file name, seed, function producing the points)
# Add or remove lines here to change the set. Keep old seeds unchanged so
# old results stay comparable.
 
CASES = [
    # Random points, check mostly for size 1000 since that is where the largest changes can happen
    ("uniform_0100",       101, lambda r: uniform(r, 100)),
    ("uniform_0300",       102, lambda r: uniform(r, 300)),
    ("uniform_1000_a",     103, lambda r: uniform(r, 1000)),
    ("uniform_1000_b",     104, lambda r: uniform(r, 1000)),
    ("uniform_1000_c",     105, lambda r: uniform(r, 1000)),
 
    # Setup a bunch of clustered points to see how the algorithm works in the presence of long and small jumps
    ("cluster_0300",       201, lambda r: clustered(r, 300, n_clusters=6, spread=30_000)),
    ("cluster_1000_few",   202, lambda r: clustered(r, 1000, n_clusters=5, spread=50_000)),
    ("cluster_1000_many",  203, lambda r: clustered(r, 1000, n_clusters=30, spread=20_000)),
 
    # Coordinates where the points are between 0 and 1, so the distances are very small. 
    # This is to check that the algorithm does not lose precision.
    ("small_coords_0200",  301, lambda r: uniform(r, 200, lo=0, hi=1)),
    ("small_coords_1000",  302, lambda r: uniform(r, 1000, lo=0, hi=1)),
 
    # Edge Cases for on one line, for duplicates, and small number of points.
    ("edge_n1",            401, lambda r: uniform(r, 1)),
    ("edge_n2",            402, lambda r: uniform(r, 2)),
    ("edge_n3",            403, lambda r: uniform(r, 3)),
    ("line_0100",          404, lambda r: on_line(r, 100)),
    ("duplicates_0200",    405, lambda r: with_duplicates(r, 200, n_unique=20)),

]
 
  
def clamp(v):
    return max(-LIMIT, min(LIMIT, v))
 
 
def write_case(path, pts):
    with open(path, "w") as f:
        f.write(f"{len(pts)}\n")
        for x, y in pts:
            # 4 decimals, like typical Kattis input; clamp guards the 10^6 limit
            f.write(f"{clamp(x):.4f} {clamp(y):.4f}\n")
 
 
def main():
    out_dir = "tests"
    os.makedirs(out_dir, exist_ok=True)
    for name, seed, make in CASES:
        r = random.Random(seed)       # own generator per case -> reproducible
        pts = make(r)
        assert 1 <= len(pts) <= 1000, name
        write_case(os.path.join(out_dir, name + ".in"), pts)
        print(f"{name + '.in':24s} N = {len(pts)}")
    print(f"\nWrote {len(CASES)} files to {out_dir}/")
 
 
if __name__ == "__main__":
    main()
 
