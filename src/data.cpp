#include "data.hpp"
#include <fstream>
#include <limits>
#include <utility>
#include <iostream>

json data;

void DataInit(const char *input) {
	std::ifstream fin(input);
	fin >> data;
	vertex.emplace(data["start_vertex"], 0);
	for(size_t i=0; i<data["target_vertex_set"].size(); ++i)
		vertex.emplace(data["target_vertex_set"][i]["id"], i+1);
	for(const auto &edge: data["edge_set"]) {
		vertex.emplace(edge["from"], vertex.size());
		vertex.emplace(edge["to"], vertex.size());
	}
	Map::Init();
}

std::map<std::string, int> vertex;

double Map::dis[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
int Map::path[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

void Map::Init() {
	std::fill(*dis, *dis+sizeof(dis)/sizeof(double), std::numeric_limits<double>::infinity());
	std::fill(*path, *path+sizeof(path)/sizeof(int), -1);
	for(const auto &edge: data["edge_set"]) {
		int x=vertex[edge["from"]], y=vertex[edge["to"]];
		dis[x][y]=edge["dis"];
	}
	for(size_t k=0; k<vertex.size(); ++k)
		for(size_t i=0; i<vertex.size(); ++i)
			for(size_t j=0; j<vertex.size(); ++j)
				if(dis[i][k]+dis[k][j]<dis[i][j]) {
					dis[i][j]=dis[i][k]+dis[k][j];
					path[i][j]=k;
				}
}

double Map::CalcPathDistance(const std::vector<int> &path) {
	double res=0;
	res+=dis[0][path.front()];
	for(size_t i=1; i<path.size(); ++i)
		res+=dis[path[i-1]][path[i]];
	res+=dis[path.back()][0];
	return res;
}

void Map::PathPrint(std::vector<int> &res, int x, int y) {
	if(path[x][y]==-1)
		res.push_back(x);
	else {
		PathPrint(res, x, path[x][y]);
		PathPrint(res, path[x][y], y);
	}
}

json Answer::result;

void Answer::AddPath(std::vector<int> target_path) {
	std::vector<int> path;
	Map::PathPrint(path, 0, target_path.front());
	for(size_t i=1; i<target_path.size(); ++i)
		Map::PathPrint(path, target_path[i-1], target_path[i]);
	Map::PathPrint(path, target_path.back(), 0);
	path.push_back(0);
	json item;
	item["distance"]=Map::CalcPathDistance(target_path);
	item["target_path"]=std::move(target_path);
	item["path"]=std::move(path);
	result.emplace_back(std::move(item));
}

void Answer::Output() {
	std::cout << result << std::endl;
}
