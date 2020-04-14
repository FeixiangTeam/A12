#include "GA/define.hpp"
#include "data.hpp"
#include <algorithm>

const int times_mu = 5;
int first[MAX_TARGET_NUM + 1], degree[MAX_TARGET_NUM + 1];

void Mutation(std::vector<Individual> &dst, Individual x) {
	using std::swap;
	const int tv_num = data["target_vertex_set"].size();
	static std::uniform_int_distribution<> choice_gen(1, tv_num);

	std::fill(degree + 1, degree + tv_num + 1, 0);
	for(int i = 1; i <= tv_num; ++i) ++degree[x.next[i]];
	for(int i = 1; i <= tv_num; ++i) if(degree[i] == 0)
		for(int p = i; p; p = x.next[p]) first[p] = i;

	bool flag = false;
	for(int k = 1; k <= times_mu; ++k) {
		Individual res = x;
		int a = choice_gen(rand_engine), b = choice_gen(rand_engine);
		if(a == b) continue;
		if(first[a] == first[b]) {
			int p = first[a];
			while(p != a && p != b) p = x.next[p];
			if(p == b) swap(a, b);
			res.next[a] = res.next[b];
			res.next[b] = 0;
		} else {
			if(res.next[a] == 0 || res.next[b] == 0) {
				if(res.next[a]) swap(a, b);
				res.next[a] = first[b];
			} else {
				int p = res.next[a];
				while(res.next[p]) p = res.next[p];
				res.next[p] = first[a];
				swap(res.next[a], res.next[b]);
			}
		}
		if(res.Calc()) { dst.push_back(res); flag = true; }
	}
	if(!flag) dst.push_back(x);
}
