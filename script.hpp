#pragma once
#include "globals.hpp"
#include "func.hpp"
#include "stmt.hpp"
using namespace std;

struct Script {
	vector<Stmt_dim> dims;
	vector<Func> funcs;

	int build() {
		progstack.reset(); // force new program stack
		// hoist-funcs
		// structs
		script_dims();
		script_funcs();
		return 0;
	}

	int run() {
		progstack.reset();
		for (auto& d : dims)
			d.run();
		for (auto& f : funcs)
			if (f.name == "main") return f.run();
		return 0;
	}

private:
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