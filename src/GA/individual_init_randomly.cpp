#include "GA/define.hpp"
#include "data.hpp"
#include <random>
#include <vector>
#include <cstddef>
#include <algorithm>

static int idx[MAX_TARGET_NUM + 1];
static int choice[MAX_TARGET_NUM + 1];

std::vector<Individual> IndividualInit(std::size_t num) {
	static std::uniform_int_distribution<> truck_num_gen(std::max(total_truck_num - 3, 1), total_truck_num);

	for(int i = 1; i <= tv_num; ++i) idx[i] = i;
	for(int i = 1; i < tv_num; ++i) choice[i] = i;
	std::vector<Individual> res(num);
	for(std::size_t k = 0; k < num; ++k) {
		Individual &now = res[k];
		do {
			std::shuffle(idx + 1, idx + tv_num + 1, rand_engine);
			std::shuffle(choice + 1, choice + tv_num, rand_engine);
			int truck_num = truck_num_gen(rand_engine);
			std::sort(choice + 1, choice + truck_num);
			for(int i = 1, j = 1; i < tv_num; ++i)
				if(j < truck_num && choice[j] == i) {
					++j; now.next[idx[i]] = 0;
				} else {
					now.next[idx[i]] = idx[i + 1];
				}
			now.next[idx[tv_num]] = 0;
		} while(!now.Calc());
	}

	return res;
}
