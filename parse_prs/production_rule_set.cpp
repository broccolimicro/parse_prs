/*
 * production_rule_set.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "production_rule_set.h"

namespace parse_prs
{
production_rule_set::production_rule_set()
{
	debug_name = "production_rule_set";
}

production_rule_set::production_rule_set(configuration &config, tokenizer &tokens)
{
	debug_name = "production_rule_set";
	parse(config, tokens);
}

production_rule_set::~production_rule_set()
{

}

void production_rule_set::parse(configuration &config, tokenizer &tokens)
{
	valid = true;

	tokens.increment(false);
	tokens.expect<production_rule>();

	while (tokens.decrement(config, __FILE__, __LINE__))
	{
		prs.push_back(production_rule(config, tokens));

		tokens.increment(false);
		tokens.expect<production_rule>();
	}
}

bool production_rule_set::is_next(configuration &config, tokenizer &tokens, int i)
{
	return production_rule::is_next(config, tokens, i);
}

void production_rule_set::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<production_rule_set>())
	{
		tokens.register_syntax<production_rule_set>();
		production_rule::register_syntax(tokens);
	}
}

string production_rule_set::to_string(string tab) const
{
	string result = "";

	for (int i = 0; i < (int)prs.size(); i++)
		result += tab + prs[i].to_string(tab) + "\n";

	return result;
}
}
