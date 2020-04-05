#include "GA/define.hpp"
#include "data.hpp"
#include <cstddef>
#include <algorithm>

static int idx[MAX_TARGET_NUM + 1];
static double dis_of[MAX_TARGET_NUM + 1];
static int pre[MAX_TARGET_NUM + 1];

std::vector<Individual> IndividualInit(std::size_t num) {
	static const int tv_num = data["target_vertex_set"].size();
	static std::uniform_int_distribution<> truck_num_gen(std::max(total_truck_num - 3, 1), total_truck_num);

	for(int i = 1; i <= tv_num; ++i) idx[i] = i;
	std::vector<Individual> res(num);
	for(std::size_t i = 0; i < num; ++i) {
		Individual &now = res[i];
		do {
			std::shuffle(idx + 1, idx + tv_num + 1, rand_engine);
			int truck_num = truck_num_gen(rand_engine);

			for(int j = truck_num + 1; j <= tv_num; ++j) {
				int p = idx[j];
				dis_of[j] = Map::dis[idx[1]][p];
				pre[p] = idx[1];
				for(int k = 2; k <= truck_num; ++k)
					if(Map::dis[idx[k]][p] < dis_of[j]) {
						dis_of[j] = Map::dis[idx[k]][p];
						pre[p] = idx[k];
					}
			}

			for(int j = tv_num; j > truck_num; --j) {
				int d = std::min_element(dis_of + truck_num + 1, dis_of + j + 1) - dis_of;
				std::swap(idx[d], idx[j]);
				dis_of[d] = dis_of[j];
				int p = idx[j];
				now.next[pre[p]] = p;
				for(int k = truck_num + 1; k < j; ++k) {
					int q = idx[k];
					if(Map::dis[p][q] < dis_of[k] || pre[q] == pre[p]) {
						dis_of[k] = Map::dis[p][q];
						pre[q] = p;
					}
				}
			}
		} while(!now.Calc());
	}

	return res;
}
