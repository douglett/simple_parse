#include "parse.hpp"
#include <map>
using namespace std;

namespace parse {
	InputFile  input; // global source file container
	Node       globals, locals, decls, funcs; // program results
	
	void _script_hoist();
	void _script_system();
	void _script_globals();
	void _script_funcs();


	int init() {
		// initialise or reset environment
		globals   = { "globals" };
		locals    = { "locals" };
		decls     = { "function-declarations" };
		funcs     = { "functions" };
		return 0;
	}
	
	int script(const std::string& fname) {
		// load
		printf(">>> %s\n", fname.c_str());
		if (input.load(fname)) exit(1);
		// parse
		_script_hoist(); // hoist-funcs
		// decls.show();
		_script_system(); // clear system funcs
		_script_globals();
		// globals.show();
		_script_funcs();
		// funcs.show();
		if (!input.eof()) input.die("expected EOF");
		return 0;
	}

	int show() {
		decls.show();
		globals.show();
		funcs.show();
		return 0;
	}
	
	Node script_get_dim(const std::string& name) {
		// return the named variable, with scoping definition
		for (const auto& l : locals.kids)
			// if (l.value == name) return { "var-local", name };
			if (l.value == name) return l;
		for (const auto& g : globals.kids)
			// if (g.value == name) return { "var-global", name };
			if (g.value == name) return g;
		input.die("undefined-variable: "+name); // error checking
		return { "??" };
	}

	Node script_get_decl(const std::string& name) {
		// return the named function declaration
		for (auto& d : decls.kids)
			if (d.value == name) return d;
		input.die("undefined-function: "+name); // error checking
		return { "??" };
	}
	
	void _script_hoist() {
		// find and parse all function declarations, forward-declaring them all
		while (!input.eof())
			if       (input.peeklower() == "function") decls.push(func_decl()); // parse function declaration
			else if  (input.peeklower() == "system"  ) decls.push(func_decl_sys()); // parse system function declaration
			else     input.nextline(); // skip this line
		input.seekline(0); // return to top of script for sequential parsing
		// check for duplicate values
		check_dup_values(decls, "function-declarations");
	}

	void _script_system() {
		// clear system funcs
		while (!input.eof())
			if       (input.eol()) input.nextline(); // skip empty lines
			else if  (input.peeklower() == "system") input.nextline(); // just assume this was handled in hoist
			else     break; // end system
	}

	void _script_globals() {
		// parse all dims
		while (!input.eof())
			if       (input.eol()) input.nextline(); // skip empty lines
			else if  (input.peeklower() == "dim") globals.push(stmt_dim(false)); // make dim
			else     break; // end of dims
		// check for duplicate globals variables
		check_dup_values(globals, "globals");
	}
	
	void _script_funcs() {
		while (!input.eof())
			if       (input.eol()) input.nextline(); // skip whitespace
			else if  (input.peeklower() == "function") funcs.push(func()); // make function
			else     break; // end of funcs
	}
}