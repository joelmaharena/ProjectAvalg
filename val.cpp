#include <iostream>
#include <tuple>
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;

struct Point
{
	float x;
	float y;
	vector<int> neightbours;
	Point() = default;
	Point(float x, float y) : x(x), y(y) {}
};

float dist(Point p1, Point p2)
{
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return sqrt(pow(dx, 2) + pow(dy, 2));
}
int N;
vector<vector<float>> DIST_MATRIX;
vector<Point> POINTS;

vector<int> greedy_tour()
{

	vector<int> tour(N);
	vector<bool> used(N);
	tour[0] = 0;
	used[0] = true;
	for (size_t i = 1; i < N; i++)
	{
		int best = -1;
		for (size_t j = 0; j < N; j++)
		{
			if (!used[j] && (best == -1 || dist(POINTS[tour[i - 1]], POINTS[j]) < dist(POINTS[tour[i - 1]], POINTS[best])))
			{
				best = j;
			}
		}
		tour[i] = best;
		used[best] = true;
	}
	return tour;
}
void read_input()
{
	float x, y = 0;

	cin >> N;
	while (cin >> x >> y)
	{
		 POINTS.push_back({x, y});
	}
	
	/*for (auto& x : coordinates){
		cout << "x:" << get<0>(x) << " y: " << get<1>(x)<< endl;
	}*/
}
void construct_matrix()
{

	DIST_MATRIX.assign(N, vector<float>(N));
      for (int i = 0; i < N; i++){
          for (int j = 0; j < N; j++){
              DIST_MATRIX[i][j] = dist(POINTS[i], POINTS[j]);
		}
	}
}
void set_neightbours(int p)
{
	int thresh = min(10, N);

	vector<tuple<float, int>> distances;
	distances.reserve(N);
	for (int i = 0; i < N; i++)
		if (i != p)
			distances.push_back({DIST_MATRIX[p][i], i});

	sort(distances.begin(), distances.end());

	POINTS[p].neightbours.clear();
	for (int i = 0; i < thresh; i++)
		POINTS[p].neightbours.push_back(get<1>(distances[i]));
}
vector<int> opt_alg(vector<int> tour)
{
	int n = tour.size();

	// pos[city] = that city's index (slot) in the tour
	vector<int> pos(n);
	for (int i = 0; i < n; i++)
		pos[tour[i]] = i;

	// precompute each city's nearest-neighbour candidate list once
	for (int c = 0; c < n; c++)
		set_neightbours(c);

	bool improved = true;
	while (improved)
	{
		improved = false;
		for (int i = 0; i < n; i++)
		{
			int c1 = tour[i];
			int c2 = tour[(i + 1) % n]; // successor of c1 (wraps at the end)

			for (int c3 : POINTS[c1].neightbours)
			{
				if (c3 == c2)
					continue;

				// neighbours are sorted by distance: once the candidate edge
				// (c1,c3) is no shorter than the edge we'd remove (c1,c2),
				// no forward 2-opt move through c1 can improve -> stop early
				if (DIST_MATRIX[c1][c3] >= DIST_MATRIX[c1][c2])
					break;

				int j = pos[c3];
				int c4 = tour[(j + 1) % n]; // successor of c3
				if (c4 == c1)
					continue;

				float removed = DIST_MATRIX[c1][c2] + DIST_MATRIX[c3][c4];
				float added = DIST_MATRIX[c1][c3] + DIST_MATRIX[c2][c4];

				if (removed - added > 1e-6f)
				{
					// 2-opt move: reverse the tour segment between the two
					// broken edges, i.e. slots (lo+1 .. hi)
					int lo = min(i, j);
					int hi = max(i, j);
					reverse(tour.begin() + lo + 1, tour.begin() + hi + 1);
					for (int k = lo + 1; k <= hi; k++)
						pos[tour[k]] = k;

					improved = true;
					break; // c1's successor changed; move on
				}
			}
		}
	}
	return tour;
}
float total_length(vector<int> tour){
	int sum = 0;
	int prev = tour[0];
	for (size_t i = 0; i < tour.size(); i++)
	{
		sum += dist(POINTS[prev] , POINTS[i]);

	}
	return sum;
}

int main()
{
	read_input();
	vector<int> tour = greedy_tour();
	construct_matrix();
	for (size_t i = 0; i < tour.size(); i++)
	{
		cout << tour[i] << endl;
	}
	cout << "Total lenght : " << total_length(tour) << endl;
	cout << "-----------------OPT-------------" << endl;
	vector<int> opt = opt_alg(tour);
	for (size_t i = 0; i < opt.size(); i++)
	{
		cout << opt[i] << endl;
	}
	cout << "Total lenght : " << total_length(opt) << endl;
}
