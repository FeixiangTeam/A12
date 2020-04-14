#include "GA/define.hpp"
#include "data.hpp"
#include "GA/config.hpp"
#include <algorithm>

std::random_device rd;
std::mt19937 rand_engine(rd());

bool Random(double Pr) {
	return rand_engine() <= Pr * rand_engine.max();
}

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

int total_truck_num;
Individual best;

static int degree[MAX_TARGET_NUM+1]; //The in-degree at each point (to find out whose value is 1)
static std::vector<int> tru[MAX_TARGET_NUM+1]; // Save each truck
static double req_weight[MAX_TARGET_NUM+1]; // The minimum weight a truck need to require
static Tru truc[MAX_TARGET_NUM+1];

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
	memset(req_weight,0,sizeof(req_weight));
	memset(degree,0,sizeof(degree));
	int tv_num=data["target_vertex_set"].size(); //The number of target points
	int truck_type=data["truck_set"].size(); // The number of truck type
	int t_cnt=0; // Temporary variable
	int task=0; //Original value of fitness
	truck_num=0; //Initializes the number of trucks
	fitness=-1; //Initializes the value of fitness, if Calc() return false, it means fitness is useless and unavailable
	/*To get number and limited weight of trucks of each type*/
	for(const auto &truck: data["truck_set"])
	{
		const auto &num=truck["num"];
		const auto &limit=truck["limit"];
		truc[t_cnt].id=truc[t_cnt].limit=truc[t_cnt].res=0;
		if(num.is_null()) truc[t_cnt].res=0; // It means this truck type has an unlimited quantity
		else truc[t_cnt].res=num.get<int>(); // ...limited quantity
		truc[t_cnt].limit=limit.get<double>();
		t_cnt++;
	}
	/*********************************************************/

	/*Judge whether each point is visited only once*/
	for(int i=1;i<=tv_num;i++)
	{
		if(next[i])
		{
			degree[next[i]]++; //Record the number of visits
			if(degree[next[i]]>1) return false;
		}
	}
	for(int i=1;i<=tv_num;i++)
	{
		if(!degree[i]) truck_num++;
	}
	if(truck_num>total_truck_num) return false; //The number of truck required by the individual exceeds the maximum limit
	/**********************************************/

	/*Calculate each truck's required minimum weight to serve the target*/
	int temp_cnt=0; //Record the number of truck in real time (temporary variable)
	for(int i=1;i<=tv_num;i++)
	{
		if(!degree[i]) //The starting point (the first point arrive from point 0)
		{
			/*!!Bug: the types of the trucks are not defined*/
//			double limit_w=data["truck_set"][0]["limit"];// Get car weight limit
			/*************************/
			temp_cnt++; //Increase the variable
			tru[temp_cnt].clear();
			tru[temp_cnt].push_back(i); //Push the first point to arrive
			req_weight[temp_cnt]+=data["target_vertex_set"][i-1]["target"].get<double>(); //Cumulative required truck weight
			int now_tru=i; //Move truck
			while(next[now_tru]) //Move until the last point
			{
				tru[temp_cnt].push_back(next[now_tru]); //Push next point
				req_weight[temp_cnt]+=data["target_vertex_set"][next[now_tru]-1]["target"].get<double>(); //Cumulative required truck weight
				now_tru=next[now_tru]; //Move to the next point
			}
//			if(temp_w>limit_w) return false; //The weight of truck required by the individual exceeds the maximum limit
		}
	}
	/************************************************************/

	/*Choose the best type for each truck*/
	std::sort(req_weight+1,req_weight+truck_num+1);
	std::sort(truc,truc+truck_type);
	int now_type=0, hash_val=1;
	for(int i=1;i<=truck_num;i++)
	{
		while( (now_type < truck_type) && (req_weight[i] > truc[now_type].limit) ) now_type++;
		if(now_type==truck_type) return false; // Existing trucks cannot meet the requirements
		else // Limited number of trucks
		{
			truc[now_type].res--;
			if(!truc[now_type].res) now_type++; // To avoid to recognize as unlimited number of trucks
		}
	}
	/*************************************/

	for(int i=1;i<=truck_num;i++) task+=Map::CalcPathDistance(tru[i]);
	task+=BETA*truck_num;
	fitness=FITNESS_FACTOR/task;
	if(fitness > best.fitness)
	{
		best = *this; //Update the best individual
	}
	return true; //This individual is acceptable
}
