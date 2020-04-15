#include "data.hpp"
#include <map>
#include <string>
#include <fstream>
#include <cstddef>
#include <limits>
#include <utility>
#include <iostream>

json data;
int tv_num;

static std::map<std::string, int> vertex;
static std::string name[MAX_VERTEX_NUM];

void DataInit(const char *input) {
	std::ifstream fin(input);
	fin >> data;

	const auto &target_vertex_set = data["target_vertex_set"];
	tv_num = target_vertex_set.size();

	int num = 0;
	const auto &start_vertex = data["start_vertex"];
	if(vertex.emplace(start_vertex, num).second)
		name[num++] = start_vertex;

	for(int i = 0; i < tv_num; ++i) {
		const auto &tg = target_vertex_set[i]["name"];
		if(vertex.emplace(tg, num).second) name[num++] = tg;
	}

	for(const auto &edge: data["edge_set"]) {
		const auto &from = edge["from"];
		const auto &to = edge["to"];
		if(vertex.emplace(from, num).second) name[num++] = from;
		if(vertex.emplace(to, num).second) name[num++] = to;
	}

	Map::Init();
}

namespace Map {
	double dis[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
	static int path[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

	void Init() {
		std::fill(*dis, *dis + sizeof(dis) / sizeof(double), std::numeric_limits<double>::infinity());
		std::fill(*path, *path + sizeof(path) / sizeof(int), -1);
		for(const auto &edge: data["edge_set"]) {
			int x = vertex[edge["from"]], y = vertex[edge["to"]];
			dis[x][y] = edge["dis"];
		}
		for(std::size_t k = 0; k < vertex.size(); ++k)
			for(std::size_t i = 0; i < vertex.size(); ++i)
				for(std::size_t j = 0; j < vertex.size(); ++j)
					if(dis[i][k] + dis[k][j] < dis[i][j]) {
						dis[i][j] = dis[i][k] + dis[k][j];
						path[i][j] = k;
					}
	}

	double CalcPathDistance(const std::vector<int> &path) {
		double res = 0;
		res += dis[0][path.front()];
		for(std::size_t i = 1; i < path.size(); ++i)
			res += dis[path[i - 1]][path[i]];
		res += dis[path.back()][0];
		return res;
	}

	std::vector<std::string> GetNamedPath(const std::vector<int> &path) {
		std::vector<std::string> res;
		for(int x: path)
			res.push_back(name[x]);
		return res;
	}

	static void PathPrint(std::vector<int> &res, int x, int y) {
		if(path[x][y] == -1)
			res.push_back(x);
		else {
			PathPrint(res, x, path[x][y]);
			PathPrint(res, path[x][y], y);
		}
	}

	std::vector<int> GetFullPath(const std::vector<int> &target_path) {
		std::vector<int> path;
		Map::PathPrint(path, 0, target_path.front());
		for(std::size_t i = 1; i < target_path.size(); ++i)
			Map::PathPrint(path, target_path[i - 1], target_path[i]);
		Map::PathPrint(path, target_path.back(), 0);
		path.push_back(0);
		return path;
	}
}

json answer;

void AnswerOutput() {
	std::cout << answer << std::endl;
}
