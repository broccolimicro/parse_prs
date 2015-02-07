/*
 * production_rule.h
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>
#include <parse_boolean/guard.h>
#include <parse_boolean/assignment.h>

#ifndef parse_prs_production_rule_h
#define parse_prs_production_rule_h

namespace parse_prs
{
struct production_rule : parse::syntax
{
	production_rule();
	production_rule(configuration &config, tokenizer &tokens);
	~production_rule();

	parse_boolean::guard implicant;
	vector<parse_boolean::assignment> actions;

	void parse(configuration &config, tokenizer &tokens);
	static bool is_next(configuration &config, tokenizer &tokens, int i = 1);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
};
}

#endif
