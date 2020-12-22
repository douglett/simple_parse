/**
 * temporary crap that i'm not sure what to do with
**/ 
#include "parse.hpp"
#include <map>
using namespace std;

namespace parse {

	// check duplicates in list. only values are checked (OK?):
	// 	locals:
	//		dim: a
	//		dim: b
	//		dim: a  -- duplicate
	void check_dup_values(const Node& nlist, const std::string& errname) {
		map<string, int> valcount;
		for (const auto& val : nlist.kids)
			if (++valcount[val.value] > 1) input.die(errname+": duplicate ["+val.value+"]");
	}
}