#pragma once
#include "inputfile.hpp"
#include "node.hpp"

namespace parse {
	extern InputFile  input; // global source file container
	extern Node       globals, locals, decls, funcs; // program results
	
	int   init();
	int   script(const std::string& fname);
	int   show();
	Node  script_get_dim(const std::string& name);
	Node  script_get_decl(const std::string& name);
	Node  func();
	Node  func_decl();
	Node  func_decl_sys();
	Node  stmt();
	Node  stmt_dim(int local);
	Node  stmt_block();
	Node  stmt_block_end(const std::string& blocktype);
	Node  expr();
	Node  expr_zero();
	Node  expr_call();
	Node  expr_assignable();
	// Node  strexp();


	// not sure where to put this stuff
	void check_dup_values(const Node& nlist, const std::string& errname);
}