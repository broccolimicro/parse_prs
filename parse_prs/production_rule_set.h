/*
 * production_rule_set.h
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>
#include "production_rule.h"

#ifndef parse_prs_production_rule_set_h
#define parse_prs_production_rule_set_h

namespace parse_prs
{
struct production_rule_set : parse::syntax
{
	production_rule_set();
	production_rule_set(configuration &config, tokenizer &tokens);
	~production_rule_set();

	vector<production_rule> prs;

	void parse(configuration &config, tokenizer &tokens);
	static bool is_next(configuration &config, tokenizer &tokens, int i = 1);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
};
}

#endif