#include "GA/define.hpp"
#include "data.hpp"
#include <algorithm>

static int idx[MAX_TARGET_NUM+1];
static double dis[MAX_TARGET_NUM+1];
static int pre[MAX_TARGET_NUM+1];

std::vector<Individual> IndividualInit(size_t num) {
	std::vector<Individual> res(num);
	static int tv_num=data["target_vertex_set"].size();

	static std::uniform_int_distribution<int> truck_num_gen(std::max(total_truck_num-3, 1), total_truck_num);
	for(int i=1; i<=tv_num; ++i) idx[i]=i;

	for(size_t i=0; i<num; ++i) {
		Individual &now=res[i];
		do {
			std::shuffle(idx+1, idx+tv_num+1, rand_engine);
			const int &truck_num=now.truck_num=truck_num_gen(rand_engine);

			for(int j=truck_num+1; j<=tv_num; ++j) {
				const int &p=idx[j];
				dis[j]=Map::dis[pre[p]=idx[1]][p];
				for(int k=2; k<=truck_num; ++k)
					if(Map::dis[idx[k]][p]<dis[j])
						dis[j]=Map::dis[pre[p]=idx[k]][p];
			}

			for(int j=tv_num; j>truck_num; --j) {
				int d=std::min_element(dis+truck_num+1, dis+j+1)-dis;
				std::swap(idx[d], idx[j]);
				dis[d]=dis[j];
				const int &p=idx[j];
				now.next[pre[p]]=p;
				for(int k=truck_num+1; k<j; ++k) {
					const int &q=idx[k];
					if(Map::dis[p][q]<dis[k] || pre[q]==pre[p])
						dis[k]=Map::dis[pre[q]=p][q];
				}
			}
		} while(!now.Calc());
	}

	return res;
}
