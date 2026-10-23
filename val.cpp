#include <iostream>
#include <tuple>
#include <vector>
#include <math.h>
using namespace std;


struct Point{
	float x;
	float y;
};

float dist(Point p1 , Point p2){
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return sqrt(pow(dx , 2) + pow(dy, 2));
}

vector<int> greedy_tour(vector<Point> points){
	int n = points.size();
	vector<int> tour(n);
	vector<bool> used(n);
	tour[0] = 0;
	used[0] = true;
	for (size_t i = 1; i < n; i++)
	{
		int best = -1;
		for (size_t j = 0; j < n; j++)
		{
			if (!used[j] && (best == -1 || dist(points[tour[i - 1]], points[j]) < dist(points[tour[i - 1]] , points[best])))
			{
				best = j;
			}
		}
		tour[i] = best;
		used[best] = true;
	}
	return tour;

}
void read_input(vector<Point> &coordinates , int &n) {
	float x,y = 0;

	cin >> n;
	while (cin >> x >> y){
		coordinates.emplace_back(x,y);
	}
	/*for (auto& x : coordinates){
		cout << "x:" << get<0>(x) << " y: " << get<1>(x)<< endl;
	}*/
}
void construct_list(vector<Point> points){
	vector<vector<float>> n_matrix(64);
	for (size_t i = 0; i < n_matrix.size(); i++)
	{
		for (size_t j = 0; j < n_matrix[1].size(); j++)
		{
			
		}
		
	}
	
}
vector<int> opt_alg(vector<int> tour){
	
}
int main() {
	vector<Point> coordinates;
	int n = 0;
	read_input(coordinates, n);
	vector<int> tour = greedy_tour(coordinates);
	for (size_t i = 0; i < tour.size(); i++)
	{
		cout << tour[i] << endl;
	}
	
}
