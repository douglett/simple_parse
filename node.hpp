#pragma once
#include <string>
#include <vector>

struct Node {
	typedef std::string string;
	std::string type, value;
	std::vector<Node> kids;

	Node& at(const string& type) {
		if (type == "") fprintf(stderr, "error: empty node key type\n"), exit(1);
		for (auto& n : kids)
			if (n.type == type) return n;
		fprintf(stderr, "error: missing in node list: %s\n", type.c_str()), exit(1);
	}
	// Node& get(const string& type) {
	// 	if (type == "") fprintf(stderr, "error: empty node key type\n"), exit(1);
	// 	for (auto& n : kids)
	// 		if (n.type == type) return n;
	// 	return kids.push_back({ type }), kids.back();
	// }
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