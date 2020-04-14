#include "GA/define.hpp"
#include "GA/config.hpp"
#include <vector>
#include <cstddef>
#include <utility>

static std::vector<Individual> u, v, w;
static int next[MAX_TARGET_NUM+1], degree[MAX_TARGET_NUM+1];
static std::vector<int> tru;

void Solve() {
	GAInit();
	u = IndividualInit(POPULATION);
	for(int k = 0; k < TIMES; ++k) {
		printf("Solve Cycle %d\n", k);
		Select(u, POPULATION);
		v.clear();
		for(std::size_t i = 0; i < u.size(); ++i)
			for(std::size_t j = 0; j < u.size(); ++j)
				if(i != j && Random(PR_CROSS))
					Cross(v, u[i], u[j]);
		w.clear();
		for(Individual &x: v)
			if(Random(PR_MUTATION))
				Mutation(w, std::move(x));
			else
				w.push_back(std::move(x));
		u = std::move(w);
	}

	int tv_num=data["target_vertex_set"].size();
	for(int i=1;i<=tv_num;i++) degree[best.next[i]]++;
	for(int i=1;i<=tv_num;i++)
	{
		if(!degree[i])
		{
			double weight=0;
			int now_tru=i;
			tru.clear();
			tru.push_back(now_tru);
			weight+=data["target_vertex_set"][i-1]["target"].get<double>();
			while(best.next[now_tru])
			{
				tru.push_back(best.next[now_tru]);
				weight+=data["target_vertex_set"][best.next[now_tru]-1]["target"].get<double>();
				now_tru=best.next[now_tru];
			}
			int this_tru_type = best_weight[ best_mp[weight] ].back();
			best_weight[ best_mp[weight] ].pop_back();
			answer.push_back({
				{"full_path", Map::GetNamedPath(Map::GetFullPath(tru))},
				{"target_path", Map::GetNamedPath(tru)},
				{"distance", Map::CalcPathDistance(tru)},
				{"weight", weight},
				{"truck", data["truck_set"][this_tru_type]["name"]},
				{"ratio", weight / data["truck_set"][this_tru_type]["limit"].get<double>()}
			});
		}
	}
}
