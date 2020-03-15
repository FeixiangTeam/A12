#include "data.hpp"
#include <vector>
#include <algorithm>
#include <limits>
#include <tuple>
#include <utility>

const size_t MAXN=20;

double weight[1<<MAXN];

double cost[1<<MAXN];
int start_by[1<<MAXN];

double path[1<<MAXN][MAXN];
int from[1<<MAXN][MAXN];

double f[1<<MAXN];
int p[1<<MAXN];

struct Truck {
	double limit, cost;
	operator double() { return limit; }
};
std::vector<Truck> trucks;

void Solve() {
	size_t num=data["target_vertex_set"].size();

	for(size_t i=0; i<num; ++i) {
		double w=data["target_vertex_set"][i]["target"];
		for(int j=0; j<(1<<i); ++j)
			weight[j|1<<i]=weight[j]+w;
	}

	std::fill(cost, cost+sizeof(cost)/sizeof(double), std::numeric_limits<double>::infinity());
	std::fill(*path, *path+sizeof(path)/sizeof(double), std::numeric_limits<double>::infinity());
	for(int i=0; i<(1<<num); ++i)
		for(size_t j=0; j<num; ++j)
			if(i&1<<j) {
				double g=path[i][j]+Map::dis[0][j+1];
				if(g<cost[i]) {
					cost[i]=g;
					start_by[i]=j;
				}
			} else
				if(i==0)
					path[1<<j][j]=Map::dis[j+1][0];
				else
					for(size_t k=0; k<num; ++k)
						if(i&1<<k) {
							double g=path[i][k]+Map::dis[j+1][k+1];
							if(g<path[i|1<<j][j]) {
								path[i|1<<j][j]=g;
								from[i|1<<j][j]=k;
							}
						}

	for(const auto truck: data["truck_set"])
		trucks.push_back({truck["limit"], truck["cost"]});
	std::sort(trucks.begin(), trucks.end(), [](const Truck &a, const Truck &b) { return a.cost<b.cost; });
	size_t n=0;
	for(size_t i=1; i<trucks.size(); ++i)
		if(trucks[i].limit>trucks[n].limit)
			trucks[++n]=std::move(trucks[i]);
	trucks.resize(n+1);

	std::fill(f, f+sizeof(f)/sizeof(double), std::numeric_limits<double>::infinity());
	f[0]=0;
	for(int i=0; i<(1<<num); ++i)
		for(int j=i; j>0; j=(j-1)&i) {
			auto k=std::lower_bound(trucks.begin(), trucks.end(), weight[j]);
			if(k!=trucks.end()) {
				double g=f[i^j]+cost[j]+k->cost;
				if(g<f[i]) {
					f[i]=g;
					p[i]=j;
				}
			}
		}

	int i=(1<<num)-1;
	while(i>0) {
		int j=p[i]; i^=j;
		std::vector<int> res;
		int k=start_by[j];
		while(j>0) {
			res.push_back(k+1);
			std::tie(j, k)=std::tuple(j^(1<<k), from[j][k]);
		}
		Answer::AddPath(std::move(res));
	}
}
