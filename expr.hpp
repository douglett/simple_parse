#pragma once
#include "globals.hpp"
using namespace std;

//struct Expr_mixed {

struct Expr {
	string type, value;

	int build() {
		atom();
		return 0;
	}

	int run() {
		if      (type == "identifier") return progstack.get(value);
		else if (type == "number") return stoi(value);
		return 0; // error? should not happen
	}

private:
	void atom() {
		if      (input.is_identifier()) type = "identifier", value = input.peek(), input.next();
		else if (input.is_integer()) type = "number", value = input.peek(), input.next();
		else    input.die();
	}
};