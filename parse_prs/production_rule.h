/*
 * production_rule.h
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>
#include <parse_expression/expression.h>
#include <parse_expression/composition.h>

#ifndef parse_prs_production_rule_h
#define parse_prs_production_rule_h

namespace parse_prs
{
struct production_rule : parse::syntax
{
	production_rule();
	production_rule(tokenizer &tokens, void *data = NULL);
	~production_rule();

	parse_expression::expression assume;
	parse_expression::expression implicant;
	parse_expression::composition action;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};
}

#endif
