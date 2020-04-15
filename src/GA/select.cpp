#include "GA/define.hpp"
#include <vector>
#include <cstddef>
#include <random>

void Select(std::vector<Individual> &dst, std::size_t num) {
	std::vector<double> pr(dst.size());
	for(std::size_t i = 0; i < dst.size(); ++i)
		pr[i] = dst[i].fitness;
	std::discrete_distribution<> choice(pr.begin(), pr.end());
	std::vector<Individual> res(num);
	for(std::size_t i = 0; i < num; ++i)
		res[i] = dst[choice(rand_engine)];
	dst = std::move(res);
}
