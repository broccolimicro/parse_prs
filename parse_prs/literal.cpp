/*
 * literal.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "literal.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>

namespace parse_prs
{
literal::literal()
{
	debug_name = "literal";
	invert = false;
	gate = true;
}

literal::literal(variable_name name, bool invert, bool gate) {
	debug_name = "literal";
	valid = true;
	this->name = name;
	this->invert = invert;
	this->gate = gate;
}

literal::literal(tokenizer &tokens, bool source, void *data)
{
	debug_name = "literal";
	invert = false;
	gate = true;
	parse(tokens, source, data);
}

literal::~literal()
{

}

void literal::parse(tokenizer &tokens, bool source, void *data)
{
	tokens.syntax_start(this);
	
	tokens.increment(true);
	tokens.expect<variable_name>();

	if (source) {
		tokens.increment(false);
		tokens.expect("@");
	}

	tokens.increment(false);
	tokens.expect("~");

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		invert = true;
	}

	if (source and tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		gate = false;
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		name = variable_name(tokens, data);
	}

	tokens.syntax_end(this);
}

bool literal::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("@", i) or tokens.is_next("~", i) or variable_name::is_next(tokens, i, data);
}

void literal::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<literal>())
	{
		tokens.register_syntax<literal>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::number>();
		tokens.register_token<parse::instance>();
		variable_name::register_syntax(tokens);
	}
}

string literal::to_string(string tab) const
{
	string result;
	if (invert) {
		result += "~";
	}

	if (not gate) {
		result += "@";
	}

	result += name.to_string(tab);

	return result;
}

parse::syntax *literal::clone() const
{
	return new literal(*this);
}

}
