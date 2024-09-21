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
	production_rule_set(tokenizer &tokens, void *data = NULL);
	~production_rule_set();

	vector<production_rule_set> regions;
	vector<production_rule> rules;
	string region;

	vector<string> assume;
	vector<string> require;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};
}

#endif
