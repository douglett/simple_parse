#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;

struct ProgramStack {
	typedef  map<string, int>  StackFrame;
	StackFrame globals;
	vector<StackFrame> stack;

	void dim(const string& id, int val=0) {
		auto& frame = stack.size() ? stack.back() : globals;
		frame[id] = val;
	}
	int exists(const string& id) {
		if      (stack.size() && stack.back().count(id)) return 2;
		else if (globals.count(id)) return 1;
		return 0;
	}
	int& get(const string& id) {
		if      (exists(id) == 2) return stack.back()[id];
		else if (exists(id) == 1) return globals[id];
		else    fprintf(stderr, "error: missing in stack: %s\n", id.c_str()), exit(1);
	}
	int& set(const string& id, int val) {
		return get(id) = val;
	}
	int push() {
		return stack.push_back({}), stack.size();
	}
	int pop() {
		if (stack.size() == 0) return 0;
		return stack.pop_back(), stack.size();
	}
	int reset() {
		return globals = {}, stack = {}, 0;
	}
};