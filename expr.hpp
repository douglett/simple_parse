#pragma once
#include "globals.hpp"
using namespace std;

//struct Expr_mixed {

struct Expr {
	string type, value;
	vector<Expr> kids;

	int build() {
		//atom();
		*this = term();
		return 0;
	}

	int zero() {
		*this = { "number", "0", {} };
		return 0;
	}

	int run() {
		if      (type == "identifier") return progstack.get(value);
		else if (type == "number") return stoi(value);
		else if (type == "operator") {
			auto l = kids.at(0).run();
			if (value == "()") return l;
			auto r = kids.at(1).run();
			if (value == "+") return l + r;
			if (value == "-") return l - r;
			if (value == "*") return l * r;
			if (value == "/") return l / r;
		}
		fprintf(stderr, "error: unknown expr: [%s] [%s]\n", type.c_str(), value.c_str()), exit(1);
	}

private:
	Expr term() {
		auto lhs = prod();
		if (input.peek() == "+" || input.peek() == "-") {
			auto op = input.peek();
			input.next();
			auto rhs = term();
			return { "operator", op, { lhs, rhs } };
		}
		return lhs;
	}
	Expr prod() {
		auto lhs = atom();
		if (input.peek() == "*" || input.peek() == "/") {
			auto op = input.peek();
			input.next();
			auto rhs = term();
			return { "operator", op, { lhs, rhs } };
		}
		return lhs;
	}
	Expr atom() {
		Expr lhs = { "??", input.peek() };
		if      (input.is_identifier()) input.next(), lhs.type = "identifier";
		else if (input.is_integer())    input.next(), lhs.type = "number";
		else if (input.peek() == "(")   return brackets();
		else    input.die();
		return lhs;
	}
	Expr brackets() {
		if (input.peek() != "(") input.die();
		input.next();
		auto lhs = term();
		if (input.peek() != ")") input.die();
		input.next();
		return { "operator", "()", { lhs } };
	}
};