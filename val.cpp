#include <iostream>
#include <tuple>
#include <vector>
#include <math.h>
using namespace std;

struct Line{
	tuple<float, float> p1;
	tuple<float, float> p2;
	int d;


};

float dist(tuple<float, float> p1 , tuple<float, float> p2){
	return sqrt(pow(get<0>(p1) - get<0>(p2) , 2) + pow(get<1>(p1) - get<1>(p2) , 2));
}

vector<int> greedy_tour(vector<tuple<float,float>> points , int n){
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
void read_input(vector<tuple<float,float>> &coordinates , int &n) {
	float x,y = 0;

	cin >> n;
	while (cin >> x >> y){
		coordinates.emplace_back(x,y);
	}
	for (auto& x : coordinates){
		cout << "x:" << get<0>(x) << " y: " << get<1>(x)<< endl;
	}
}
int main() {
	vector<tuple<float,float>> coordinates;
	int n = 0;
	read_input(coordinates, n);
	vector<int> tour = greedy_tour(coordinates , n);
	for (size_t i = 0; i < tour.size(); i++)
	{
		cout << tour[i] << endl;
	}
	
}
