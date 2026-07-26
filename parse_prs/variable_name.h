#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "member_name.h"

#include <common/net.h>

namespace parse_prs {

struct variable_name : parse::syntax {
	variable_name();
	variable_name(std::string net);
	variable_name(tokenizer &tokens, std::any data=std::any());
	~variable_name();

	vector<member_name> names;
	string region;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

