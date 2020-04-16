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
double target[MAX_TARGET_NUM + 1];

void GAInit() {
	best.fitness = 0;

	const auto &truck_set = data["truck_set"];
	for(const auto &truck: truck_set) {
		const auto &num = truck["num"];
		if(!num.is_null()) {
			total_truck_num += num.get<int>();
		} else {
			total_truck_num = tv_num;
			break;
		}
	}

	const int truck_type_num = truck_set.size();
	for(int i = 0; i < truck_type_num; ++i) {
		const auto &truck = truck_set[i];
		const auto &num = truck["num"];
		int tnum;
		if(!num.is_null()) tnum = num.get<int>();
		else tnum = tv_num;
		if(tnum > 0) trucks.push_back({i, tnum, truck["limit"].get<double>()});
	}
	std::sort(trucks.begin(), trucks.end());

	const auto &target_vertex_set = data["target_vertex_set"];
	for(int i = 1; i <= tv_num; ++i)
		target[i] = target_vertex_set[i - 1]["target"].get<double>();
}

static int degree[MAX_TARGET_NUM + 1];
static double weight[MAX_TARGET_NUM];

bool Individual::Calc() {
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
			for(int p = i; p; p = next[p]) weight[j] += target[p];
			++j;
		}
	std::sort(weight, weight + truck_num);

	int used = 0;
	for(int i = 0, j = 0; i < truck_num; ++i) {
		if(weight[i] > trucks[j].limit || used >= trucks[j].num) {
			++j; used = 0;
			while(j < truck_type_num && weight[i] > trucks[j].limit) ++j;
			if(j >= truck_type_num) return false;
		}
		++used;
	}

	double cost = BETA * truck_num;
	for(int i = 1; i <= tv_num; ++i) {
		if(degree[i] == 0) cost += Map::dis[0][i];
		cost += Map::dis[i][next[i]];
	}
	fitness = FITNESS_FACTOR / cost;

	if(fitness > best.fitness) best = *this;
	return true;
}
