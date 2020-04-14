#include "GA/define.hpp"
#include <cstddef>
#include <random>

static std::vector<double> pr;
static std::vector<Individual> res;

void Select(std::vector<Individual> &dst, std::size_t num) {
	pr.resize(dst.size());
	for(std::size_t i = 0; i < dst.size(); ++i)
		pr[i] = dst[i].fitness;
	std::discrete_distribution<> choice(pr.begin(), pr.end());
	res.resize(num);
	for(std::size_t i = 0; i < num; ++i)
		res[i] = dst[choice(rand_engine)];
	dst = std::move(res);
}
