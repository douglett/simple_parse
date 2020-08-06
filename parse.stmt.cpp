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
		// note: redim error checking handled in parent
		input.next();
		auto myexpr = expr_zero(); // default is zero (empty)
		if (input.peek() == "=") // optional assignement operation
			input.next(), myexpr = expr(); // get expression
		if (!input.eol()) input.die(); // end-line
		input.nextline();
		return { "dim", name, {
			myexpr
		}};
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
		
		// if (!progstack.exists(name)) input.die("undefined-variable"); // error checking
		printf("TODO: call exists check\n");
		printf("CALL: %d\n", isdec(name));

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
		// if (!progstack.exists(name)) input.die("undefined-variable"); // error checking
		input.next();
		if (input.peek() != "=") input.die(); // assignement operation
		input.next();
		auto myexpr = expr();
		if (!input.eol()) input.die(); // end-line
		input.nextline();
		return { "stmt-assign", "", {
			{ "name", name },
			myexpr
		}};
	}
}