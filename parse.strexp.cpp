#include "parse.hpp"
using namespace std;

namespace parse {
	int _op_concat();

	Node strexp() {
		// "string" .. int .. "string"
		if (!input.is_strlit()) input.die();
		Node str_expr = { "string-expr" };
		str_expr.push({ "string-literal", input.peek() }), input.next(); // string literal
		// single dimension list of strings
		while (true) {
			// check if we are concatonating
			if  (input.eol()) break; // list end
			if  (!_op_concat()) input.die("expected concat operator (..)"); // expect concatonation operator
			// get expressions
			if   (input.is_strlit()) str_expr.push({ "string-literal", input.peek() }), input.next(); // string literal
			else str_expr.push( expr() ); // expected numerical expression
		}
		// final value
		return str_expr;
	}

	int _op_concat() {
		// parse possible ".." operator
		auto pos = input.pos;
		if (input.peek() != ".") return 0;
		input.next();
		if (input.peek() != ".") return input.pos = pos, 0;
		input.next();
		return 1;
	}
}