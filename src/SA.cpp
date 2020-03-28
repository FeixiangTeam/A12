#include "define.hpp"
#include <cstring>
#include <algorithm>
#include <cmath>

using namespace std;
const double dec_rate=0.90; //模拟退火系数，即降温率
const int max_rand=100000; //产生的随机数上限
const double eps=1e-4; //浮点数精度
const double T0=100000; //初始温度
const double T_min=1e-4; //临界温度

double T; //实时温度
int siz; //种群大小（个体数目）
vector<Individual> sel_gen; //最终选择的个体组成的种群

bool accept(Individual old_g,Individual new_g,double Temp) //可接受函数，为真则接受新一代种群的个体
{
	double f_new=caculaFitness(new_g),f_old=caculaFitness(old_g); //f_val()为返回适应值的函数
	if(f_new<f_old) return true; //更优，直接接受
	int posb=rd()%max_rand; //随机出一个值
	int val=(int)(1.0/(1.0+exp(-(f_new-f_old)/Temp))*max_rand); //判断是否接受
	return posb<=val;
}

vector<Individual> sel_SA(vector<Individual> &old_gen,vector<Individual> &new_gen) //传入上一代种群、新一代种群
{
	sel_gen.clear(); //初始化答案数组
	siz=old_gen.size(); //获取种群大小
	for(int i=0;i<siz;i++)
	{
		Individual old_g=old_gen[i],new_g=new_gen[i];
		Individual better_indiv=old_g; //先选择上一代种群的个体
		T=T0; //更新实时温度为初始温度
		while(T>T_min) //一直退火，直到实时温度低于临界温度，这里有点迷，感觉论文没有说清楚
		{
			if(accept(old_g,new_g,T)) //判断新的个体是否能被接受
			{
				better_indiv=new_g; //新的个体能被接受，更新
				break;
			}
			T*=dec_rate; //按照设定的退火系数进行降温
		}
		sel_gen.push_back(better_indiv); //选择最终接受的一个个体
	}
	return sel_gen;
}
