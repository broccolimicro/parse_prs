/*
 * production_rule.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "production_rule.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace parse_prs
{
production_rule::production_rule()
{
	debug_name = "production_rule";
}

production_rule::production_rule(configuration &config, tokenizer &tokens)
{
	debug_name = "production_rule";
	parse(config, tokens);
}

production_rule::~production_rule()
{

}

void production_rule::parse(configuration &config, tokenizer &tokens)
{
	valid = true;

	tokens.increment(false);
	tokens.expect(",");

	tokens.increment(true);
	tokens.expect<parse_boolean::assignment>();

	tokens.increment(true);
	tokens.expect("->");

	tokens.increment(true);
	tokens.expect<parse_boolean::guard>();

	if (tokens.decrement(config, __FILE__, __LINE__))
		implicant.parse(config, tokens);

	if (tokens.decrement(config, __FILE__, __LINE__))
		tokens.next();

	if (tokens.decrement(config, __FILE__, __LINE__))
		actions.push_back(parse_boolean::assignment(config, tokens));

	while (tokens.decrement(config, __FILE__, __LINE__))
	{
		tokens.next();

		tokens.increment(false);
		tokens.expect(",");

		tokens.increment(true);
		tokens.expect<parse_boolean::assignment>();

		if (tokens.decrement(config, __FILE__, __LINE__))
			actions.push_back(parse_boolean::assignment(config, tokens));
	}
}

bool production_rule::is_next(configuration &config, tokenizer &tokens, int i)
{
	return parse_boolean::guard::is_next(config, tokens, i);
}

void production_rule::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<production_rule>())
	{
		tokens.register_syntax<production_rule>();
		tokens.register_token<parse::symbol>();
		parse_boolean::guard::register_syntax(tokens);
		parse_boolean::assignment::register_syntax(tokens);
	}
}

string production_rule::to_string(string tab) const
{
	string result = implicant.to_string(tab) + "->";
	if (actions.size() > 0)
		result += actions[0].to_string(tab);

	for (int i = 1; i < (int)actions.size(); i++)
		result += "," + actions[i].to_string(tab);

	return result;
}
}
