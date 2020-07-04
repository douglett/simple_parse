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

private:
	void atom() {
		if      (input.is_identifier()) type = "identifier", value = input.peek(), input.next();
		else if (input.is_integer()) type = "number", value = input.peek(), input.next();
		else    input.die();
	}
};