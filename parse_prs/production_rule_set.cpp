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

production_rule_set::production_rule_set(tokenizer &tokens, void *data)
{
	debug_name = "production_rule_set";
	parse(tokens, data);
}

production_rule_set::~production_rule_set()
{

}

void production_rule_set::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect<production_rule>();

	while (tokens.decrement(__FILE__, __LINE__, data))
	{
		rules.push_back(production_rule(tokens, data));

		tokens.increment(false);
		tokens.expect<production_rule>();
	}

	tokens.syntax_end(this);
}

bool production_rule_set::is_next(tokenizer &tokens, int i, void *data)
{
	return production_rule::is_next(tokens, i, data);
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

	for (int i = 0; i < (int)rules.size(); i++)
		result += tab + rules[i].to_string(tab) + "\n";

	return result;
}

parse::syntax *production_rule_set::clone() const
{
	return new production_rule_set(*this);
}

}
