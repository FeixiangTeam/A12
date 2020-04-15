#ifndef DATA_HPP
#define DATA_HPP

#include "json.hpp"
using json = nlohmann::json;
#include <cstddef>
#include <vector>
#include <string>

constexpr std::size_t MAX_VERTEX_NUM = 100;
constexpr std::size_t MAX_TARGET_NUM = 50;

extern json data;
extern int tv_num;

void DataInit(const char *input);

namespace Map {
	extern double dis[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

	void Init();
	double CalcPathDistance(const std::vector<int> &path);
	std::vector<std::string> GetNamedPath(const std::vector<int> &path);
	std::vector<int> GetFullPath(const std::vector<int> &target_path);
}

extern json answer;

void AnswerOutput();

#endif
