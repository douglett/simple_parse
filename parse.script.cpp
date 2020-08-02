#include "parse.hpp"
using namespace std;

namespace parse {
	InputFile  input; // global source file container
	Node       globals, locals, funcs; // program results
	
	void _script_globals();
	void _script_funcs();


	int script(const std::string& fname) {
		// reset
		globals = { "globals" };
		locals  = { "locals" };
		funcs   = { "funcs" };
		// load
		if (input.load(fname)) exit(1);
		// parse
		_script_globals();
		globals.show();
		_script_funcs();
		funcs.show();
		return 0;
	}

	void _script_globals() {
		while (!input.eof())
			if      (input.eol()) input.nextline(); // skip empty lines
			else if (input.peeklower() == "dim") globals.push(stmt_dim()); // make dim
			else    break; // end of dims
	}
	
	void _script_funcs() {
		while (!input.eof()) {
			if      (input.eol()) input.nextline(); // skip whitespace
			else if (input.peeklower() == "function") funcs.push(func()); // make function
			else    break; // end of funcs
		}
	}
}