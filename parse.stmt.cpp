#include "parse.hpp"
using namespace std;

namespace parse {
	Node _stmt_print();
	Node _stmt_print_string();
	Node __print_list();
	Node _stmt_input();
	Node _stmt_call();
	Node _stmt_return();
	Node _stmt_while();
	Node _stmt_if();
	Node _stmt_assign();

	Node stmt() {
		if      (input.peeklower() == "print")  return _stmt_print();
		else if (input.peeklower() == "prints") return _stmt_print_string();
		else if (input.peeklower() == "input")  return _stmt_input();
		else if (input.peeklower() == "call")   return _stmt_call();
		else if (input.peeklower() == "return") return _stmt_return();
		else if (input.peeklower() == "while")  return _stmt_while();
		else if (input.peeklower() == "if")     return _stmt_if();
		else if (input.is_identifier())         return _stmt_assign();
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

	Node stmt_block() {
		Node block = { "stmt-block" };
		// get all statements in block
		while (!input.eof())
			if      (input.eol()) input.nextline(); // skip empty lines
			else if (input.peeklower() == "end" ) break; // expect end-function here
			else if (input.peeklower() == "else") break; // pecial case for if-statement blocks
			else    block.push( stmt() ); // expect statement
		// result
		return block;
	}

	Node stmt_block_end(const std::string& blocktype) {
		// check block end
		if (input.peeklower() != "end") input.die(); // end keyword
		input.next();
		if (input.peeklower() != blocktype) input.die("expected block end [" + blocktype + "]"); // function keyword
		input.next();
		if (!input.eol()) input.die(); // endline
		input.nextline();
		// just return Node for consistency
		return { "stmt-block-end", blocktype };
	}

	Node _stmt_print() {
		Node stmt = { "stmt-print" };
		if (input.peeklower() != "print") input.die();
		input.next();
		if (!input.eol()) stmt.push( __print_list() ); // print can have no arguments (empty line)
		return stmt;
	}

	Node _stmt_print_string() {
		Node stmt = { "stmt-print-string" };
		if (input.peeklower() != "prints") input.die();
		input.next();
		auto var = expr_assignable(); // get assignment location (non-optional)
		if (input.peek() != ",") input.die();
		input.next();
		stmt.push({ "target", "", { var } }); // add assignment target
		stmt.push( __print_list() ); // add list (must have 1+ paramaters)
		return stmt;
	}

	Node __print_list() {
		Node stmt = { "list" };
		while (true) {
			// list contents
			if      (input.is_strlit()) stmt.push({ "string-literal", input.peek() }), input.next(); // literal
			else if (input.peek() == "$") input.next(), stmt.push({ "$", "", { expr_assignable() } }); // string pointer
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
	
	Node _stmt_input() {
		if (input.peeklower() != "input") input.die();
		input.next();
		// check for custom text prompt
		string prompt = "\"> \"";
		if (input.is_strlit()) {
			prompt = input.peek();
			input.next();
			if (input.peek() != ",") input.die(); // expect comma
			input.next();
		}
		// get assignment target
		auto var = expr_assignable();
		// end statement
		if (!input.eol()) input.die(); // end-line
		input.nextline();
		return { "stmt-input", "", {
			{ "prompt", prompt },
			{ "target", "", { var } }
		}};
	}

	Node _stmt_call() {
		// call keyword and function name
		if (input.peeklower() != "call") input.die(); // call keyword
		input.next();
		auto mycall = expr_call();
		if (!input.eol()) input.die(); // end-line
		input.nextline();
		return { "stmt-call", "", {
			mycall
		}};
	}

	Node _stmt_return() {
		if (input.peeklower() != "return") input.die(); // return keyword
		input.next();
		auto myexpr = expr_zero();
		if (!input.eol()) myexpr = expr(); // optional return expression
		if (!input.eol()) input.die(); // expect end-line
		input.nextline();
		return { "stmt-return", "", {
			myexpr
		}};
	}

	Node _stmt_while() {
		if (input.peeklower() != "while") input.die(); // keyword
		input.next();
		auto condition = expr();
		if (!input.eol()) input.die(); // end-line
		input.nextline();
		auto block = stmt_block(); // get statement block
		auto endw  = stmt_block_end("while");
		return { "stmt-while", "", {
			condition,
			block
		}};
	}

	Node _stmt_if() {
		if (input.peeklower() != "if") input.die(); // must start with if keyword
		Node stmt = { "stmt-if" };
		// first condition block
		input.next();
		auto condition = expr();
		if (!input.eol()) input.die(); // end-line
		input.nextline();
		auto block = stmt_block(); // get statement block
		stmt.push({ "if-case", "if", { condition, block } });
		// else statements
		while (input.peeklower() == "else") {
			input.next();
			// else-if block
			if (input.peeklower() == "if") {
				input.next();
				auto condition = expr();
				if (!input.eol()) input.die(); // end-line
				input.nextline();
				auto block = stmt_block(); // get statement block
				stmt.push({ "if-case", "else-if", { condition, block } });
			}
			// else block
			else {
				if (!input.eol()) input.die(); // end-line
				input.nextline();
				auto block = stmt_block(); // get statement block
				stmt.push({ "if-case", "else", { block } });
				break; // naked else must be the final case
			}
		}
		// make sure we have the final end-if closure
		auto endif = stmt_block_end("if");
		return stmt;
	}

	Node _stmt_assign() {
		// get assignment target
		auto var = expr_assignable();
		// get expression to assign
		if (input.peek() != "=") input.die(); // assignement operation
		input.next();
		auto myexpr = expr();
		if (!input.eol()) input.die(); // end-line
		input.nextline();
		return { "stmt-assign", "", {
			{ "target", "", { var } },
			myexpr
		}};
	}
}