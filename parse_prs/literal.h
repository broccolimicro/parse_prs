#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include <parse_ucs/variable_name.h>

namespace parse_prs
{
using parse_ucs::variable_name;

struct literal : parse::syntax {
	literal();
	literal(variable_name name, bool invert, bool gate=true);
	literal(tokenizer &tokens, bool source, void *data = NULL);
	~literal();

	variable_name name;
	bool gate;
	bool invert;

	void parse(tokenizer &tokens, bool source, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

