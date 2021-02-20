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
	// i32 _dim(const Node& mydim, int asglobal);
	i32 _stmt_block(const Node& myblock);
	i32 _stmt(const Node& mystmt);
	i32 _expr(const Node& expr);



	// external

	i32 globals() {
		for (const auto& gdim : parse::globals.kids)
			// _dim(gdim, true);
			stack[gdim.value] = _expr( gdim.at("expr") );
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
		// locals.show();
		for (auto& local : locals.kids)
			// _dim(local, false);
			stack["~"+local.value] = _expr( local.at("expr") );

		return _stmt_block( function.at("stmt-block") );
		// exit(1);
	}

	i32 _syscall(const string& name, const vector<i32>& args) {
		// printf("syscall: %s\n", name.c_str());
		// if (name == "ioredirect") { // && args.size() == 2) {
		// 	return 0;
		// }

		if (name == "malloc" && args.size() == 1) {
			heap.push_back( vector<i32>(args[0], 0) );
			return heap.size()-1;
		} 

		fprintf(stderr, "unknown system call: %s:%d\n", name.c_str(), args.size());
		exit(1);
	}



	// func internals

	// i32 _dim(const Node& mydim, int asglobal) {
	// 	stack[mydim.value] = _expr( mydim.at("expr") );
	// 	return 0;
	// }

	i32 _stmt_block(const Node& myblock) {
		for (auto& st : myblock.kids)
			_stmt(st);
		return 0;
	}

	i32 _stmt(const Node& mystmt) {
		// if (mystmt.type == "stmt-block")
		
		if (mystmt.type == "stmt-call") {
			return call( mystmt.at("function-call").at("name").value, {} );
		}
		if (mystmt.type == "stmt-print") {
			string s;
			for (auto &var : mystmt.kids)
				if      (var.type == "string-literal")  s += InputFile::escape_strlit( var.value );
				else if (var.type == "expr")  s += to_string( _expr(var) );
				else    fprintf(stderr, "stmt-print error: %s\n", var.type.c_str()), exit(1);
			printf("%s\n", s.c_str());
			return 0;
		}
		if (mystmt.type == "stmt-input") {
			// mystmt.show();

			// if (mystmt.count("prompt"))
			printf("%s", InputFile::escape_strlit( mystmt.at("prompt").value ).c_str()); // input prompt
			string input;
			getline(cin, input); // get input

			// todo: allocate somewhere
			auto dst = mystmt.count("var-local") ? mystmt.at("var-local") : mystmt.at("var-global");
			int loc = stack[ dst.value ];
			heap[loc] = {};
			for (auto c : input) heap[loc].push_back(c);
			if (input.length() == 0) heap[loc].push_back(0);  // make sure it isn't totally empty, until free() called

			return 0;
		}

		// missing type
		fprintf(stderr, "unknown statement type: %s\n", mystmt.type.c_str());
		mystmt.show();
		exit(1);
	}



	// expressions

	i32 _expr(const Node& expr) {

		if (expr.type == "expr")  return _expr(expr.kids[0]);
		if (expr.type == "number") {
			// printf("[[%d]]\n", stoi(expr.value));
			return stoi(expr.value);
		}
		if (expr.type == "var-local") {
			return stack["~"+expr.value];
		}
		if (expr.type == "var-global") {
			return stack[expr.value];
		}
		if (expr.type == "function-call") {
			vector<i32> args;
			for (auto& arg : expr.at("arguments").kids)
				args.push_back( _expr(arg) );
			return call( expr.at("name").value, args );
		}

		fprintf(stderr, "unknown expr type: %s\n", expr.type.c_str());
		expr.show();
		exit(1);
	}

}