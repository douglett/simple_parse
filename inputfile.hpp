#pragma once
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct InputFile {
	vector<string> rawlines;
	vector<vector<string>> lines;
	uint32_t linepos = 0, pos = 0;

	InputFile(const string& name) {
		std::fstream fs(name, ios::in);
		if (!fs.is_open())
			fprintf(stderr, "error opening file: %s\n", name.c_str()), exit(1);

		string s;
		while(std::getline(fs, s)) {
			rawlines.push_back(s);
			parseline(s);
			linepos++; // for errors
		}
		linepos = 0;
		fs.close();

		showrawlines();
		showlines();
	}

	int eof() const {
		return linepos >= rawlines.size();
	}
	int eol() const {
		return pos >= getline().size();
	}
	const vector<string>& getline() const {
		return lines.at(linepos);
	}
	const string& getrawline() const {
		return rawlines.at(linepos);
	}

	int is_identifier() const {
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
		int lno = 0;
		for (auto ln : rawlines) {
			lno++;
			printf("%02d: %s\n", lno, ln.c_str());
		}
	}

	void showlines() const {
		int lno = 0;
		for (auto ln : lines) {
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

private:
	void parseline(const string& ln) {
		vector<string> tokens;
		string tok;
		for (int i = 0; i < (int)ln.length(); i++) {
			auto c = ln[i];
			if      (isspace(c)) addtok(tokens, tok);
			else if (isalnum(c)) tok += c;
			else if (c == '#') break; // omit line comments
			else if (c == '"') addtok(tokens, tok), tok = getstrlit(ln, i), addtok(tokens, tok);
			else    addtok(tokens, tok), tok = string(c, 1), addtok(tokens, tok);
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

	int addtok(vector<string>& tokens, string& tok) {
		if (tok.length()) tokens.push_back(tok);
		tok = "";
		return 0;
	}
};