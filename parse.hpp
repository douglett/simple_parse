#pragma once
#include "inputfile.hpp"
#include "node.hpp"

namespace parse {
	extern InputFile  input; // global source file container
	extern Node       globals, locals, funcdecs, funcs; // program results
	
	int   script(const std::string& fname);
	Node  func();
	Node  func_header();
	Node  stmt();
	Node  stmt_dim();
	Node  expr();
	Node  expr_zero();
}