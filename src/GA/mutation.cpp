#include "GA/define.hpp"
#include <algorithm>
const int MAXP=10000000,times_mu=5,pm=2333333;
int pre[35],car[35],in[35];
void Mutation(std::vector<Individual> &dst, Individual x)
{
	using std::swap;
	int N=data["target_vertex_set"].size();
	bool flag=0;
	for(int i=1;i<=N;i++)in[x.next[i]]++;
	for(int i=1;i<=N;i++)if(!in[i]){
		int temp=i;
		while(x.next[temp])car[temp]=i,temp=x.next[temp];
		car[temp]=i;
	}
	for(int i=1;i<=times_mu;i++){
		Individual temp=x;
		int a=rand()%N+1,b=rand()%N+1;
		if(a!=b&&rand()%MAXP<=pm){
			if(car[a]==car[b]){
				int j=car[a];
				while(j!=a&&j!=b)j=x.next[j];
				if(j==b)swap(a,b);
				temp.next[a]=temp.next[b];
				temp.next[b]=0;temp.truck_num++;
			}
			else
			{
				if(!temp.next[a]||!temp.next[b])
				{
					if(temp.next[a])swap(a,b);
					temp.next[a]=car[b];
				}
				else
				{
					int j=temp.next[a];
					while(temp.next[j])j=temp.next[j];
					temp.next[j]=car[a];
					int t=temp.next[a];
					temp.next[a]=temp.next[b];
					temp.next[b]=t;
				}
				temp.truck_num--;
			}
			if(temp.Calc()&&temp.fitness>=x.fitness)dst.push_back(temp),flag=1;
		}
	}
	if(!flag)dst.push_back(x);
	return;
}
