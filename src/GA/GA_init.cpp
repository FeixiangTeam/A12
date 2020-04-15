#include "GA/define.hpp"
#include "data.hpp"
#include "GA/config.hpp"
#include <algorithm>

std::random_device rd;
std::mt19937 rand_engine(rd());

bool Random(double Pr) {
	return rand_engine() <= Pr * rand_engine.max();
}

int total_truck_num;
Individual best;

std::vector<Truck> trucks;

void GAInit() {
	const int tv_num = data["target_vertex_set"].size();

	best.fitness = 0;

	for(const auto &truck: data["truck_set"]) {
		const auto &num = truck["num"];
		if(!num.is_null()) {
			total_truck_num += num.get<int>();
		} else {
			total_truck_num = tv_num;
			break;
		}
	}

	const int truck_type_num = data["truck_set"].size();
	for(int i = 0; i < truck_type_num; ++i) {
		const auto &truck = data["truck_set"][i];
		const auto &num = truck["num"];
		int tnum;
		if(!num.is_null()) tnum = num.get<int>();
		else tnum = tv_num;
		if(tnum > 0) trucks.push_back({i, tnum, truck["limit"].get<double>()});
	}
	std::sort(trucks.begin(), trucks.end());
}

Individual::Individual(): next(data["target_vertex_set"].size() + 1) { }

static int degree[MAX_TARGET_NUM + 1];
static double weight[MAX_TARGET_NUM];

bool Individual::Calc() {
	const int tv_num = data["target_vertex_set"].size();
	const int truck_type_num = trucks.size();

	truck_num = 0;
	std::fill(degree + 1, degree + tv_num + 1, 0);
	for(int i = 1; i <= tv_num; ++i) {
		if(next[i] == 0) ++truck_num;
		else ++degree[next[i]];
	}
	if(truck_num > total_truck_num) return false;

	for(int i = 1, j = 0; i <= tv_num; ++i)
		if(degree[i] == 0) {
			weight[j] = 0;
			for(int p = i; p; p = next[p])
				weight[j] += data["target_vertex_set"][p - 1]["target"].get<double>();
			++j;
		}
	std::sort(weight, weight + truck_num);

	for(int i = 0, j = 0, used = 0; i < truck_num; ++i) {
		if(weight[i] > trucks[j].limit) {
			++j; used = 0;
			while(j < truck_type_num && weight[i] > trucks[j].limit) ++j;
			if(j >= truck_type_num) return false;
		}
		++used;
		if(used >= trucks[j].num) { ++j; used = 0; }
	}

	double cost = BETA * truck_num;
	for(int i = 1; i <= truck_num; ++i)
		if(degree[i] == 0) cost += Map::dis[0][i];
		else cost += Map::dis[i][next[i]];
	fitness = FITNESS_FACTOR / cost;

	if(fitness > best.fitness) best = *this;
	return true;
}
