#pragma once
#include "inputfile.hpp"
#include <string>
using namespace std;

struct Stmt {
	string type;

	vector<string> literals; // temp

	int build(InputFile& inp) {
		printf("%02d: stmt: %s\n", inp.linepos+1, inp.peek().c_str());
		if (inp.peeklower() == "print") do_print(inp);
		else inp.die();
		return 0;
	}

private:
	void do_print(InputFile& inp) {
		printf("parsing print...\n");
		type = "print";
		inp.next();
		//while (inp.is_strlit()) inp.next();
		while (true)
			if (inp.is_strlit()) literals.push_back(inp.peek()), inp.next();
			else break;
		if (!inp.eol()) inp.die();
		inp.nextline();

		for (auto l : literals)
			printf("  literal: %s\n", l.c_str());
	}
};