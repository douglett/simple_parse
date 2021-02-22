#pragma once
#include <string>
#include <vector>

struct Node {
	typedef std::string string;
	std::string type, value;
	std::vector<Node> kids;

	// const accessors
	int count(const string& type) const {
		int c = 0;
		if (type == "") return 0;
		for (auto& n : kids)
			if (n.type == type) c++;
		return c;
	}
	const Node& at(const string& type, const string& value="") const {
		return ((Node*)this)->at(type);
	}

	// mutators
	Node& at(const string& type, const string& value="") {
		if (type == "") fprintf(stderr, "error: empty node key type\n"), exit(1);
		for (auto& n : kids)
			if      (n.type == type && value == "") return n;
			else if (n.type == type && n.value == value) return n;
		fprintf(stderr, "error: missing in node list: %s:%s\n", type.c_str(), value.c_str());
		show();
		exit(1);
	}
	Node& push(const Node& n) {
		kids.push_back(n);
		return kids.back();
	}

	// accessors
	int length() const {
		return kids.size();
	}
	void show(int ind=0) const {
		// printf("%s%s: %s\n", string(ind*2, ' ').c_str(), type.c_str(), value.c_str());
		printf("%s%s%s:%s %s\n", string(ind*2, ' ').c_str(), "\033[33m", type.c_str(), "\033[0m", value.c_str());
		for (auto& n : kids)
			n.show(ind+1);
	}
};