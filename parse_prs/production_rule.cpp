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

production_rule::production_rule(tokenizer &tokens, void *data)
{
	debug_name = "production_rule";
	parse(tokens, data);
}

production_rule::~production_rule()
{

}

void production_rule::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect(",");
	tokens.expect(":");

	tokens.increment(true);
	tokens.expect<parse_boolean::assignment>();

	tokens.increment(true);
	tokens.expect("->");

	tokens.increment(true);
	tokens.expect<parse_boolean::guard>();

	if (tokens.decrement(__FILE__, __LINE__, data))
		implicant.parse(tokens, 0, data);

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (actions.size() == 0)
			actions.push_back(vector<parse_boolean::assignment>());
		actions.back().push_back(parse_boolean::assignment(tokens, data));
	}

	while (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (tokens.next() == ":")
			actions.push_back(vector<parse_boolean::assignment>());

		tokens.increment(false);
		tokens.expect(",");
		tokens.expect(":");

		tokens.increment(true);
		tokens.expect<parse_boolean::assignment>();

		if (tokens.decrement(__FILE__, __LINE__, data))
		{
			if (actions.size() == 0)
				actions.push_back(vector<parse_boolean::assignment>());
			actions.back().push_back(parse_boolean::assignment(tokens, data));
		}
	}

	tokens.syntax_end(this);
}

bool production_rule::is_next(tokenizer &tokens, int i, void *data)
{
	return parse_boolean::guard::is_next(tokens, i, data);
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
	for (int i = 0; i < (int)actions.size(); i++)
	{
		if (i != 0)
			result += ":";

		for (int j = 0; j < (int)actions[i].size(); j++)
		{
			if (j != 0)
				result += ",";

			result += actions[i][j].to_string(tab);
		}
	}
	return result;
}
}
