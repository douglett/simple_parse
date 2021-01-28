#include "parse.hpp"
#include <map>
using namespace std;

namespace parse {
	void _func_args(const Node& arguments);
	void _func_locals();

	Node func() {
		auto decl   = func_decl();
		locals      = { "locals" }; // reset locals. saved in namespace
			_func_args( decl.at("arguments") );
			_func_locals();
		auto block  = stmt_block("function");
		// return the function
		auto name = decl.value;
		Node myfunc = { "function", name, {
			decl,
			locals,
			block
		}};
		locals = {}; // reset local variables
		return myfunc;
	}

	Node func_decl() {
		if (input.peeklower() != "function") input.die(); // function keyword
		input.next();
		if (!input.is_identifier()) input.die(); // function name
		string name = input.peek();
		input.next(); // note: redefinition checks handled in '_script_hoist'
		// function arguments start
		if (input.peek() != "(") input.die();
		input.next();
		// arguments list
		Node args = { "arguments" };
		while (true) {
			if (!input.is_identifier()) break;
			args.push({ "dim", input.peek() });
			input.next();
			if (input.peek() != ",") break;
			input.next(); // eat comma
		}
		// check for duplicate arguments
		check_dup_values(args, "function-arguments");
		// function arguments end
		if (input.peek() != ")") input.die();
		input.next();
		// end line
		if (!input.eol()) input.die();
		input.nextline();
		return { "function-declaration", name, {
			// { "name", name },
			{ "scope", "script (default?)" },
			args
		}};
	}

	Node func_decl_sys() {
		if (input.peeklower() != "system") input.die(); // system keyword
		input.next();
		auto decl = func_decl();
		decl.at("scope").value = "system";
		return decl;
	}

	void _func_args(const Node& arguments) {
		// copy arguments into local variables
		int argc = 0;
		for (auto arg : arguments.kids) {
			arg.push({ "argument", to_string(argc) });
			locals.push(arg);
			argc++;
		}
		check_dup_values(locals, "local-arguments"); // check for duplicate local variables (will also check clones argument names)
	}

	void _func_locals() {
		// parse local variables
		while (!input.eof())
			if      (input.eol()) input.nextline(); // skip empty lines
			else if (input.peeklower() == "dim") locals.push(stmt_dim()); // make dim
			else    break; // end of dims
		check_dup_values(locals, "locals"); // check for duplicate local variables (will also check clones argument names)
	}
}