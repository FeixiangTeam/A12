#include "define.hpp"
#include <cstring>
#include <algorithm>
#include <cmath>

using namespace std;
const double dec_rate=0.90; //ģ���˻�ϵ������������
const int max_rand=100000; //���������������
const double eps=1e-4; //����������
const double T0=100000; //��ʼ�¶�
const double T_min=1e-4; //�ٽ��¶�

double T; //ʵʱ�¶�
int siz; //��Ⱥ��С��������Ŀ��
vector<Individual> sel_gen; //����ѡ��ĸ�����ɵ���Ⱥ

bool accept(Individual old_g,Individual new_g,double Temp) //�ɽ��ܺ�����Ϊ���������һ����Ⱥ�ĸ���
{
	double f_new=caculaFitness(new_g),f_old=caculaFitness(old_g); //f_val()Ϊ������Ӧֵ�ĺ���
	if(f_new<f_old) return true; //���ţ�ֱ�ӽ���
	int posb=rd()%max_rand; //�����һ��ֵ
	int val=(int)(1.0/(1.0+exp(-(f_new-f_old)/Temp))*max_rand); //�ж��Ƿ����
	return posb<=val;
}

vector<Individual> sel_SA(vector<Individual> &old_gen,vector<Individual> &new_gen) //������һ����Ⱥ����һ����Ⱥ
{
	sel_gen.clear(); //��ʼ��������
	siz=old_gen.size(); //��ȡ��Ⱥ��С
	for(int i=0;i<siz;i++)
	{
		Individual old_g=old_gen[i],new_g=new_gen[i];
		Individual better_indiv=old_g; //��ѡ����һ����Ⱥ�ĸ���
		T=T0; //����ʵʱ�¶�Ϊ��ʼ�¶�
		while(T>T_min) //һֱ�˻�ֱ��ʵʱ�¶ȵ����ٽ��¶ȣ������е��ԣ��о�����û��˵���
		{
			if(accept(old_g,new_g,T)) //�ж��µĸ����Ƿ��ܱ�����
			{
				better_indiv=new_g; //�µĸ����ܱ����ܣ�����
				break;
			}
			T*=dec_rate; //�����趨���˻�ϵ�����н���
		}
		sel_gen.push_back(better_indiv); //ѡ�����ս��ܵ�һ������
	}
	return sel_gen;
}
