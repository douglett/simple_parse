#include "parse.hpp"
using namespace std;

namespace parse {
	Node _comp();
	Node _term();
	Node _prod();
	Node _atom();
	Node _call(const string& name);
	Node _array_index(const string& name);
	Node _brackets();

	Node expr() {
		return { "expr", "", { _comp() }};
	}
	
	Node expr_zero() {
		return { "expr", "", { {"number", "0"} }};
	}

	Node expr_call() {
		// return call expression, or error
		if (!input.is_identifier()) input.die();
		auto name = input.peek();
		input.next();
		return _call(name);
	}

	Node expr_assignable() {
		// return assignable variable, array_index, or error
		if (!input.is_identifier()) input.die();
		auto name = input.peek();
		input.next();
		if    (input.peek() == "[")  return _array_index(name);
		else  return script_get_var(name);
	}

	Node _comp() {
		auto lhs = _term();
		// find operator in list
		string op;
		int pos = input.pos;
		for (auto opcode : vector<string>{ "==", "!=", ">=", "<=", ">", "<" }) {
			for (char opsymbol : opcode) {  // see if the next X characters correspond to the opcode
				if (input.peek() != string(1, opsymbol)) break;  // missing character - break
				input.next();
				op += opsymbol;  // add to code
			}
			if (op == opcode) break;  // full code found! break here
			op = "", input.pos = pos; // not found, reset and restart
		}
		// if operator was found
		if (op != "") {
			auto rhs = _term();
			return { "operator", op, { lhs, rhs } };
		}
		return lhs;
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
		if      (input.is_identifier()) {
			input.next();
			// a bit of duplication here with expr_assignable above. OK?
			if      (input.peek() == "(")  return _call(value);
			else if (input.peek() == "[")  return _array_index(value);
			else    return script_get_var(value);
		}
		else if (input.is_integer())   return input.next(), Node{ "number", value };
		else if (input.peek() == "(")  return _brackets();
		else    input.die();
		return  { "??" };
	}

	Node _call(const string& name) {
		// arguments list start
		if (input.peek() != "(") input.die(); 
		input.next();
		// comma seperated argument list
		Node args = { "arguments" };
		while (true) {
			if (input.peek() == ")") break; // break on close-bracket. list can be empty
			args.push(expr());
			if (input.peek() != ",") break; // make sure list is comma-seperated
			input.next();
		}
		// argument list end
		if (input.peek() != ")") input.die();
		input.next();
		// argument count check
		auto decl   = script_get_decl(name); // get global declaration. forces existance check
		int decarg  = decl.at("arguments").length(), 
			callarg = args.length();
		if (decarg != callarg) 
			input.die("call: expected " + to_string(decarg) + " arguments (got " + to_string(callarg) + ")");
		// return function call
		return { "function-call", "", {
			{ "name", name },
			args
		}};
	}

	Node _array_index(const string& name) {
		auto var = script_get_var(name);
		if (input.peek() != "[") input.die(); 
		input.next();
		auto addr = expr();
		if (input.peek() != "]") input.die(); 
		input.next();
		return { "array-access", "", {
			var,
			addr
		}};
	}

	Node _brackets() {
		if (input.peek() != "(") input.die();
		input.next();
		auto lhs = _comp();
		if (input.peek() != ")") input.die();
		input.next();
		return { "operator", "()", { lhs } };
	}
}