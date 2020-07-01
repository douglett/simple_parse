#pragma once
#include "globals.hpp"
#include <string>
using namespace std;

// generic statement base class
struct Stmt_type {
	virtual int build() { return 0; }
};

// print statements
struct Stmt_print : Stmt_type {
	vector<string> literals; // temp

	int build() {
		printf("parsing print...\n");
		input.next();
		//while (input.is_strlit()) input.next();
		while (true)
			if (input.is_strlit()) literals.push_back(input.peek()), input.next();
			else break;
		if (!input.eol()) input.die();
		input.nextline();

		for (auto l : literals)
			printf("  literal: %s\n", l.c_str());
		return 0;
	}
};

// statement parser god class
struct Stmt : Stmt_type {
private:
	string type;
	Stmt_print s_print;
public:
	int build() {
		printf("%02d: stmt: %s\n", input.linepos+1, input.peek().c_str());
		if (input.peeklower() == "print") type = "print", s_print.build();
		else input.die();
		return 0;
	}
	Stmt_type* get() {
		return &s_print;
	}
};