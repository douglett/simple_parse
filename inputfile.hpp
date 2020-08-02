#pragma once
#include <string>
#include <vector>
#include <fstream>

struct InputFile {
	typedef std::string string;
	typedef std::vector<std::string> vecstr;
	vecstr rawlines;
	std::vector<vecstr> lines;
	uint32_t linepos = 0, pos = 0;

	InputFile() { }
	InputFile(const string& name) { load(name); }

	// initialising InputFile with source file
	int load(const string& name) {
		clearall();
		std::fstream fs(name, std::ios::in);
		if (!fs.is_open())
			// fprintf(stderr, "error opening file: %s\n", name.c_str()), exit(1);
			return fprintf(stderr, "error opening file: %s\n", name.c_str()), 1;

		string s;
		while(std::getline(fs, s)) {
			rawlines.push_back(s);
			parseline(s);
			linepos++; // for errors
		}
		linepos = 0;
		fs.close();

		//showrawlines();
		//showlines();
		return 0;
	}
	void clearall() {
		lines = {};
		rawlines = {};
		linepos = pos = 0;
	}
	void seekline(int lno) {
		linepos = lno < 0 || lno > (int)lines.size() ? lines.size() : lno;
		pos = 0;
	}

	// parsing
	int eof() const {
		return linepos >= rawlines.size();
	}
	int eol() const {
		return eof() || pos >= getline().size();
	}
	const vecstr& getline() const {
		return lines.at(linepos);
	}
	const string& getrawline() const {
		return rawlines.at(linepos);
	}

	int is_keyword() const {
		static const vecstr KEYWORDS = {
			"function", "end", "dim", "print" };
		auto t = peek();
		for (auto& k : KEYWORDS)
			if (t == k) return 1;
		return 0;
	};
	int is_identifier() const {
		if (is_keyword()) return 0;
		auto t = peek();
		if (t.length() == 0 || !isalpha(t[0])) return 0;
		for (auto c : t)
			if (!isalnum(c)) return 0;
		return 1;
	}
	int is_strlit() const {
		auto t = peek();
		return t.length() >= 2 && t[0] == '"' && t.back() == '"';
	}
	int is_integer() const {
		auto t = peek();
		if (t.length() == 0) return 0;
		for (auto c : t)
			if (c < '0' || c > '9') return 0;
		return 1;
	}

	string peek() const {
		if (eof()) return "<EOF>";
		if (eol()) return "<EOL>";
		return getline().at(pos);
	}
	string peeklower() const {
		auto s = peek();
		if (eof() || eol()) return s;
		for (auto& c : s) c = tolower(c);
		return s;
	}
	int next() {
		return ++pos, eol();
	}
	int nextline() {
		return ++linepos, pos=0, eof();
	}

	void showrawlines() const {
		printf("::source-raw-lines::\n");
		int lno = 0;
		for (auto ln : rawlines) {
			lno++;
			printf("%02d: %s\n", lno, ln.c_str());
		}
	}

	void showlines() const {
		printf("::source-lines::\n");
		int lno = 0;
		for (auto& ln : lines) {
			lno++;
			printf("%02d: ", lno);
			for (auto tok : ln)
				printf("[%s] ", tok.c_str());
			printf("\n");
		}
	}

	void die(const string& msg="") {
		fprintf(stderr, "SYNTAX ERROR\n");
		if (msg.length())
		fprintf(stderr, "    %s\n", msg.c_str());
		fprintf(stderr, "    at line %d\n", linepos+1);
		exit(1);
	}

	static string escape_strlit(const string& str) {
		if (str.length() >= 2 && str.front() == '"' && str.back() == '"')
			return str.substr(1, str.length()-2);
		return str;
	}

private:
	void parseline(const string& ln) {
		vecstr tokens;
		string tok;
		for (int i = 0; i < (int)ln.length(); i++) {
			auto c = ln[i];
			if      (isspace(c)) addtok(tokens, tok);
			else if (isalnum(c)) tok += c;
			else if (c == '#') break; // omit line comments
			else if (c == '"') addtok(tokens, tok), tok = getstrlit(ln, i), addtok(tokens, tok);
			else    addtok(tokens, tok), tok = string(1, c), addtok(tokens, tok);
		}
		addtok(tokens, tok);
		lines.push_back(tokens);
	}

	string getstrlit(const string& ln, int& i) {
		string tok = "\"";
		while (++i) {
			if (i >= (int)ln.length()) die("string termination error");
			tok += ln[i];
			if (ln[i] == '"') break;
		}
		return tok;
	}

	int addtok(vecstr& tokens, string& tok) {
		if (tok.length()) tokens.push_back(tok);
		tok = "";
		return 0;
	}
};