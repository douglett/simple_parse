#include "parse.hpp"
#include <iostream>
using namespace std;

namespace runtime1 {
	typedef  uint32_t  i32;
	i32 call(const std::string& name, const vector<i32>& args);
	i32 globals();
}

int main() {
	printf("hello world\n");
	parse::init();
	// parse::script("tests/stdlib.bas");
	parse::script("tests/test.bas");
	// parse::script("tests/01.hello.bas");
	// parse::script("tests/02.doug1.bas");
	parse::show();

	printf("::start::\n");
	// runtime1::globals();
	// runtime1::call("main", {});
}