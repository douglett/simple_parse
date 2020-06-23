#include "func.hpp"
#include <iostream>
using namespace std;


int main() {
	printf("hello world\n");
	InputFile inf("test.bas");

	Func f;
	f.build(inf);
}