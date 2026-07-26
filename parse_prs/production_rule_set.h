#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "production_rule.h"

namespace parse_prs
{
struct production_rule_set : parse::syntax
{
	production_rule_set();
	production_rule_set(tokenizer &tokens, std::any data=std::any());
	~production_rule_set();

	vector<production_rule_set> regions;
	vector<production_rule> rules;
	string region;

	vector<string> assume;
	vector<string> require;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};
}

