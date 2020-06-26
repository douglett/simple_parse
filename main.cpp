#include "globals.hpp"
#include "func.hpp"
#include <iostream>
using namespace std;


int main() {
	printf("hello world\n");
	// InputFile inf("test.bas");
	input.load("test.bas");

	Func f;
	f.build();
}