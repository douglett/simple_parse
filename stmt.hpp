#pragma once
#include "inputfile.hpp"
#include <string>
using namespace std;

struct Stmt {
	string type;

	int build(InputFile& inp) {
		printf("%02d: stmt: %s\n", inp.linepos+1, inp.peek().c_str());
		if (inp.peeklower() == "print") do_print(inp);
		else inp.die();
		return 0;
	}

private:
	void do_print(InputFile& inp) {
		type = "print";
		inp.next();
		while (inp.is_strlit()) inp.next();
		inp.nextline();
	}
};