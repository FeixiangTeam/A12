#include "GA/define.hpp"
#include "GA/config.hpp"
#include <vector>
#include <cstddef>
#include <utility>

static std::vector<Individual> u, v, w;

void Answer();

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
	Answer();
}

static int degree[MAX_TARGET_NUM + 1];
struct Task {
	int begin;
	double weight;
	friend bool operator<(const Task &x, const Task &y) {
		return x.weight < y.weight;
	}
};

void Answer() {
	const int truck_type_num = trucks.size();

	const auto &next = best.next;
	std::fill(degree + 1, degree + tv_num + 1, 0);
	for(int i = 1; i <= tv_num; ++i) ++degree[next[i]];

	std::vector<Task> tasks(best.truck_num);
	for(int i = 1, j = 0; i <= tv_num; ++i)
		if(degree[i] == 0) {
			tasks[j].begin = i;
			double weight = 0;
			for(int p = i; p; p = next[p]) weight += target[p];
			tasks[j].weight = weight;
			++j;
		}
	std::sort(tasks.begin(), tasks.end());

	const auto &truck_set = data["truck_set"];
	int used = 0;
	for(int i = 0, j = 0; i < best.truck_num; ++i) {
		if(tasks[i].weight > trucks[j].limit || used >= trucks[j].num) {
			++j; used = 0;
			while(j < truck_type_num && tasks[i].weight > trucks[j].limit) ++j;
		}
		++used;

		std::vector<int> path;
		for(int p = tasks[i].begin; p; p = next[p]) path.push_back(p);
		answer.push_back({
			{"full_path", Map::GetNamedPath(Map::GetFullPath(path))},
			{"target_path", Map::GetNamedPath(path)},
			{"distance", Map::CalcPathDistance(path)},
			{"weight", tasks[i].weight},
			{"truck", truck_set[trucks[j].id]["name"]},
			{"ratio", tasks[i].weight / trucks[j].limit}
		});
	}
}
