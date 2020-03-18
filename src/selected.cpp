#include <stdlib.h> 
#include <algorithm>
#include <string.h> 

using namespace std;

int des_num;//���͵���Ŀ
int colony_num; //��ʼ����Ⱥ������Ŀ 
int d[des_num+1][des_num+1];//i��j���ľ���
int gene[colony_num+1][des_num+1];//��������Ⱥ
int bestRooting[colony_num+1];//���·�� 
double xiShu;//Ŀ�꺯�����ڳ�������ϵ��
double bestFitness;//������Ӧֵ 
int bestNum;//���Ÿ������� 
int worstNum;//���������� 
int N;//��������ϵ�� 
int s;//����� 
 
//�ҵ����Ϊ0���� 
int find(int *arr,int count1,int count2,int x){
	bool deter = 1; //�жϱ��� 
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
//������Ӧֵ 	
caculaFitness(){
	//Ŀ�꺯��f(x)=d[i][gene[colony_num+1][des_num+1]]����һ���֣� + d[0][gene[colony_num][des_num]���ڶ����֣� + xiShu*1*k���������֣�;
	double task[colony_num+1] = 0;//Ŀ�꺯�� 
	for(int i = 1; i <= colony_num; i++){
	    for(int j = 1; j <= des_num; j++){
    	    task[i] += d[j][gene[i][j]];
        }
	}//��һ���� 
	for(int i = 1; i <= colony_num; i++){
	    for(int k = 1; k <= des_num; k++){
	    	int a;
		    a = find(gene,des_num,colony_num,k);
		    if(a != 0 ){
		    task[i] += d[0][a];
			}
	    }
	}//�ڶ����� 
	
		for(int j = 1; j <= colony_num; j++){
		    for(int i = 0; i <= des_num; i++){
			    if(gene[j][i] == 0){
	                task[j] += xiShu; 
		        }
	        }
	    }//�������� 
	
	}
	//������Ӧֵ���ʲ������о�Ӣ���� 
caculaReFitness(){
		double fitness[colony_num+1];//��Ӧֵ
		double sum = 0;//��Ӧֵ�ܺ� 
		double gaiLv[colony_num+1]; //��Ӧ���� 
		int best = 1; //�����Ӧֵ����Ⱥ 
		int worst = 1;//�����Ӧֵ����Ⱥ 
		for(int j = 1; j <= colony_num; j++){
			fitness[j] = N / task[j];
			if(fitness[j] > fitness[best]){
			    best = j; 
			}
			if(fitness[j] < fitness[worst]){
			worst = j; 
		}
		copy(bestRooting,gene[best]);//ѡ��������Ӧֵ�������忽��������·�� 
		bestFitness = fitness[best];//���������Ӧֵ 
		bestNum = best; //������Ÿ�������
		worstNum = worst;//������������� 
		 
		 for(int j = 1; j <= colony_num; j++){
		 	sum += fitness[j];
		 } 
		 for(int j = 1; j <= colony_num; j++){
		 	gaiLv[j] = fitness[j]/sum;
		 }
	}
	//�����ۼӸ������
caculaSumFitness(){
        double sumFitness[colony_num+1]; // �ۼ�ֵ 
		for(int j = 1; j <= colony_num; j++){
			for(int m = 1; m <= j; m++){
			sumFitness[j] += gaiLv[m]; 
			}
		}
	} 
	//��Ⱥѡ�� 
select(){
		int tempColony[colony_num+1][des_num+1];//��ʱ��Ⱥ 
		memcpy(tempColony[1],gene[bestNum],sizeof(tempColony[1]));//����һ������ֵ��������ʱ��Ⱥ��
		double ran;//����� 
		for(int t = 2; t <= colony_num; t++){
			if(t == worstNum){
			    continue;
			} //�����ĸ���ȥ�� 
			else{
			ran = rand() % RAND_MAX + 1;
			s = (double) / 100.0;//���������
			for(int i = 1; i <= colony_num; i++){
			    	if(sumFitness[i] <= s){
			    		memcpy(tempColony[t],gene[1],sizeof(tempColony[t]));
			    		break;
					}//��������������Ӧֵ�򽫵�һ�����屣������ʱ��Ⱥ�� 
					else{
					    for(int j = 2; j  <= colony_num; j++){
						    if(s <= sumFitness[j] && s > sumFitness[j-1]){
								memcpy(tempColony[t],gene[j],sizeof(tempColony[t]));
								break;
					        }//���������λ����������Ӧֵ֮��ʱ����j�����屣������ʱ��Ⱥ��    
					    }
				    }
				}
			}
		}
		for(int i = 1; i <= colony_num; i++){
			memcpy(gene[i],tempColony[i],sizeof(gene[i]));//����ʱ��Ⱥ���븴�Ƶ���Ⱥ�� 
		}
    }
	int main(){
		return 0;
	}	  

