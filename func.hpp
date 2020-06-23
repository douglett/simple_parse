#include "inputfile.hpp"

struct Func {
	//const string ERR = "syntax error";

	int build(InputFile& inf) {
		// function start
		if (inf.peek() != "function") return 0;
		inf.next();
		if (!inf.is_identifier()) inf.die();
		auto name = inf.peek();
		printf("function name: %s\n", name.c_str());
		inf.next();
		if (!inf.eol()) inf.die();
		inf.nextline();

		// function end
		printf("%s\n", inf.peek().c_str());
		if (inf.peek() != "end") inf.die("a");
		inf.next();
		if (inf.peek() != "function") inf.die();
		inf.next();
		if (!inf.eol()) inf.die();
		inf.nextline();
		return 0;
	}
};