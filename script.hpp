#pragma once
#include "globals.hpp"
#include "func.hpp"
#include "stmt.hpp"
using namespace std;

struct Script : ProgramStack::FunctionCaller {
	vector<Stmt_dim> dims;
	vector<Func> funcs;

	int build() {
		progstack.reset(); // force new program stack
		script_hoist(); // hoist-funcs
		// structs
		script_dims();
		script_funcs();
		return 0;
	}

	int run() {
		progstack.reset();
		for (auto& d : dims)
			d.run();
		return call("main");
	}

	int call(const string& name) {
		for (auto& f : funcs)
			if (f.name == name) return f.run();
		fprintf(stderr, "Script: missing function definition: %s\n", name.c_str()), exit(1);
		//return 0;
	}

private:
	void script_hoist() {
		input.seekline(0); // goto start
		while (!input.eof()) {
			if (input.peek() == "function") {
				input.next();
				if (!input.is_identifier()) input.die(); // expect function name
				string name = input.peek(); // get name
				printf("script-hoist: %s\n", name.c_str());
			}
			input.nextline();
		}
		input.seekline(0); // goto start
	}

	void script_dims() {
		while (!input.eof())
			if      (input.eol()) input.nextline(); // skip empty lines
			else if (input.peeklower() == "dim") dims.push_back({}), dims.back().build(); // make dim
			else    break;
	}

	void script_funcs() {
		while (!input.eof()) {
			if      (input.eol()) input.nextline(); // skip whitespace
			else if (input.peeklower() == "function") funcs.push_back({}), funcs.back().build(); // make function
			else    break;
		}
	}
};