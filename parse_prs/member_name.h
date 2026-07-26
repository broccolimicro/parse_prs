#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "slice.h"

#include <common/net.h>

namespace parse_prs {

struct member_name : parse::syntax {
	member_name();
	member_name(std::string field);
	member_name(tokenizer &tokens, std::any data=std::any());
	~member_name();

	string name;
	vector<slice> slices;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

