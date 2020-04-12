#include "GA/define.hpp"
#include "data.hpp"
#include "GA/config.hpp"

std::random_device rd;
std::mt19937 rand_engine(rd());

int total_truck_num;
Individual best;
static bool vis[MAX_TARGET_NUM+1];
static int degree[MAX_TARGET_NUM+1]; //The in-degree at each point (to find out whose value is 1)
static std::vector<int> tru[MAX_TARGET_NUM+1]; //Save each truck

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
	int tv_num=data["target_vertex_set"].size(); //The number of target points
	truck_num=0; //Initializes the number of trucks
	int task=0; //Original value of fitness
	memset(degree,0,sizeof(degree)); //Initializes the "degree" array
	memset(vis,false,sizeof(vis)); //Initializes the "vis" array
	fitness=-1; //Initializes the value of fitness, if Calc() return false, it means fitness is useless and unavailable
	/*Judge whether each point is visited only once*/
	for(int i=1;i<=tv_num;i++)
	{
		if(!next[i]) //next[i]==0, it means back to the starting point
		{
			tru_num++; //Calculate the number of trucks
			if(vis[i]) return false; //It means this point has been visited more than once
			vis[i]=true; //Mark that this point has been visited
		}
		else
		{
			if(vis[next[i]]) return false; //It means next point has been visited more than once
			vis[next[i]]=true; //Mark that the next point has been visited
			degree[next[i]]++; //Record the number of visits
		}
	}
	if(tru_num>total_truck_num) return false; //The number of truck required by the individual exceeds the maximum limit
	/**********************************************/
	
	/*Judge whether each truck is overweight to serve the target*/
	int temp_cnt=0; //Record the number of truck in real time (provide a variable)
	for(int i=1;i<=tv_num;i++)
	{
		if(!degree[i]) //The starting point (the first point arrive from point 0)
		{
			/*!!Bug: the types of the trucks are not defined*/
			double limit_w=data["truck_set"][0]["limit"];// Get car weight limit
			/*************************/
			double temp_w=0; //Record the current weight required
			temp_cnt++; //Increase the variable
			tru[temp_cnt].push_back(i); //Push the first point to arrive
			temp_w+=data["target_vertex_set"][i-1]["target"].get<double>(); //Cumulative required truck weight
			int now_tru=i; //Move truck
			while(next[now_tru]) //Move until the last point
			{
				tru[temp_cnt].push_back(next[now_tru]); //Push next point
				temp_w+=data["target_vertex_set"][next[now_tru]-1]["target"].get<double>(); //Cumulative required truck weight
				now_tru=next[now_tru]; //Move to the next point
			}
			if(temp_w>limit_w) return false; //The weight of truck required by the individual exceeds the maximum limit
		}
	}
	/************************************************************/
	
	for(int i=1;i<=tru_num;i++) task+=Map::CalcPathDistance(tru[i]);
	task+=BETA*tru_num;
	fitness=FITNESS_FACTOR/task;
	if(fitness > best.fitness)
		best = *this; //Update the best individual
	return true; //This individual is acceptable
}
