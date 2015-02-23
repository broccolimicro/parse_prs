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

	tokens.increment(true);
	tokens.expect<parse_boolean::internal_choice>();

	tokens.increment(true);
	tokens.expect("->");

	tokens.increment(true);
	tokens.expect<parse_boolean::disjunction>();

	if (tokens.decrement(__FILE__, __LINE__, data))
		implicant.parse(tokens, data);

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		action.parse(tokens, data);

	tokens.syntax_end(this);
}

bool production_rule::is_next(tokenizer &tokens, int i, void *data)
{
	return parse_boolean::disjunction::is_next(tokens, i, data);
}

void production_rule::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<production_rule>())
	{
		tokens.register_syntax<production_rule>();
		tokens.register_token<parse::symbol>();
		parse_boolean::disjunction::register_syntax(tokens);
		parse_boolean::internal_choice::register_syntax(tokens);
	}
}

string production_rule::to_string(string tab) const
{
	return implicant.to_string(tab) + "->" + action.to_string(tab);
}

parse::syntax *production_rule::clone() const
{
	return new production_rule(*this);
}

}
