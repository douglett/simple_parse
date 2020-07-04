#pragma once
#include "globals.hpp"
#include "stmt.hpp"
#include <string>
using namespace std;

struct Func {
	//const string ERR = "syntax error";
	string func_name;
	vector<string> dims;
	vector<Stmt> statements;

	int build() {
		func_start();
		func_dims();
		func_body();
		func_end();
		return 0;
	}
	Node makeast() const {
		Node ast = { "function", func_name, {
			{ "dims", "", {}},
			{ "statements", "", {}},
		}};
		for (auto& st : statements)
			ast.at("statements").push( st.makeast() );
		return ast;
	}

private:
	void func_start() {
		if (input.peeklower() != "function") input.die();
		input.next();
		if (!input.is_identifier()) input.die();
		func_name = input.peek();
		//printf("function name: %s\n", func_name.c_str());
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

	void func_dims() {
		while (!input.eof())
			if      (input.eol()) input.nextline(); // skip empty lines
			else if (input.peeklower() == "dim") {
				input.next();
				if (!input.is_identifier()) input.die();
				dims.push_back(input.peek()), input.next();
			}
			else    break;
	}

	void func_body() {
		while (!input.eof())
			if      (input.peeklower() == "end") break; // expect end-function here
			else if (input.eol()) input.nextline(); // skip empty lines
			else    { // expect statement
				Stmt st;
				st.build();
				statements.push_back(st);
			}
	}
};