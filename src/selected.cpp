#include <stdlib.h> 
#include <algorithm>
#include <string.h> 

using namespace std;

int des_num;//配送点数目
int colony_num; //初始化钟群个体数目 
int d[des_num+1][des_num+1];//i到j处的距离
int gene[colony_num+1][des_num+1];//编码后的钟群
int bestRooting[colony_num+1];//最佳路线 
double xiShu;//目标函数对于车辆数的系数
double bestFitness;//最优适应值 
int bestNum;//最优个体的序号 
int worstNum;//最差个体的序号 
int N;//函数修正系数 
int s;//随机数 
 
//找到入度为0的数 
int find(int *arr,int count1,int count2,int x){
	bool deter = 1; //判断变量 
 		for(int j = 1; j <= count2; j++){
		    for(int i = 1; i <= count1; i++){
 		        if(arr[j][i] == x){
 		        	deter = 0;
				}
		    }
		    while(deter){
		    	return x;
			}
			while(!deter){
				return 0;
			}
	    }
    } 
//计算适应值 	
caculaFitness(){
	//目标函数f(x)=d[i][gene[colony_num+1][des_num+1]]（第一部分） + d[0][gene[colony_num][des_num]（第二部分） + xiShu*1*k（第三部分）;
	double task[colony_num+1] = 0;//目标函数 
	for(int i = 1; i <= colony_num; i++){
	    for(int j = 1; j <= des_num; j++){
    	    task[i] += d[j][gene[i][j]];
        }
	}//第一部分 
	for(int i = 1; i <= colony_num; i++){
	    for(int k = 1; k <= des_num; k++){
	    	int a;
		    a = find(gene,des_num,colony_num,k);
		    if(a != 0 ){
		    task[i] += d[0][a];
			}
	    }
	}//第二部分 
	
		for(int j = 1; j <= colony_num; j++){
		    for(int i = 0; i <= des_num; i++){
			    if(gene[j][i] == 0){
	                task[j] += xiShu; 
		        }
	        }
	    }//第三部分 
	
	}
	//计算适应值概率并但进行精英保留 
caculaReFitness(){
		double fitness[colony_num+1];//适应值
		double sum = 0;//适应值总和 
		double gaiLv[colony_num+1]; //适应概率 
		int best = 1; //最大适应值的钟群 
		int worst = 1;//最大适应值的钟群 
		for(int j = 1; j <= colony_num; j++){
			fitness[j] = N / task[j];
			if(fitness[j] > fitness[best]){
			    best = j; 
			}
			if(fitness[j] < fitness[worst]){
			worst = j; 
		}
		copy(bestRooting,gene[best]);//选出最大的适应值并将个体拷贝给最优路径 
		bestFitness = fitness[best];//算出最优适应值 
		bestNum = best; //算出最优个体的序号
		worstNum = worst;//算出最差个体的序号 
		 
		 for(int j = 1; j <= colony_num; j++){
		 	sum += fitness[j];
		 } 
		 for(int j = 1; j <= colony_num; j++){
		 	gaiLv[j] = fitness[j]/sum;
		 }
	}
	//计算累加个体概率
caculaSumFitness(){
        double sumFitness[colony_num+1]; // 累加值 
		for(int j = 1; j <= colony_num; j++){
			for(int m = 1; m <= j; m++){
			sumFitness[j] += gaiLv[m]; 
			}
		}
	} 
	//钟群选择 
select(){
		int tempColony[colony_num+1][des_num+1];//临时种群 
		memcpy(tempColony[1],gene[bestNum],sizeof(tempColony[1]));//将上一代最优值保留到临时种群中
		double ran;//随机数 
		for(int t = 2; t <= colony_num; t++){
			if(t == worstNum){
			    continue;
			} //将最差的个体去掉 
			else{
			ran = rand() % RAND_MAX + 1;
			s = (double) / 100.0;//产生随机数
			for(int i = 1; i <= colony_num; i++){
			    	if(sumFitness[i] <= s){
			    		memcpy(tempColony[t],gene[1],sizeof(tempColony[t]));
			    		break;
					}//如果随机数大于适应值则将第一个个体保留在临时种群中 
					else{
					    for(int j = 2; j  <= colony_num; j++){
						    if(s <= sumFitness[j] && s > sumFitness[j-1]){
								memcpy(tempColony[t],gene[j],sizeof(tempColony[t]));
								break;
					        }//否则当随机数位于两个体适应值之间时将第j个个体保留在临时种群中    
					    }
				    }
				}
			}
		}
		for(int i = 1; i <= colony_num; i++){
			memcpy(gene[i],tempColony[i],sizeof(gene[i]));//将临时种群编码复制到种群中 
		}
    }
	int main(){
		return 0;
	}	  

