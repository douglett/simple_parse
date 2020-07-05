#pragma once
#include "globals.hpp"
#include "expr.hpp"
#include <string>
using namespace std;

// generic statement base class
struct Stmt_type {
	virtual int build() { return 0; }
	virtual int run() { return -1; }
};

// print statements
struct Stmt_print : Stmt_type {
	struct PrintList {
		string type, strlit;
		Expr expr;
		PrintList(const string& val) { type = "strlit"; strlit = val; }
		PrintList(const Expr& val) { type = "expr"; expr = val; }
	};
	vector<PrintList> plist;

	int build() {
		if (input.peeklower() != "print") input.die();
		input.next();
		while (true)
			if      (input.eol()) break; // list end
			else if (input.is_strlit()) plist.emplace_back( escape_literal(input.peek()) ), input.next(); // literal
			else    plist.emplace_back(Expr()), plist.back().expr.build(); // should be expression
		if (!input.eol()) input.die(); // expect endline
		input.nextline();
		return 0;
	}

	int run() {
		static const string spc = " ";
		string str;
		for (auto& pl : plist)
			if      (pl.type == "strlit") str += pl.strlit + spc;
			else if (pl.type == "expr") str += "0" + spc;
			else    str += "??";
		printf("%s\n", str.c_str());
		return 0;
	}

private:
	string escape_literal(string str) {
		if (str.length() >= 2 && str.front() == '"' && str.back() == '"')
			return str.substr(1, str.length()-2);
		return str;
	}
};

// assign statement
struct Stmt_assign : Stmt_type {
	string name;
	Expr expr;

	int build() {
		if (!input.is_identifier()) input.die(); // get variable name
		name = input.peek();
		input.next();
		if (input.peek() != "=") input.die(); // assignement operation
		input.next();
		expr.build(); // get expression
		if (!input.eol()) input.die(); // end-line
		input.nextline();
		return 0;
	}
};

// statement parser god class
struct Stmt : Stmt_type {
private:
	string type;
	Stmt_print  stmt_print;
	Stmt_assign stmt_assign;
public:
	int build() {
		//printf("%02d: stmt: %s\n", input.linepos+1, input.peek().c_str());
		if      (input.peeklower() == "print") type = "print", stmt_print.build();
		else if (input.is_identifier()) type = "assign", stmt_assign.build();
		else    input.die();
		return 0;
	}
	int run() {
		return get()->run();
	}
	Stmt_type* get() {
		if (type == "print")  return &stmt_print;
		if (type == "assign") return &stmt_assign;
		fprintf(stderr, "unknown statement type [%s]\n", type.c_str());
		exit(1);
	}
};