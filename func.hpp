#pragma once
#include "inputfile.hpp"
#include "stmt.hpp"
#include <string>
using namespace std;

struct Func {
	//const string ERR = "syntax error";
	string func_name;
	vector<Stmt> statements;

	int build(InputFile& inf) {
		func_start(inf);
		func_body(inf);
		func_end(inf);
		return 0;
	}

private:
	void func_start(InputFile& inf) {
		if (inf.peeklower() != "function") inf.die();
		inf.next();
		if (!inf.is_identifier()) inf.die();
		func_name = inf.peek();
		printf("function name: %s\n", func_name.c_str());
		inf.next();
		if (!inf.eol()) inf.die();
		inf.nextline();
	}

	void func_end(InputFile& inf) {
		if (inf.peeklower() != "end") inf.die();
		inf.next();
		if (inf.peeklower() != "function") inf.die();
		inf.next();
		if (!inf.eol()) inf.die();
		inf.nextline();
	}

	void func_body(InputFile& inf) {
		Stmt st;
		while (!inf.eof()) {
			if      (inf.peeklower() == "end") break;
			else if (inf.eol()) inf.nextline();
			else    st.build(inf);
		}
	}
};