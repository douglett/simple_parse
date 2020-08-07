#include "parse.hpp"
#include <map>
using namespace std;

namespace parse {
	InputFile  input; // global source file container
	Node       globals, locals, decls, funcs; // program results
	
	void _script_hoist();
	void _script_globals();
	void _script_funcs();


	int script(const std::string& fname) {
		// reset
		globals   = { "globals" };
		locals    = { "locals" };
		decls     = { "function-declarations" };
		funcs     = { "functions" };
		// load
		if (input.load(fname)) exit(1);
		// parse
		_script_hoist(); // hoist-funcs
		decls.show();
		_script_globals();
		globals.show();
		_script_funcs();
		funcs.show();
		return 0;
	}
	
	Node script_getvar(const std::string& name) {
		for (const auto& l : locals.kids)
			if (l.value == name) return { "var-local", name };
		for (const auto& g : globals.kids)
			if (g.value == name) return { "var-global", name };
		input.die("undefined-variable: "+name); // error checking
		return { "??" };
	}

	Node script_getdecl(const std::string& name) {
		for (auto& d : decls.kids)
			if (d.at("name").value == name) return d;
		printf("TODO: call exists check\n");
		// input.die("undefined-function: "+name); // error checking
		return { "??" };
	}
	
	void _script_hoist() {
		printf("TODO: hoist\n");
		printf("TODO: hoist function redef check\n");
	}

	void _script_globals() {
		// parse all dims
		while (!input.eof())
			if      (input.eol()) input.nextline(); // skip empty lines
			else if (input.peeklower() == "dim") globals.push(stmt_dim()); // make dim
			else    break; // end of dims
		// check for duplicate values
		map<string, int> gcount;
		for (const auto& g : globals.kids)
			if (++gcount[g.value] > 1) input.die("globals: duplicate ["+g.value+"]");
	}
	
	void _script_funcs() {
		while (!input.eof()) {
			if      (input.eol()) input.nextline(); // skip whitespace
			else if (input.peeklower() == "function") funcs.push(func()); // make function
			else    break; // end of funcs
		}
	}
}