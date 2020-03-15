#ifndef DATA_HPP
#define DATA_HPP

#include "json.hpp"
using json=nlohmann::json;
#include <map>
#include <string>
#include <vector>
#include <utility>

const size_t MAX_VERTEX_NUM=50;

extern json data;

void DataInit(const char *input);

extern std::map<std::string, int> vertex;

namespace Map {
	extern double dis[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
	extern int path[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

	void Init();

	void PathPrint(std::vector<int> &res, int x, int y);
}

namespace Answer {
	extern json result;

	void AddPath(std::vector<int> target_path);

	void Output();
}

#endif
