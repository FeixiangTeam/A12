#include "define.hpp"
#include "data.hpp"
#include <stdlib.h> 
#include <algorithm>
#include <string.h> 

using namespace std;

static int tv_num = data["target_vertex_set"].size();//配送点数目
static int s;//随机数 
int bestNum;
int worstNum;
double sum;
 
//钟群选择 
void Select(std::vector<Individual>& dst, size_t num){
	bestNum = 0;
	worstNum = 0;
	sum = 0;
	for (int i = 0; i < num; i++) {
		if (dst[i].fitness > dst[bestNum]) {
			bestNum = i;
		}
		if (dst[i].fitness > dst[worstNum) {
			worstNum = i;
		}
	}
	for (int i = 0; i < num; i++) {
		sum += dst[i].fitness;
	}
	for (int i = 0; i < num; i++) {
		dst[i].gaiLv = dst[i].fitness / sum;
	}
	for (int i = 0; i < num; i++) {
		for (int j = 0; j <= i; j++) {
			dst[i].sumFitness += dst[j].gaiLv;
		}
	}
	    int tempColony[num + 1][tv_num + 1];//临时种群
		memcpy(tempColony[0],dst[bestNum].next,sizeof(tempColony[0]));//将上一代最优值保留到临时种群中
		for(int t = 1; t < num; t++){
			if(t == worstNum){
			    continue;
			} //将最差的个体去掉 
			else{
			static std::uniform_real_distribution<> random_values(0.0, 1.0);
			s = random_values(rand_engine);
			for(int i = 0; i < num; i++){
			    	if(dst[i].sumFitness <= s){
			    		memcpy(tempColony[t],dst[0],sizeof(tempColony[t]));
			    		break;
					}//如果随机数大于适应值则将第一个个体保留在临时种群中 
					else{
					    if(i >= 1){
						    if(s <= dst[i].sumFitness && s > dst[i].sumFitness){
								memcpy(tempColony[t],dst[i],sizeof(tempColony[t]));
								break;
					        }//否则当随机数位于两个体适应值之间时将第j个个体保留在临时种群中    
					    }
				    }
				}
			}
		}
		for(int i = 0; i < num; i++){
			memcpy(dst[i].next,tempColony[i],sizeof(dst[i]));//将临时种群编码复制到种群中 
		}
    }
