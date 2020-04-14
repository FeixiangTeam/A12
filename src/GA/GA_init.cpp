#include "GA/define.hpp"
#include "data.hpp"
#include "GA/config.hpp"
#include <algorithm>

std::random_device rd;
std::mt19937 rand_engine(rd());

bool Random(double Pr) {
	return rand_engine() <= Pr * rand_engine.max();
}

int total_truck_num;
Individual best;

std::vector<Truck> trucks;

void GAInit() {
	const int tv_num = data["target_vertex_set"].size();

	best.fitness = 0;

	for(const auto &truck: data["truck_set"]) {
		const auto &num = truck["num"];
		if(!num.is_null()) {
			total_truck_num += num.get<int>();
		} else {
			total_truck_num = tv_num;
			break;
		}
	}

	const int truck_type_num = data["truck_set"].size();
	trucks.resize(truck_type_num);
	for(int i = 0; i < truck_type_num; ++i) {
		const auto &truck = data["truck_set"][i];
		const auto &num = truck["num"];
		Truck &t = trucks[i];
		t.id = i;
		if(!num.is_null()) t.num = num.get<int>();
		else t.num = tv_num;
		t.limit = truck["limit"].get<double>();
	}
	std::sort(trucks.begin(), trucks.end());
}

Individual::Individual(): next(data["target_vertex_set"].size() + 1) { }

<<<<<<< HEAD
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
=======
static int degree[MAX_TARGET_NUM + 1];
static double weight[MAX_TARGET_NUM];
static int used[MAX_TARGET_NUM];

bool Individual::Calc() {
	const int tv_num = data["target_vertex_set"].size();
	const int truck_type_num = trucks.size();

	truck_num = 0;
	std::fill(degree + 1, degree + tv_num + 1, 0);
	for(int i = 1; i <= tv_num; ++i) {
		if(next[i] == 0) ++truck_num;
		else ++degree[next[i]];
>>>>>>> cxy004
	}
	if(truck_num > total_truck_num) return false;

	for(int i = 1, j = 0; i <= tv_num; ++i)
		if(degree[i] == 0) {
			weight[j] = 0;
			for(int p = i; p; p = next[p])
				weight[j] += data["target_vertex_set"][p - 1]["target"].get<double>();
			++j;
		}
	std::sort(weight, weight + truck_num);

	std::fill(used, used + truck_type_num, 0);
	for(int i = 0, j = 0; i < truck_num; ++i) {
		while(j < truck_type_num && (weight[i] > trucks[j].limit || used[j] >= trucks[j].num)) ++j;
		if(j >= truck_type_num) return false;
		++used[j];
	}

	double cost = BETA * truck_num;
	for(int i = 1; i <= truck_num; ++i)
		if(degree[i] == 0) cost += Map::dis[0][i];
		else cost += Map::dis[i][next[i]];
	fitness = FITNESS_FACTOR / cost;

	if(fitness > best.fitness) best = *this;
	return true;
}
