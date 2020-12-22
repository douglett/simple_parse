#include "parse.hpp"
#include <map>
using namespace std;

namespace parse {
	Node _func_locals();
	Node _func_body();
	Node _func_end();

	Node func() {
		auto decl   = func_decl();
		locals      = _func_locals(); // save locals in namespace, for error checking
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
		map<string, int> valcount;
		for (const auto& val : args.kids)
			if (++valcount[val.value] > 1) input.die("function-arguments: duplicate ["+val.value+"]");

		// function arguments end
		if (input.peek() != ")") input.die();
		input.next();
		// end line
		if (!input.eol()) input.die();
		input.nextline();
		return { "function-declaration", name, {
			{ "name", name },
			args
		}};
	}

	Node _func_locals() {
		// parse all dims
		Node mylocals  = { "locals" }; // reset locals
		while (!input.eof())
			if      (input.eol()) input.nextline(); // skip empty lines
			else if (input.peeklower() == "dim") mylocals.push(stmt_dim()); // make dim
			else    break; // end of dims
		return mylocals;
		// check for duplicate local variables
		map<string, int> valcount;
		for (const auto& val : locals.kids)
			if (++valcount[val.value] > 1) input.die("globals: duplicate ["+val.value+"]");
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