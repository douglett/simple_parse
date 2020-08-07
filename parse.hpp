#pragma once
#include "inputfile.hpp"
#include "node.hpp"

namespace parse {
	extern InputFile  input; // global source file container
	extern Node       globals, locals, decls, funcs; // program results
	
	int   script(const std::string& fname);
	Node  script_getvar(const std::string& name);
	Node  script_getdecl(const std::string& name);
	Node  func();
	Node  func_decl();
	Node  stmt();
	Node  stmt_dim();
	Node  expr();
	Node  expr_zero();
}