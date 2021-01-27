#include "parse.hpp"
#include <iostream>
using namespace std;

int main() {
	printf("hello world\n");
	parse::init();
	// parse::script("tests/test.bas");
	parse::script("tests/stdlib.bas");
	parse::script("tests/01.hello.bas");
}