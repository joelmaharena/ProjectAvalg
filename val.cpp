#include <iostream>
#include <tuple>
#include <vector>
#include <math.h>
#include <algorithm>
#include <random>
using namespace std;

struct Point
{
	double x;
	double y;
	vector<int> neighbours;
	Point() = default;
	Point(double x, double y) : x(x), y(y) {}
};

double dist(Point& p1, Point& p2)
{
	double dx = p1.x - p2.x;
	double dy = p1.y - p2.y;
	return sqrt(pow(dx, 2) + pow(dy, 2));
}
int N;
vector<vector<double>> DIST_MATRIX;
vector<Point> POINTS;

double total_length(const vector<int>& tour){
	double sum = 0;
	int prev = tour.back();
	for (size_t i = 0; i < tour.size(); i++)
	{
		int cur = tour[i];
		sum += DIST_MATRIX[prev][cur];
		// sum += dist(POINTS[prev] , POINTS[tour[i]]);
		prev = tour[i];
	}
	return sum;
}


vector<vector<int>> greedy_tour()
{
	std:mt19937 generator(42);
	vector<int> starts(N);
	iota(starts.begin(), starts.end(), 0);
	shuffle(starts.begin(), starts.end(), generator);
	std::uniform_int_distribution<int> distribution(0, N - 1);
	double best_len = 32983298;
	vector<int> tour(N);
	vector<bool> used(N);
	int z = min(110, N);
	vector<vector<int>> tours;
	for (size_t a = 0; a < z; a++) {
		fill(used.begin(), used.end(), false);	
		int random_num = starts[a];
		tour[0] = random_num;
		used[random_num] = true;
		for (size_t i = 1; i < N; i++)
		{
			int best = -1;
			for (size_t j = 0; j < N; j++)
			{
				if (!used[j] && (best == -1 || DIST_MATRIX[tour[i - 1]][j] < DIST_MATRIX[tour[i - 1]][best]))
				{
					best = j;
				}
			}
			tour[i] = best;
			used[best] = true;
		}
		tours.push_back(tour);
	}
	return tours;
}
void read_input()
{
	double x, y = 0;

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

	DIST_MATRIX.assign(N, vector<double>(N));
      for (int i = 0; i < N; i++){
          for (int j = 0; j < N; j++){
              DIST_MATRIX[i][j] = dist(POINTS[i], POINTS[j]);
		}
	}
}
void set_neighbours(int p)
{
	int min_neighbours = 20;
	int thresh = min(min_neighbours, N-1);

	vector<tuple<double, int>> distances;
	distances.reserve(N);
	for (int i = 0; i < N; i++)
		if (i != p)
			distances.push_back({DIST_MATRIX[p][i], i});

	partial_sort(distances.begin(), distances.begin() + thresh, distances.end());

	POINTS[p].neighbours.clear();
	for (int i = 0; i < thresh; i++)
		POINTS[p].neighbours.push_back(get<1>(distances[i]));
}


vector<int> opt_alg(vector<int> tour)
{
	int n = tour.size();

	// pos[city] = that city's index (slot) in the tour
	vector<int> pos(n);
	for (int i = 0; i < n; i++)
		pos[tour[i]] = i;

	// precompute each city's nearest-neighbour tour list once

	bool improved = true;
	while (improved)
	{
		improved = false;
		for (int i = 0; i < n; i++)
		{
			int c1 = tour[i];
			int c2 = tour[(i + 1) % n]; // successor of c1 (wraps at the end)

			for (int c3 : POINTS[c1].neighbours)
			{
				if (c3 == c2)
					continue;
				int j = pos[c3];
				int c4 = tour[(j + 1) % n]; // successor of c3
				if (c4 == c1)
					continue;

				double removed = DIST_MATRIX[c1][c2] + DIST_MATRIX[c3][c4];
				double added = DIST_MATRIX[c1][c3] + DIST_MATRIX[c2][c4];

				if (removed - added > 1e-6)
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

void brute_force(vector<int> & tour) {
	tour[0] = 0;
	vector<int> other_cities(N);
	for (int i = 0; i < N; i++) {
		other_cities[i] = i ;
	}
	double best_len = 93293;
	while(next_permutation(other_cities.begin(), other_cities.end())) {
		double len = total_length(other_cities);
		if(len < best_len) {
			best_len = len;
			tour = other_cities;
		}
	}
}


vector<int> second_run(vector<int> best, double len, int attempts) {
	double best_len = len;
	mt19937 generator(43);
	vector<int> pos(best.size());
	iota(pos.begin(), pos.end(), 0);

	for (int i = 0; i < attempts; i++) {
		vector<int> tour = best;

		shuffle(pos.begin(), pos.end(), generator);

		swap(tour[pos[0]], tour[pos[1]]);
		swap(tour[pos[2]], tour[pos[3]]);

		tour = opt_alg(tour);
		double len = total_length(tour);
		if(len < best_len) {
			best_len = len;
			best = tour;
		}
	}

	return best;
}

int main()
{
	read_input();
	construct_matrix();
	vector<vector<int>> tours = greedy_tour();

	//Comment out before submitting to avoid printing the tour and its length
	// for (size_t i = 0; i < tour.size(); i++)
	// {
	// 	cout << tour[i] << endl;
	// }
	// cout << "Total lenght : " << total_length(tour) << endl;
	// cout << "-----------------OPT-------------" << endl;
	//This ends comment block before submitting to avoid printing the tour and its length
	vector<int> opt(N), best_opt(N);
	double best_len = 9329392393;
	if(N < 12) {
		brute_force(best_opt);
	} else {
		for (int c = 0; c < N; c++)
			set_neighbours(c);
		for (auto& tour: tours) {
		
			opt = opt_alg(tour);
			double len = total_length(opt);
			if(len < best_len) {
				best_opt = opt;
				best_len = len;
			}	
		}
	}
	best_opt = second_run(best_opt, best_len, 850);
	for (size_t i = 0; i < best_opt.size(); i++)
	{
		cout << best_opt[i] << endl;
	}

	// this Line also needs to be commented out before submitting to avoid printing the length
	// cout << "Total lenght : " << total_length(opt) << endl;
}

