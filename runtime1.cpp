#include "node.hpp"
#include "parse.hpp"
#include <iostream>
#include <map>
using namespace std;

namespace runtime1 {
	typedef  uint32_t  i32;
	map<string, i32> stack;
	vector<vector<i32>> heap = { {0} };

	i32 _syscall(const string& name, const vector<i32>& args);
	i32 _fncall(const Node& function, const vector<i32>& args);
	i32 _dim(const Node& mydim, int local);
	i32 _stmt_block(const Node& myblock);
	i32 _stmt(const Node& mystmt);
	i32 _expr(const Node& expr);
	string _string_ptr(i32 ptr);
	i32 _stack_val(const Node& var);
	i32 _stack_deref(const Node& var);
	void _assign_string(const Node& var, const string& str);



	// external

	i32 globals() {
		for (const auto& gdim : parse::globals.kids)
			_dim(gdim, false);
		return 0;
	}

	i32 call(const std::string& name, const vector<i32>& args) {
		const auto& funcdec = parse::decls.at("function-declaration", name);
		return funcdec.at("scope").value == "system"
			? _syscall(name, args)  // system function
			: _fncall( parse::funcs.at("function", name), args );  // user-defined function
	}



	// call

	i32 _fncall(const Node& function, const vector<i32>& args) {
		// printf("fncall: %s %d\n", function.value.c_str(), args.size());
		// function.show();
		// auto& argsdec = function.at("function-declaration").at("arguments");
		// argsdec.show();
		auto& locals = function.at("locals");
		for (auto& local : locals.kids)
			_dim(local, true);
		return _stmt_block( function.at("stmt-block") );
	}

	i32 _syscall(const string& name, const vector<i32>& args) {
		// printf("syscall: %s\n", name.c_str());
		if (name == "malloc" && args.size() == 1) {
			heap.push_back( vector<i32>(args[0], 0) );
			return heap.size()-1;
		} 
		// missing syscall
		fprintf(stderr, "unknown system call: %s:%d\n", name.c_str(), args.size());
		exit(1);
	}



	// func internals

	i32 _dim(const Node& mydim, int local) {
		auto name = (local ? "~" : "") + mydim.value;
		if (stack.count(name) != 0)
			fprintf(stderr, "error: redim: %s\n", name.c_str()), exit(1);
		stack[name] = _expr( mydim.at("expr") );
		return 0;
	}

	i32 _stmt_block(const Node& myblock) {
		for (auto& st : myblock.kids)
			_stmt(st);
		return 0;
	}

	i32 _stmt(const Node& mystmt) {
		if (mystmt.type == "stmt-call") {
			return call( mystmt.at("function-call").at("name").value, {} );
		}
		else if (mystmt.type == "stmt-print" || mystmt.type == "stmt-print-string") {
			string str;
			for (auto &var : mystmt.at("list").kids)
				if      (var.type == "string-literal")  str += InputFile::escape_strlit( var.value );
				else if (var.type == "$")  str += _string_ptr( _expr(var.kids[0]) );
				else if (var.type == "expr")  str += to_string( _expr(var) );
				else    fprintf(stderr, "stmt-print error: %s\n", var.type.c_str()), mystmt.show(), exit(1);
			if    (mystmt.type == "stmt-print")  printf("%s\n", str.c_str()); // dump to console
			else  _assign_string( mystmt.at("target").kids[0], str ); // print into a memory string
			return 0;
		}
		else if (mystmt.type == "stmt-input") {
			// mystmt.show();
			printf("%s", InputFile::escape_strlit( mystmt.at("prompt").value ).c_str()); // input prompt
			string input;
			getline(cin, input); // get input
			auto dst = mystmt.count("var-local") ? mystmt.at("var-local") : mystmt.at("var-global"); // find input target
			_assign_string( dst, input ); // dump into a memory string
			return 0;
		}
		// missing type
		fprintf(stderr, "unknown statement type: %s\n", mystmt.type.c_str());
		mystmt.show();
		exit(1);
	}



	// expressions

	i32 _expr(const Node& expr) {
		if (expr.type == "expr")        return _expr(expr.kids[0]);
		if (expr.type == "number")      return stoi(expr.value);
		if (expr.type == "var-local")   return _stack_val(expr);
		if (expr.type == "var-global")  return _stack_val(expr);
		if (expr.type == "function-call") {
			vector<i32> args;
			for (auto& arg : expr.at("arguments").kids)
				args.push_back( _expr(arg) );
			return call( expr.at("name").value, args );
		}
		// error
		fprintf(stderr, "unknown expr type: %s\n", expr.type.c_str());
		expr.show();
		exit(1);
	}



	// helpers

	string _string_ptr(i32 ptr) {
		string str;
		auto& mem = heap[ptr];
		for (auto v : mem)
			str += char(v);
		return str;
	}

	i32 _stack_val(const Node& var) {
		auto name = var.value;
		if (var.type == "var-local") name = "~"+name;
		if (stack.count(name) == 0)
			fprintf(stderr, "undefined variable: %s\n", name.c_str()), exit(1);
		return stack[name];
	}

	i32 _stack_deref(const Node& var) {
		int loc = _stack_val(var);
		if (loc <= 0 || loc >= heap.size())
			fprintf(stderr, "deref error\n"), exit(1);
		return loc;
	}

	void _assign_string(const Node& var, const string& str) {
		int loc = _stack_deref(var);
		heap[loc] = {};
		for (auto c : str) heap[loc].push_back(c);
		if (str.length() == 0) heap[loc].push_back(0);  // make sure it isn't totally empty, until free() called
	}

}