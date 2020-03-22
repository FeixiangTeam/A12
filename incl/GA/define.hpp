#ifndef DEFINE_HPP
#define DEFINE_HPP

#include "data.hpp"
#include <random>
#include <vector>

extern std::random_device rd;
extern std::mt19937 rand_engine;

bool Random(double pr);

extern int total_truck_num;

void GAInit();

struct Individual {
	int next[MAX_TARGET_NUM+1];
	int truck_num;
	double fitness;
	Individual() =default;
	bool Calc();
};

std::vector<Individual> IndividualInit(size_t num);

void Select(std::vector<Individual> &dst, size_t num);

void Cross(Individual a, Individual b, std::vector<Individual> &dst);

void Mutation(Individual x, std::vector<Individual> &dst);

#endif
