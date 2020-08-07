#pragma once
#include "inputfile.hpp"
#include "node.hpp"

namespace parse {
	extern InputFile  input; // global source file container
	extern Node       globals, locals, funcdecs, funcs; // program results
	
	int   script(const std::string& fname);
	// int   isdef(const std::string& name);
	int   isdec(const std::string& name);
	Node  var_get(const std::string& name);
	Node  func();
	Node  func_dec();
	Node  stmt();
	Node  stmt_dim();
	Node  expr();
	Node  expr_zero();
}