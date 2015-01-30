/*
 * production_rule.h
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>
#include "guard.h"
#include "assignment.h"

#ifndef production_rule_h
#define production_rule_h

namespace prs
{
struct production_rule : parse::syntax
{
	production_rule();
	production_rule(configuration &config, tokenizer &tokens);
	~production_rule();

	guard implicant;
	vector<assignment> actions;

	void parse(configuration &config, tokenizer &tokens);
	static bool is_next(configuration &config, tokenizer &tokens, int i = 1);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "");
};
}

#endif
