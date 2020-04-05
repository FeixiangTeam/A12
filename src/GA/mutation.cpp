#include "GA/define.hpp"
const int MAXP=10000000,times_mu=5,pm=2333333;
int pre[35];
void Mutation(Individual x,std::vector<Individual> &dst)
{
	int N=data["target_vertex_set"].size();
	bool flag=0;
	for(int i=1;i<=N;i++)pre[x.next[i]]=i;
	for(int i=1;i<=times_mu;i++){
		Individual temp=x;
		int a=rand()%N+1,b=rand()%N+1;
		if(a!=b&&rand()%MAXP<=pm){
			if(temp.next[a]==b||temp.next[b]==a){
				if(temp.next[b]==a){int t=a;a=b;b=t;}
				temp.next[pre[a]]=b;
				temp.next[a]=temp.next[b];
				temp.next[b]=a;
			}
			else{
				temp.next[pre[a]]=b;temp.next[pre[b]]=a;
				int t=temp.next[a];temp.next[a]=temp.next[b];temp.next[b]=t;
			}
			if(temp.Calc()&&temp.fitness>=x.fitness)dst.push_back(temp),flag=1;
		}
	}
	if(!flag)dst.push_back(x);
	return;
}
