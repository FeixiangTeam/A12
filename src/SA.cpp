#include "GA/define.hpp"
#include <algorithm>
#include <cmath>

const double dec_rate=0.90; // the rate of temperature reduction(per operation)
const int max_rand=100000; // the upper bound of the generated random Numbers
const double eps=1e-4; // floating point precision
const double T0=100000; // initial temperature
const double T_min=1e-4; // critical temperature

double T; // real-time temperature
int siz; // population size (number of individuals)
std::vector<Individual> sel_gen; // the population of ultimately selected individuals

bool accept(Individual old_g,Individual new_g,double Temp) // the function will return true if accept the new generation of individual
{
	double f_new=new_g.fitness,f_old=old_g.fitness; // get fitness
	if(f_new<f_old) return true; //better, directly accept
	int posb=rand()%max_rand; // pick a random number
	int val=(int)(1.0/(1.0+exp(-(f_new-f_old)/Temp))*max_rand); // judge if it's acceptable
	return posb<=val;
}

std::vector<Individual> sel_SA(std::vector<Individual> &old_gen,std::vector<Individual> &new_gen) // push previous and new generation population
{
	sel_gen.clear(); // initialize the answer array
	siz=old_gen.size(); // get population size
	for(int i=0;i<siz;i++)
	{
		Individual old_g=old_gen[i],new_g=new_gen[i];
		old_g.Calc();new_g.Calc(); // calculate fitness
		Individual better_indiv=old_g; // initialize with the individuals from the previous generation
		T=T0; // update the real-time temperature to the initial temperature
		while(T>T_min) // loop until the real-time temperature is below the critical temperature (!!have some question here!!)
		{
			if(accept(old_g,new_g,T)) // judge if it's acceptable
			{
				better_indiv=new_g; // the new individual is acceptavble, update
				break;
			}
			T*=dec_rate; // use the reduction rate to decrease temperature
		}
		sel_gen.push_back(better_indiv); // select the ultimately picked individual
	}
	return sel_gen; //return the answer array (ultimately picked individual)
}
