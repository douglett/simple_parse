#include "parse.hpp"
#include <map>
using namespace std;

namespace parse {
	Node _func_args(const Node& arguments);
	void _func_locals();
	Node _func_body();
	Node _func_end();

	Node func() {
		auto decl   = func_decl();
		locals      = _func_args( decl.at("arguments") ); // save locals in namespace, for error checking
					  _func_locals();
		auto body   = _func_body();
		auto fend   = _func_end();
		// return the function
		auto name = decl.at("name").value;
		Node myfunc = { "function", name, {
			// header.at("name"),
			decl,
			locals,
			body
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
			{ "name", name },
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

	Node _func_args(const Node& arguments) {
		Node mylocals  = { "locals" }; // reset locals
		// copy arguments into local variables
		int argc = 0;
		for (auto arg : arguments.kids) {
			arg.push({ "argument", to_string(argc) });
			mylocals.push(arg);
			argc++;
		}
		check_dup_values(locals, "local-arguments"); // check for duplicate local variables (will also check clones argument names)
		return mylocals; // return
	}

	void _func_locals() {
		// parse local variables
		while (!input.eof())
			if      (input.eol()) input.nextline(); // skip empty lines
			else if (input.peeklower() == "dim") locals.push(stmt_dim()); // make dim
			else    break; // end of dims
		check_dup_values(locals, "locals"); // check for duplicate local variables (will also check clones argument names)
	}

	Node _func_body() {
		Node body = { "body" };
		while (!input.eof())
			if      (input.peeklower() == "end") break; // expect end-function here
			else if (input.eol()) input.nextline(); // skip empty lines
			else    body.push( stmt() ); // expect statement
		return body;
	}

	Node _func_end() {
		if (input.peeklower() != "end") input.die(); // end keyword
		input.next();
		if (input.peeklower() != "function") input.die(); // function keyword
		input.next();
		if (!input.eol()) input.die(); // endline
		input.nextline();
		return { "??" }; // don't actually return anything here
	}
}