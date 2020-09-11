#pragma once
#include "inputfile.hpp"
#include "node.hpp"

namespace parse {
	extern InputFile  input; // global source file container
	extern Node       globals, locals, decls, funcs; // program results
	
	int   script(const std::string& fname);
	Node  script_get_var(const std::string& name);
	Node  script_get_decl(const std::string& name);
	Node  func();
	Node  func_decl();
	Node  stmt();
	Node  stmt_dim();
	Node  expr();
	Node  expr_zero();
	Node  strexp();
}