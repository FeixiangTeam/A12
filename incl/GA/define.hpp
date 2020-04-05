#ifndef DEFINE_HPP
#define DEFINE_HPP

#include "data.hpp"
#include <random>
#include <vector>

extern std::random_device rd;
extern std::mt19937 rand_engine;

bool Random(double pr);

extern int total_truck_num;

void GAInit();

struct Individual {
	int next[MAX_TARGET_NUM+1];
	int truck_num;
	double fitness;
	double gaiLv;
	double sumFitness;
	Individual();
	bool Calc()
	{
		int tv_num=data["target_vertex"].size();
		int limit_w=data["truck"][0]["limit"].asInt();
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
		if(tru_num>tot_truck_num) return false;

		int temp_cnt=0;
		for(int i=1;i<=tv_num;i++)
		{
			if(!degree[i])
			{
				int temp_w=0;
				temp_cnt++;
				tru[temp_cnt].push_back(0);
				tru[temp_cnt].push_back(i);
				temp_w+=data["target_vertex"][i-1]["target"].asInt();
				int now_tru=i;
				while(next[now_tru])
				{
					tru[temp_cnt].push_back(next[now_tru]);
					temp_w+=data["target_vertex"][next[now_tru]-1]["target"].asInt();
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
				task+=map::dis[tru[i][j]][tru[i][j+1]];
			}
		}
		task+=XiShu*tru_num;
		fitness=N/task;
		return true;
	}
};

std::vector<Individual> IndividualInit(size_t num);

void Select(std::vector<Individual> &dst, size_t num);

void Cross(const Individual &a, const Individual &b, std::vector<Individual> &dst);

void Mutation(Individual x, std::vector<Individual> &dst);

#endif
