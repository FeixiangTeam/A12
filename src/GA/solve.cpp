#include "GA/define.hpp"
#include "GA/config.hpp"
#include <vector>
#include <cstddef>
#include <utility>

static std::vector<Individual> u, v, w;

void Solve() {
	GAInit();
	u = IndividualInit(POPULATION);
	for(int k = 0; k < TIMES; ++k) {
		Select(u, POPULATION);
		v.clear();
		for(std::size_t i = 0; i < u.size(); ++i)
			for(std::size_t j = 0; j < u.size(); ++j)
				if(i != j && Random(PR_CROSS))
					Cross(v, u[i], u[j]);
		w.clear();
		for(Individual &x: v)
			if(Random(PR_MUTATION))
				Mutation(w, std::move(x));
			else
				w.push_back(std::move(x));
		u = std::move(w);
	}
	
	answer.push_back({
			{"full_path", Map::GetNamedPath(Map::GetFullPath(res))},
			{"target_path", Map::GetNamedPath(res)},
			{"distance", Map::CalcPathDistance(res)},
			{"weight", w},
			{"truck", data["truck_set"][t]["name"]},
			{"ratio", w / data["truck_set"][t]["limit"].get<double>()}
	});
}
