#include "data.hpp"
#include <cstddef>
#include <vector>
#include <algorithm>
#include <limits>
#include <tuple>
#include <utility>

constexpr std::size_t MAXN = 20;

double weight[1 << MAXN];

double cost[1 << MAXN];
int start_by[1 << MAXN];

double path[1 << MAXN][MAXN];
int from[1 << MAXN][MAXN];

double f[1 << MAXN];
int p[1 << MAXN];

struct Truck {
	std::size_t id;
	double limit, cost;
	operator double() { return limit; }
	friend bool operator<(const Truck &x, const Truck &y) {
		return x.cost < y.cost;
	}
};
std::vector<Truck> trucks;

void Solve() {
	for(int i = 0; i < tv_num; ++i) {
		double w = data["target_vertex_set"][i]["target"];
		for(int j = 0; j < (1 << i); ++j)
			weight[j | 1 << i] = weight[j] + w;
	}

	std::fill(cost, cost + sizeof(cost) / sizeof(double), std::numeric_limits<double>::infinity());
	std::fill(*path, *path + sizeof(path) / sizeof(double), std::numeric_limits<double>::infinity());
	for(int i = 0; i < (1 << tv_num); ++i)
		for(int j = 0; j < tv_num; ++j)
			if(i & 1 << j) {
				double g = path[i][j] + Map::dis[0][j + 1];
				if(g < cost[i]) {
					cost[i] = g;
					start_by[i] = j;
				}
			} else {
				if(i == 0)
					path[1 << j][j] = Map::dis[j + 1][0];
				else
					for(int k = 0; k < tv_num; ++k)
						if(i & 1 << k) {
							double g = path[i][k] + Map::dis[j + 1][k + 1];
							if(g < path[i | 1 << j][j]) {
								path[i | 1 << j][j] = g;
								from[i | 1 << j][j] = k;
							}
						}
			}

	const auto &truck_set = data["truck_set"];
	for(std::size_t i = 0; i < truck_set.size(); ++i) {
		const auto &truck = truck_set[i];
		trucks.push_back({i, truck["limit"], truck["cost"]});
	}
	std::sort(trucks.begin(), trucks.end());
	std::size_t n = 0;
	for(std::size_t i = 1; i < trucks.size(); ++i)
		if(trucks[i].limit > trucks[n].limit)
			trucks[++n] = std::move(trucks[i]);
	trucks.resize(n + 1);

	std::fill(f, f + sizeof(f) / sizeof(double), std::numeric_limits<double>::infinity());
	f[0] = 0;
	for(int i = 0; i < (1 << tv_num); ++i)
		for(int j = i; j > 0; j = (j - 1) & i) {
			auto k = std::lower_bound(trucks.begin(), trucks.end(), weight[j]);
			if(k != trucks.end()) {
				double g = f[i ^ j] + cost[j] + k->cost;
				if(g < f[i]) {
					f[i] = g;
					p[i] = j;
				}
			}
		}

	int i = (1 << tv_num) - 1;
	while(i > 0) {
		int j = p[i]; i ^= j;
		double w = weight[j];
		std::size_t t = std::lower_bound(trucks.begin(), trucks.end(), w)->id;
		std::vector<int> res;
		int k = start_by[j];
		while(j > 0) {
			res.push_back(k + 1);
			std::tie(j, k)=std::tuple(j ^ (1 << k), from[j][k]);
		}
		answer.push_back({
			{"full_path", Map::GetNamedPath(Map::GetFullPath(res))},
			{"target_path", Map::GetNamedPath(res)},
			{"distance", Map::CalcPathDistance(res)},
			{"weight", w},
			{"truck", truck_set[t]["name"]},
			{"ratio", w / truck_set[t]["limit"].get<double>()}
		});
	}
}
