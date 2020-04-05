#include "define.hpp"
#include "data.hpp"
#include <stdlib.h> 
#include <algorithm>
#include <string.h> 

using namespace std;

static int tv_num = data["target_vertex_set"].size();//���͵���Ŀ
static int s;//����� 
int bestNum;
int worstNum;
double sum;
 
//��Ⱥѡ�� 
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
	    int tempColony[num + 1][tv_num + 1];//��ʱ��Ⱥ
		memcpy(tempColony[0],dst[bestNum].next,sizeof(tempColony[0]));//����һ������ֵ��������ʱ��Ⱥ��
		for(int t = 1; t < num; t++){
			if(t == worstNum){
			    continue;
			} //�����ĸ���ȥ�� 
			else{
			static std::uniform_real_distribution<> random_values(0.0, 1.0);
			s = random_values(rand_engine);
			for(int i = 0; i < num; i++){
			    	if(dst[i].sumFitness <= s){
			    		memcpy(tempColony[t],dst[0],sizeof(tempColony[t]));
			    		break;
					}//��������������Ӧֵ�򽫵�һ�����屣������ʱ��Ⱥ�� 
					else{
					    if(i >= 1){
						    if(s <= dst[i].sumFitness && s > dst[i].sumFitness){
								memcpy(tempColony[t],dst[i],sizeof(tempColony[t]));
								break;
					        }//���������λ����������Ӧֵ֮��ʱ����j�����屣������ʱ��Ⱥ��    
					    }
				    }
				}
			}
		}
		for(int i = 0; i < num; i++){
			memcpy(dst[i].next,tempColony[i],sizeof(dst[i]));//����ʱ��Ⱥ���븴�Ƶ���Ⱥ�� 
		}
    }
