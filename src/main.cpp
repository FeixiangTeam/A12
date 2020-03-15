#include "data.hpp"
#include <cstdio>

void Solve();

int main(int argc, const char *argv[]) {
	if(argc!=2) {
		printf("Usage: %s input_file\n", argv[0]);
		return 1;
	}
	DataInit(argv[1]);
	Solve();
	Answer::Output();
	return 0;
}
