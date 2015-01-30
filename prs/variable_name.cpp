/*
 * variable_name.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "variable_name.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace prs
{
variable_name::variable_name()
{
	debug_name = "variable_name";
}

variable_name::variable_name(configuration &config, tokenizer &tokens)
{
	debug_name = "variable_name";
	parse(config, tokens);
}

variable_name::~variable_name()
{

}

void variable_name::parse(configuration &config, tokenizer &tokens)
{
	valid = true;

	tokens.increment(false);
	tokens.expect(".");

	tokens.increment(true);
	tokens.expect<parse::instance>();

	if (tokens.decrement(config, __FILE__, __LINE__))
		names.push_back(tokens.next());

	while (tokens.decrement(config, __FILE__, __LINE__))
	{
		tokens.next();

		tokens.increment(false);
		tokens.expect(".");

		tokens.increment(true);
		tokens.expect<parse::instance>();

		if (tokens.decrement(config, __FILE__, __LINE__))
			names.push_back(tokens.next());
	}
}

bool variable_name::is_next(configuration &config, tokenizer &tokens, int i)
{
	return tokens.is_next<parse::instance>(i);
}

void variable_name::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<variable_name>())
	{
		tokens.register_syntax<variable_name>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::symbol>();
	}
}

string variable_name::to_string(string tab)
{
	string result;
	if (names.size() > 0)
		result = names[0];

	for (int i = 1; i < (int)names.size(); i++)
		result += "." + names[i];
	return result;
}
}
