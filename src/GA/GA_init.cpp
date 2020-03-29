#include "GA/define.hpp"
#include "data.hpp"
#include "GA/config.hpp"
#include <cstring>

std::random_device rd;
std::mt19937 rand_engine(rd());

int total_truck_num;

void GAInit() {
	for(const auto &truck: data["truck_set"]) {
		const auto &num=truck["num"];
		if(!num.is_null()) {
			total_truck_num+=num.get<int>();
		} else {
			total_truck_num=data["target_vertex_set"].size();
			break;
		}
	}
}

Individual::Individual() {
	std::memset(next, 0x00, sizeof(next));
}
