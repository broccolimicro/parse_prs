/*
 * assignment.h
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>
#include "variable_name.h"

#ifndef prs_lang_assignment_h
#define prs_lang_assignment_h

namespace prs_lang
{
struct assignment : parse::syntax
{
	assignment();
	assignment(configuration &config, tokenizer &tokens);
	~assignment();

	variable_name variable;
	bool value;

	void parse(configuration &config, tokenizer &tokens);
	static bool is_next(configuration &config, tokenizer &tokens, int i = 1);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "");
};
}

#endif
