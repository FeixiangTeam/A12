#include "GA/define.hpp"
#include "data.hpp"
#include <random>
#include <algorithm>
#include <utility>

static bool flag[MAX_TARGET_NUM + 1];
static int degree[MAX_TARGET_NUM + 1];

void Cross(std::vector<Individual> &dst, const Individual &a, const Individual &b) {
	const int tv_num = data["target_vertex_set"].size();
	static std::uniform_int_distribution<> choice_gen(1, tv_num);
	Individual res;
	do {
		std::fill(res.next.begin(), res.next.end(), 0);
		for(int p = choice_gen(rand_engine); p; p = a.next[p]) {
			res.next[p] = a.next[p];
			flag[p] = true;
		}
		std::fill(degree + 1, degree + tv_num + 1, 0);
		for(int i = 1; i <= tv_num; ++i) ++degree[b.next[i]];
		for(int i = 1; i <= tv_num; ++i)
			if(degree[i] == 0) {
				int pre = 0;
				for(int p = i; p; p = b.next[p])
					if(flag[p]) flag[p] = false;
					else {
						if(pre != 0) res.next[pre] = p;
						pre = p;
					}
			}
	} while(!res.Calc());
	dst.push_back(std::move(res));
}
