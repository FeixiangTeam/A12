#ifndef DEFINE_HPP
#define DEFINE_HPP

#include "data.hpp"
#include <cstddef>
#include <random>
#include <vector>

extern std::random_device rd;
extern std::mt19937 rand_engine;

bool Random(double pr);

extern int total_truck_num;

void GAInit();

struct Individual {
	std::vector<int> next;
	double fitness;
	int truck_num;
	Individual();
	bool Calc();
};

extern Individual best;

std::vector<Individual> IndividualInit(std::size_t num);

void Select(std::vector<Individual> &dst, std::size_t num);

void Cross(std::vector<Individual> &dst, const Individual &a, const Individual &b);

void Mutation(std::vector<Individual> &dst, Individual x);

#endif
