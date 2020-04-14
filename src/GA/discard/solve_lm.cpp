#include "GA/define.hpp"
#include "GA/config.hpp"
#include <vector>
#include <cstddef>
#include <utility>

struct Tru
{
	int id; // The type
	int res; // The number of trucks remaining
	double limit; // The limit weight
	bool operator < (const Tru &tr) const
	{
		return limit<tr.limit; // Sort by limit (greedy strategy)
	}
};
struct Route
{
	std::vector<int> tru;
	double weight;
	bool operator < (const Route &rou) const
	{
		return weight<rou.weight;
	}
};

static std::vector<Individual> u, v, w;
static int next[MAX_TARGET_NUM+1], degree[MAX_TARGET_NUM+1];
static Tru truc[MAX_TARGET_NUM+1];
static Route route[MAX_TARGET_NUM+1];


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
	
	// answer
	int t_cnt=0; // Temporary variable
	int tv_num=data["target_vertex_set"].size();
	int truck_type=data["truck_set"].size(); // The number of truck type
	for(const auto &truck: data["truck_set"])
	{
		const auto &num=truck["num"];
		const auto &limit=truck["limit"];
		if(num.is_null()) truc[t_cnt].res=0; // It means this truck type has an unlimited quantity
		else truc[t_cnt].res=num.get<int>(); // ...limited quantity
		truc[t_cnt].limit=limit.get<double>();
		t_cnt++;
	}
	for(int i=1;i<=tv_num;i++) degree[best.next[i]]++;
	t_cnt=0;
	for(int i=1;i<=tv_num;i++)
	{
		if(!degree[i])
		{
			route[++t_cnt].weight=0;
			int now_tru=i;
			route[t_cnt].tru.clear();
			route[t_cnt].tru.push_back(now_tru);
			route[t_cnt].weight+=data["target_vertex_set"][i-1]["target"].get<double>();
			while(best.next[now_tru])
			{
				route[t_cnt].tru.push_back(best.next[now_tru]);
				route[t_cnt].weight+=data["target_vertex_set"][best.next[now_tru]-1]["target"].get<double>();
				now_tru=best.next[now_tru];
			}
		}
	}
	std::sort(truc,truc+truck_type);
	std::sort(route+1,route+best.truck_num+1);
	int now_type=0;
	for(int i=1;i<=best.truck_num;i++)
	{
		while( (now_type < truck_type) && (route[i].weight > truc[now_type].limit) ) now_type++;
		int this_tru_type = now_type;
		if(truc[now_type].res!=0) // Limited number of trucks
		{
			truc[now_type].res--;
			if(!truc[now_type].res) now_type++; // To avoid to recognize as unlimited number of trucks
		}
		answer.push_back({
			{"full_path", Map::GetNamedPath(Map::GetFullPath(route[i].tru))},
			{"target_path", Map::GetNamedPath(route[i].tru)},
			{"distance", Map::CalcPathDistance(route[i].tru)},
			{"weight", route[i].weight},
			{"truck", data["truck_set"][this_tru_type]["name"]},
			{"ratio", route[i].weight / data["truck_set"][this_tru_type]["limit"].get<double>()}
		});
	}
}
