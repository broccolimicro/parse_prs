#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

namespace parse_prs {

struct slice : parse::syntax {
	slice();
	slice(int lo, int hi=-1);
	slice(tokenizer &tokens, std::any data=std::any());
	~slice();

	string lower;
	string upper;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

