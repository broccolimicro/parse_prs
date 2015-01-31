/*
 * guard.h
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef guard_h
#define guard_h

namespace prs_lang
{
struct guard : parse::syntax
{
	guard();
	guard(configuration &config, tokenizer &tokens, int i = 0);
	~guard();

	vector<parse::syntax*> operands;
	vector<string> functions;
	int level;

	void parse(configuration &config, tokenizer &tokens, int i = 0);
	static bool is_next(configuration &config, tokenizer &tokens, int i = 1);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "");
};
}

#endif
