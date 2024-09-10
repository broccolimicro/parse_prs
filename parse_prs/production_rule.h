#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include <parse_expression/expression.h>
#include <parse_expression/assignment.h>

#include "guard.h"

namespace parse_prs
{
struct production_rule : parse::syntax
{
	production_rule();
	production_rule(tokenizer &tokens, void *data = NULL);
	~production_rule();

	parse_expression::expression assume;
	guard implicant;
	parse_expression::assignment action;
	bool weak;
	bool pass;
	bool keep;
	uint64_t after;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};
}

