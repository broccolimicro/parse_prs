#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include <parse_expression/expression.h>
#include <parse_expression/assignment.h>

#include "literal.h"

namespace parse_prs
{

struct term;

struct guard : parse::syntax
{
	guard();
	guard(tokenizer &tokens, int level, bool source, void *data = NULL);
	~guard();

	enum {
		AND = 0,
		OR = 1
	};

	vector<term> terms;
	
	string region;
	int level;

	void parse(tokenizer &tokens, int level, bool source, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

struct term {
	term();
	term(guard sub);
	term(literal ltrl);
	~term();

	guard sub;
	literal ltrl;

	string size;
	string variant;

	guard pchg;
	
	string to_string(string tab = "") const;
};

}

