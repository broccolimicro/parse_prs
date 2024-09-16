/*
 * guard.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "guard.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>

namespace parse_prs
{
guard::guard()
{
	debug_name = "guard";
	this->level = OR;
}

guard::guard(tokenizer &tokens, int level, bool source, void *data)
{
	debug_name = "guard";
	parse(tokens, level, source, data);
}

guard::~guard()
{

}

void guard::parse(tokenizer &tokens, int level, bool source, void *data)
{
	this->level = level;
	tokens.syntax_start(this);

	tokens.increment(false);
	if (level == OR) {
		tokens.expect("|");
	} else {
		tokens.expect("&");
	}

	tokens.increment(true);
	if (level == OR) {
		tokens.expect<guard>();
	} else {
		tokens.expect<literal>();
		tokens.expect("(");
	}

	guard pchg;
	while (true) {
		if (tokens.decrement(__FILE__, __LINE__, data)) {
			bool sizing = false;
			if (tokens.found("(")) {
				sizing = true;
				tokens.next();

				tokens.increment(false);
				tokens.expect("<");

				tokens.increment(false);
				tokens.expect("'");

				tokens.increment(true);
				tokens.expect(")");

				tokens.increment(true);
				tokens.expect<guard>();

				if (tokens.decrement(__FILE__, __LINE__, data)) {
					terms.push_back(term(guard(tokens, OR, source, data)));
				}

				if (tokens.decrement(__FILE__, __LINE__, data)) {
					tokens.next();
				}

				if (tokens.decrement(__FILE__, __LINE__, data))
				{
					tokens.next();

					tokens.increment(true);
					tokens.expect<parse::number>();

					if (tokens.decrement(__FILE__, __LINE__, data))
						terms.back().sub.region = tokens.next();
				}
				source = false;
			} else if (tokens.found<literal>()) {
				sizing = true;
				tokens.increment(false);
				tokens.expect("<");

				terms.push_back(term(literal(tokens, source, data)));
				source = false;
			} else if (tokens.found<guard>()) {
				terms.push_back(term(guard(tokens, AND, source, data)));
			}

			if (pchg.valid) {
				terms.back().pchg = pchg;
				pchg = guard();
			}

			if (sizing) {
				if (tokens.decrement(__FILE__, __LINE__, data)) {
					tokens.next();

					tokens.increment(true);
					tokens.expect(">");

					tokens.increment(false);
					tokens.expect(",");

					tokens.increment(true);
					tokens.expect<parse::number>();

					if (tokens.decrement(__FILE__, __LINE__, data)) {
						terms.back().size = tokens.next();
					}

					if (tokens.decrement(__FILE__, __LINE__, data)) {
						tokens.next();

						tokens.increment(true);
						tokens.expect<parse::instance>();

						if (tokens.decrement(__FILE__, __LINE__, data)) {
							terms.back().variant = tokens.next();
						}
					}

					if (tokens.decrement(__FILE__, __LINE__, data)) {
						tokens.next();
					}
				}
			}
		}

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();

			if (level == AND) {
				tokens.increment(false);
				tokens.expect("{");

				if (tokens.decrement(__FILE__, __LINE__, data)) {
					tokens.next();

					tokens.increment(true);
					tokens.expect("}");

					tokens.increment(true);
					tokens.expect<guard>();

					if (tokens.decrement(__FILE__, __LINE__, data)) {
						pchg.parse(tokens, OR, true, data);
					}
					
					if (tokens.decrement(__FILE__, __LINE__, data)) {
						tokens.next();
					}
				}
			}

			tokens.increment(false);
			if (level == OR) {
				tokens.expect("|");
			} else {
				tokens.expect("&");
			}

			tokens.increment(true);
			if (level == OR) {
				tokens.expect<guard>();
			} else {
				tokens.expect<literal>();
				tokens.expect("(");
			}
		} else {
			break;
		}
	}

	tokens.syntax_end(this);
}

bool guard::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("(", i) or literal::is_next(tokens, i, data);
}

void guard::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<guard>())
	{
		tokens.register_syntax<guard>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::number>();
		tokens.register_token<parse::white_space>(false);
		literal::register_syntax(tokens);
	}
}

string guard::to_string(string tab) const
{
	string result;
	for (int i = 0; i < (int)terms.size(); i++) {
		if (i != 0) {
			if (level == OR) {
				result += "|";
			} else {
				result += "&";
				if (terms[i].pchg.valid) {
					result += "{" + terms[i].pchg.to_string(tab) + "}";
				}
			}
		}

		result += terms[i].to_string(tab);
	}
	return result;
}

parse::syntax *guard::clone() const
{
	return new guard(*this);
}

term::term() {
}

term::term(guard sub) {
	this->sub = sub;
}

term::term(literal ltrl) {
	this->ltrl = ltrl;
}

term::~term() {
}

string term::to_string(string tab) const
{
	string result;
	if (sub.valid) {
		if (sub.level == guard::AND) {
			result += sub.to_string(tab);
		} else {
			result += "(" + sub.to_string(tab) + ")";
			if (sub.region != "") {
				result += "'" + sub.region;
			}
		}
	} else if (ltrl.valid) {
		result += ltrl.to_string(tab);
	}

	if (size != "") {
		result += "<" + size;
		if (variant != "") {
			result += "," + variant;
		}
		result += ">";
	}
	return result;
}

}
