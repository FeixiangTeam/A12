#include "GA/define.hpp"
#include "data.hpp"
#include "GA/config.hpp"

std::random_device rd;
std::mt19937 rand_engine(rd());

int total_truck_num;
Individual best;

void GAInit() {
	best.fitness = 0;
	for(const auto &truck: data["truck_set"]) {
		const auto &num = truck["num"];
		if(!num.is_null()) {
			total_truck_num += num.get<int>();
		} else {
			total_truck_num = data["target_vertex_set"].size();
			break;
		}
	}
}

Individual::Individual(): next(data["target_vertex_set"].size() + 1) { }

bool Individual::Calc()
{
	int tv_num=data["target_vertex_set"].size();
	double limit_w=data["truck_set"][0]["limit"];
	bool vis[MAX_TARGET_NUM+1];
	int tru_num=0;
	int task=0;
	int degree[MAX_TARGET_NUM+1];
	int dist[MAX_TARGET_NUM+1];
	std::vector<int> tru[MAX_TARGET_NUM+1];
	memset(degree,0,sizeof(degree));
	memset(dist,0,sizeof(dist));
	memset(vis,false,sizeof(vis));
	fitness=-1;
	for(int i=1;i<=tv_num;i++)
	{
		if(!next[i])
		{
			tru_num++;
			if(vis[i]) return false;
			vis[i]=true;
		}
		else
		{
			if(vis[next[i]]) return false;
			vis[next[i]]=true;
			degree[next[i]]++;
		}
	}
	if(tru_num>total_truck_num) return false;

	int temp_cnt=0;
	for(int i=1;i<=tv_num;i++)
	{
		if(!degree[i])
		{
			double temp_w=0;
			temp_cnt++;
			tru[temp_cnt].push_back(0);
			tru[temp_cnt].push_back(i);
			temp_w+=data["target_vertex_set"][i-1]["target"].get<double>();
			int now_tru=i;
			while(next[now_tru])
			{
				tru[temp_cnt].push_back(next[now_tru]);
				temp_w+=data["target_vertex_set"][next[now_tru]-1]["target"].get<double>();
				now_tru=next[now_tru];
			}
			tru[temp_cnt].push_back(0);
			if(temp_w>limit_w) return false;
		}
	}

	for(int i=1;i<=tru_num;i++)
	{
		int tr_size=tru[i].size();
		for(int j=0;j<tr_size-1;j++)
		{
			task+=Map::dis[tru[i][j]][tru[i][j+1]];
		}
	}
	task+=BETA*tru_num;
	fitness=FITNESS_FACTOR/task;
	if(fitness > best.fitness)
		best = *this;
	return true;
}
