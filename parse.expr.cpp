#include "parse.hpp"
using namespace std;

namespace parse {
	Node _term();
	Node _prod();
	Node _atom();
	Node _brackets();

	Node expr() {
		return { "expr", "", { _term() }};
	}
	
	Node expr_zero() {
		return { "expr", "", { {"number", "0"} }};
	}

	Node _term() {
		auto lhs = _prod();
		if (input.peek() == "+" || input.peek() == "-") {
			auto op = input.peek();
			input.next();
			auto rhs = _term();
			return { "operator", op, { lhs, rhs } };
		}
		return lhs;
	}

	Node _prod() {
		auto lhs = _atom();
		if (input.peek() == "*" || input.peek() == "/") {
			auto op = input.peek();
			input.next();
			auto rhs = _prod();
			return { "operator", op, { lhs, rhs } };
		}
		return lhs;
	}

	Node _atom() {
		auto value = input.peek();
		if      (input.is_identifier()) return input.next(), script_get_var(value);
		else if (input.is_integer())    return input.next(), Node{ "number", value };
		else if (input.peek() == "(")   return _brackets();
		else    input.die();
		return  { "??" };
	}

	Node _brackets() {
		if (input.peek() != "(") input.die();
		input.next();
		auto lhs = _term();
		if (input.peek() != ")") input.die();
		input.next();
		return { "operator", "()", { lhs } };
	}
}