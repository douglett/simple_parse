#include "globals.hpp"
#include "script.hpp"
#include <iostream>
using namespace std;


int main() {
	printf("hello world\n");
	// InputFile inf("test.bas");
	input.load("test.bas");

	Script s;
	s.build();
	int res = s.run();
	printf("result: %d\n", res);
}