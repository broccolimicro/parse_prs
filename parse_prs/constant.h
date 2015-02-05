/*
 * constant.h
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef prs_lang_constant_h
#define prs_lang_constant_h

namespace prs_lang
{
struct constant : parse::syntax
{
	constant();
	constant(configuration &config, tokenizer &tokens);
	~constant();

	string value;

	void parse(configuration &config, tokenizer &tokens);
	static bool is_next(configuration &config, tokenizer &tokens, int i = 1);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "");
};
}

#endif
