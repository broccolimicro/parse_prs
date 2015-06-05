/*
 * production_rule_set.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "production_rule_set.h"
#include <parse/default/number.h>
#include <parse/default/symbol.h>

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
	tokens.expect("{");
	tokens.expect<production_rule>();

	while (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (tokens.found("{"))
		{
			tokens.next();

			tokens.increment(true);
			tokens.expect<parse::number>();

			tokens.increment(true);
			tokens.expect("'");

			tokens.increment(true);
			tokens.expect("}");

			tokens.increment(true);
			tokens.expect<production_rule_set>();

			bool regionadded = false;
			if (tokens.decrement(__FILE__, __LINE__, data))
			{
				regions.push_back(production_rule_set(tokens, data));
				regionadded = true;
			}

			if (tokens.decrement(__FILE__, __LINE__, data))
				tokens.next();

			if (tokens.decrement(__FILE__, __LINE__, data))
				tokens.next();

			if (tokens.decrement(__FILE__, __LINE__, data) && regionadded)
				regions.back().region = tokens.next();

		}
		else
			rules.push_back(production_rule(tokens, data));

		tokens.increment(false);
		tokens.expect("{");
		tokens.expect<production_rule>();
	}

	tokens.syntax_end(this);
}

bool production_rule_set::is_next(tokenizer &tokens, int i, void *data)
{
	return production_rule::is_next(tokens, i, data) || tokens.is_next("{");
}

void production_rule_set::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<production_rule_set>())
	{
		tokens.register_syntax<production_rule_set>();
		tokens.register_token<parse::number>();
		tokens.register_token<parse::symbol>();
		production_rule::register_syntax(tokens);
	}
}

string production_rule_set::to_string(string tab) const
{
	string result = "";

	for (int i = 0; i < (int)regions.size(); i++)
		result += tab + regions[i].to_string(tab) + "\n";

	for (int i = 0; i < (int)rules.size(); i++)
		result += tab + rules[i].to_string(tab) + "\n";

	if (region != "")
		result = "{" + result + "}'" + region;

	return result;
}

parse::syntax *production_rule_set::clone() const
{
	return new production_rule_set(*this);
}

}
