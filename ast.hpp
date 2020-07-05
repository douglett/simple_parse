#pragma once
#include <string>
#include <vector>
using namespace std;

struct AST {

	// AST node inner class
	struct Node {
		string type, value;
		vector<Node> kids;

		Node& at(const string& type) {
			for (auto& n : kids)
				if (n.type == type) return n;
			fprintf(stderr, "error: missing in node list: %s\n", type.c_str());
			exit(1);
		}
		//Node& get(const string& type)
		Node& push(const Node& n) {
			kids.push_back(n);
			return kids.back();
		}
		void show(int ind=0) const {
			printf("%s%s: %s\n", string(ind*2, ' ').c_str(), type.c_str(), value.c_str());
			for (auto& n : kids)
				n.show(ind+1);
		}
	};

	// AST results
	Node astnode;

	// AST builders

	Node make_func(const Func& func) {
		Node ast = { "function", func_name, {
			{ "dims", "", {}},
			{ "statements", "", {}},
		}};
		for (auto& st : statements)
			ast.at("statements").push( make_stmt(st) );
		return ast;
	}

	Node make_stmt(const Stmt& stmt) {
		return { "statement", "TODO" };
	}

} // end AST