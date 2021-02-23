#include "node.hpp"
#include "parse.hpp"
#include <iostream>
#include <map>
using namespace std;

namespace runtime1 {
	typedef  uint32_t  i32;
	map<string, i32> stack;
	vector<vector<i32>> heap = { {0} };
	i32 return_flag = 0;

	i32 _syscall(const string& name, const vector<i32>& args);
	i32 _fncall(const Node& function, const vector<i32>& args);
	i32 _dim(const Node& mydim, int local);
	i32 _dim_arg(const Node& mydim, const vector<i32>& args);
	i32 _stmt_block(const Node& myblock);
	i32 _stmt(const Node& mystmt);
	i32 _expr(const Node& expr);
	i32 _die(const string& msg, const Node& n);
	string _ptr_to_string(i32 ptr);
	i32 _string_to_ptr(const string& str, i32 ptr);
	i32 _stack_get(const Node& var);



	// external

	i32 globals() {
		for (const auto& gdim : parse::globals.kids)
			if (gdim.type == "dim")  _dim(gdim, false);
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
		auto& locals = function.at("locals");
		for (auto& local : locals.kids)
			if      (local.type == "dim" && local.count("argument"))  _dim_arg(local, args);
			else if (local.type == "dim")  _dim(local, true);
		i32 val = _stmt_block( function.at("stmt-block") );
		return return_flag = 0, val;
	}

	i32 _syscall(const string& name, const vector<i32>& args) {
		if (name == "malloc" && args.size() == 1) {
			heap.push_back( vector<i32>(args[0], 0) );
			return heap.size()-1;
		} 
		else if (name == "sizeof" && args.size() == 1) {
			// i32 ptr = args[0];
			// if (ptr < 0 || ptr >= heap.size()) _die("bad pointer", {});
			// return heap[ptr].size();
			return heap[args[0]].size();
		}
		// missing syscall
		return _die("unknown system call: " + name + " (argc: " + to_string(args.size()) + ")", {});
	}



	// func internals

	i32 _dim(const Node& mydim, int local) {
		auto name = (local ? "~" : "") + mydim.value;
		if (stack.count(name) != 0)  _die("redim error", mydim); // redim check
		stack[name] = _expr( mydim.at("expr") );
		return 0;
	}

	i32 _dim_arg(const Node& mydim, const vector<i32>& args) {
		auto name = "~" + mydim.value; // argument always local
		auto argloc = stoi(mydim.at("argument").value);
		if (stack.count(name) != 0)  _die("redim error", mydim); // redim check
		if (argloc < 0 || argloc >= args.size())  _die("arg count error", mydim);
		stack[name] = args[argloc];
		return 0;
	}

	i32 _stmt_block(const Node& myblock) {
		if (myblock.type != "stmt-block") _die("expected block", myblock);
		for (auto& st : myblock.kids) {
			i32 val = _stmt(st);
			if (return_flag) return val;
		}
		return 0;
	}

	i32 _stmt(const Node& mystmt) {
		if (mystmt.type == "stmt-call") {
			auto name = mystmt.at("function-call").at("name").value;
			vector<i32> arglist;
			for (auto& arg : mystmt.at("function-call").at("arguments").kids)
				if (arg.type == "expr")  arglist.push_back( _expr(arg) );
			return call( name, arglist );
		}

		else if (mystmt.type == "stmt-print" || mystmt.type == "stmt-print-string") {
			string str;
			for (auto &var : mystmt.at("list").kids)
				if      (var.type == "string-literal")  str += InputFile::escape_strlit( var.value );
				else if (var.type == "$")               str += _ptr_to_string( _expr(var.at(0)) );
				else if (var.type == "expr")            str += to_string( _expr(var) );
				else    _die("unexpected in print", var);
			if    (mystmt.type == "stmt-print")  printf("%s\n", str.c_str()); // dump to console
			else  _string_to_ptr( str, _stack_get(mystmt.at("target").at(0)) ); // print into a memory string
			return 0;
		}

		else if (mystmt.type == "stmt-input") {
			printf("%s", InputFile::escape_strlit( mystmt.at("prompt").value ).c_str()); // input prompt
			string input;
			getline(cin, input); // get input
			_string_to_ptr( input, _stack_get(mystmt.at("target").at(0)) ); // print into a memory string
			return 0;
		}

		else if (mystmt.type == "stmt-while") {
			while ( _expr(mystmt.at("expr")) ) {
				printf("while::\n");
				i32 val = _stmt_block( mystmt.at("stmt-block") );
				if (return_flag) return val;
			}
			return 0;
		}
		
		else if (mystmt.type == "stmt-if") {
			for (auto& ifcase : mystmt.kids)
				if      (ifcase.type  != "if-case")  _die("ifcase error", ifcase);
				else if (ifcase.value == "if"      && _expr(ifcase.at("expr")) )  return _stmt_block(ifcase.at("stmt-block"));
				else if (ifcase.value == "else-if" && _expr(ifcase.at("expr")) )  return _stmt_block(ifcase.at("stmt-block"));
				else if (ifcase.value == "else")                                  return _stmt_block(ifcase.at("stmt-block"));
			return 0;  // no match (ok)
		}

		else if (mystmt.type == "stmt-assign") {
			i32 val = _expr(mystmt.at("expr"));
			if      (mystmt.count("var-global"))  return stack[mystmt.at("var-global").value] = val;
			else if (mystmt.count("var-local"))   return stack["~"+mystmt.at("var-local").value] = val;
			// TODO: heap array
		}

		else if (mystmt.type == "stmt-return") {
			auto result = _expr(mystmt.at("expr"));
			return_flag = 1;
			return result;
		}

		// missing type
		return _die("unknown statement type", mystmt);
	}



	// expressions

	i32 _expr(const Node& expr) {
		if      (expr.type == "expr")        return _expr(expr.at(0));
		else if (expr.type == "number")      return stoi(expr.value);
		else if (expr.type == "var-local")   return _stack_get(expr);
		else if (expr.type == "var-global")  return _stack_get(expr);
		else if (expr.type == "array-access") {
			i32 ptr = _stack_get( expr.at("target").at(0) );
			i32 offset = _expr( expr.at("expr") );
			return heap[ptr][offset]; // TODO: range checking
		}
		else if (expr.type == "function-call") {
			vector<i32> args;
			for (auto& arg : expr.at("arguments").kids)
				if (arg.type == "expr")  args.push_back( _expr(arg) );
			return call( expr.at("name").value, args );
		}
		else if (expr.type == "operator") {
			if      (expr.value == "+" )  return _expr(expr.at(0)) +  _expr(expr.at(1));
			else if (expr.value == "*" )  return _expr(expr.at(0)) *  _expr(expr.at(1));
			else if (expr.value == "||")  return _expr(expr.at(0)) || _expr(expr.at(1));
			else if (expr.value == "!=")  return _expr(expr.at(0)) != _expr(expr.at(1));
			else if (expr.value == "<" )  return _expr(expr.at(0)) <  _expr(expr.at(1));
		}
		return _die("unknown expr type", expr); // error
	}



	// helpers

	i32 _die(const string& msg, const Node& n) {
		fprintf(stdout, "error: %s\n", msg.c_str());
		n.show();
		exit(1);
		return 0;
	}

	string _ptr_to_string(i32 ptr) {
		string str;
		auto& mem = heap[ptr];
		for (auto v : mem)
			if (v == 0)  break;
			else  str += char(v);
		return str;
	}

	i32 _string_to_ptr(const string& str, i32 ptr) {
		auto& mem = heap[ptr];
		mem = {};
		for (auto c : str)
			mem.push_back(c);
		if (mem.size() == 0)  mem.push_back(0); // make sure it isn't totally empty, until free() called
		return ptr;
	}

	i32 _stack_get(const Node& var) {
		auto name = (var.type == "var-local" ? "~" : "") + var.value;
		if (stack.count(name) == 0)   _die("undefined variable", var);
		return stack[name];
	}

	// vector<i32>& _heap_get(i32 ptr) {
		
	// }

}