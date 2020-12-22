#include "parse.hpp"
using namespace std;

namespace parse {
	Node _stmt_print();
	Node _stmt_call();
	Node _stmt_assign();

	Node stmt() {
		if      (input.peeklower() == "print") return _stmt_print();
		else if (input.peeklower() == "call")  return _stmt_call();
		else if (input.is_identifier())        return _stmt_assign();
		else    input.die();
		return { "??" }; // should die before here
	}

	Node stmt_dim() {
		if (input.peeklower() != "dim") input.die(); // is dim statement
		input.next();
		if (!input.is_identifier()) input.die(); // expect identifier
		string name = input.peek(); // save dim name
		input.next();
		// note: redim error checking handled in parent

		// dim array
		if (input.peek() == "[") {
			// deprecated error
			fprintf(stderr, "warning: arrays deprecated\n"), exit(1);
			// 
			// input.next();
			// Node mydim = { "dim_array", name }; // holds the array definition
			// // get array size
			// auto size_expr = expr_zero(); // default array size of zero
			// if (input.peek() != "]") size_expr = expr(); // if not empty brackets, get size expression
			// if (input.peek() != "]") input.die(); // array brackets end
			// input.next();
			// // get string expression
			// if (input.peek() == "=") { // optional assignement operation
			// 	input.next();
			// 	mydim.push( strexp() ); // create string expression and append
			// }
			// // end expression
			// if (!input.eol()) input.die(); // end-line
			// input.nextline();
			// return mydim;
		}

		// dim single int
		else {
			auto myexpr = expr_zero(); // default is zero (empty)
			if (input.peek() == "=") // optional assignement operation
				input.next(), myexpr = expr(); // get expression
			if (!input.eol()) input.die(); // end-line
			input.nextline();
			return { "dim", name, {
				myexpr
			}};
		}
	}

	Node _stmt_print() {
		Node stmt = { "stmt-print" };
		if (input.peeklower() != "print") input.die();
		input.next();
		int list_empty = input.eol(); // print can have no arguments (empty line)
		while (!list_empty) {
			// list contents
			if      (input.is_strlit()) stmt.push({ "string-literal", input.peek() }), input.next(); // literal
			else    stmt.push( expr() ); // should be expression
			// next list item
			if      (input.eol()) break; // list end
			else if (input.peek() == ",") input.next(); // next list item
			else    input.die(); // otherwise error
		}
		if (!input.eol()) input.die(); // expect endline
		input.nextline();
		return stmt;
	}

	Node _stmt_call() {
		if (input.peeklower() != "call") input.die(); // call keyword
		input.next();
		if (!input.is_identifier()) input.die(); // get function name
		auto name = input.peek();
		script_get_decl(name); // make sure function declaration exists
		input.next();
		if (!input.eol()) input.die(); // end-line
		input.nextline();
		return { "stmt-call", "", {
			{ "name", name }
		}};
	}

	Node _stmt_assign() {
		if (!input.is_identifier()) input.die(); // get variable name
		auto name = input.peek();
		input.next();
		auto var = script_get_var(name);
		if (input.peek() != "=") input.die(); // assignement operation
		input.next();
		auto myexpr = expr();
		if (!input.eol()) input.die(); // end-line
		input.nextline();
		return { "stmt-assign", "", {
			var,
			myexpr
		}};
	}
}