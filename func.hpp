#pragma once
#include "globals.hpp"
#include "stmt.hpp"
#include <string>
using namespace std;

struct Func {
	//const string ERR = "syntax error";
	string func_name;
	vector<Stmt> statements;

	int build() {
		func_start();
		func_body();
		func_end();
		return 0;
	}

private:
	void func_start() {
		if (input.peeklower() != "function") input.die();
		input.next();
		if (!input.is_identifier()) input.die();
		func_name = input.peek();
		printf("function name: %s\n", func_name.c_str());
		input.next();
		if (!input.eol()) input.die();
		input.nextline();
	}

	void func_end() {
		if (input.peeklower() != "end") input.die();
		input.next();
		if (input.peeklower() != "function") input.die();
		input.next();
		if (!input.eol()) input.die();
		input.nextline();
	}

	void func_body() {
		Stmt st;
		while (!input.eof()) {
			if      (input.peeklower() == "end") break; // expect end-function here
			else if (input.eol()) input.nextline(); // skip empty lines
			else    st.build(); // expect statement
		}
	}
};