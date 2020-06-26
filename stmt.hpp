#pragma once
#include "inputfile.hpp"
#include <string>
using namespace std;

struct Stmt {
	string type;

	vector<string> literals; // temp

	int build() {
		printf("%02d: stmt: %s\n", input.linepos+1, input.peek().c_str());
		if (input.peeklower() == "print") do_print();
		else input.die();
		return 0;
	}

private:
	void do_print() {
		printf("parsing print...\n");
		type = "print";
		input.next();
		//while (input.is_strlit()) input.next();
		while (true)
			if (input.is_strlit()) literals.push_back(input.peek()), input.next();
			else break;
		if (!input.eol()) input.die();
		input.nextline();

		for (auto l : literals)
			printf("  literal: %s\n", l.c_str());
	}
};