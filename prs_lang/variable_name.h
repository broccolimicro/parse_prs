/*
 * variable_name.h
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef variable_name_h
#define variable_name_h

namespace prs_lang
{
struct variable_name : parse::syntax
{
	variable_name();
	variable_name(configuration &config, tokenizer &tokens);
	~variable_name();

	vector<string> names;

	void parse(configuration &config, tokenizer &tokens);
	static bool is_next(configuration &config, tokenizer &tokens, int i = 1);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "");
};
}

#endif
