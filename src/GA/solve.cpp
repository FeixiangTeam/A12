#include "GA/config.hpp"
#include "GA/define.hpp"
#include <vector>
#include <utility>

std::vector<Individual> u, v, w;

void Solve() {
	GAInit();
	u=IndividualInit(POPULATION);
	for(int k=0; k<TIMES; ++k) {
		Select(u, POPULATION);
		v.clear();
		for(int i=0; i<u.size(); ++i)
			for(int j=0; j<u.size(); ++j)
				if(i!=j && Random(PR_CROSS))
					Cross(u[i], u[j], v);
		w.clear();
		for(Individual &&x: v)
			if(Random(PR_MUTATION))
				Mutation(x, w);
			else
				w.push_back(x);
		u=std::move(w);
	}
}
