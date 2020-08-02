#include "parse.hpp"
using namespace std;

namespace parse {
	Node _func_locals();
	Node _func_body();
	Node _func_end();

	Node func() {
		auto header = func_header();
		locals      = _func_locals();
		auto body   = _func_body();
		auto fend   = _func_end();
		// return the function
		auto name = header.at("name").value;
		return { "function", name, {
			// header.at("name"),
			header,
			locals,
			body
		}};
	}

	Node func_header() {
		if (input.peeklower() != "function") input.die(); // function keyword
		input.next();
		if (!input.is_identifier()) input.die(); // function name
		string name = input.peek();
		input.next();
		if (!input.eol()) input.die(); // endline
		input.nextline();
		return { "function-header", "", {
			{ "name", name }
		}};
	}

	Node _func_locals() {
		printf("TODO: redef check global\n");
		Node mylocals  = { "locals" }; // reset locals
		while (!input.eof())
			if      (input.eol()) input.nextline(); // skip empty lines
			else if (input.peeklower() == "dim") mylocals.push(stmt_dim()); // make dim
			else    break; // end of dims
		return mylocals;
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