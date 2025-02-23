/*
 * production_rule.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "production_rule.h"
#include <parse/default/instance.h>
#include <parse/default/number.h>
#include <parse/default/symbol.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

#include <string>
#include <limits>

using namespace std;

namespace parse_prs
{
production_rule::production_rule()
{
	debug_name = "production_rule";
	weak = false;
	force = false;
	pass = false;
	keep = false;
	after = std::numeric_limits<uint64_t>::max();
}

production_rule::production_rule(tokenizer &tokens, void *data)
{
	debug_name = "production_rule";
	weak = false;
	force = false;
	pass = false;
	keep = false;
	after = std::numeric_limits<uint64_t>::max();
	parse(tokens, data);
}

production_rule::~production_rule()
{

}

void production_rule::parse(tokenizer &tokens, void *data)
{
	weak = false;
	force = false;
	pass = false;
	keep = false;
	after = std::numeric_limits<uint64_t>::max();

	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect<parse::new_line>();

	tokens.increment(false);
	tokens.expect("[");

	tokens.increment(false);
	tokens.expect("{");

	tokens.increment(true);
	tokens.expect<parse_expression::assignment>();

	tokens.increment(true);
	tokens.expect("->");

	tokens.increment(true);
	tokens.expect<parse_prs::guard>();

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		implicant.parse(tokens, guard::OR, true, data);
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		action.parse(tokens, data);
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(true);
		tokens.expect("}");

		tokens.increment(true);
		tokens.expect<parse_expression::expression>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			assume.parse(tokens, data);
		}

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
		}
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(true);
		tokens.expect("]");

		bool first = true;
		do {
			if (first) {
				first = false;
			} else {
				tokens.next();
			}

			tokens.increment(false);
			tokens.expect(",");

			tokens.increment(true);
			tokens.expect("keep");
			tokens.expect("weak");
			tokens.expect("force");
			tokens.expect("pass");
			tokens.expect("after");

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				string value = tokens.next();
				if (value == "keep") {
					keep = true;
				}	else if (value == "weak") {
					weak = true;
				}	else if (value == "force") {
					force = true;
				} else if (value == "pass") {
					pass = true;
				} else if (value == "after") {
					tokens.increment(true);
					tokens.expect<parse::number>();
					tokens.increment(true);
					tokens.expect("=");
					if (tokens.decrement(__FILE__, __LINE__, data)) {
						tokens.next();
					}
					if (tokens.decrement(__FILE__, __LINE__, data)) {
						after = stoull(tokens.next());
					}
				}
			}
		} while (tokens.decrement(__FILE__, __LINE__, data));

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
		}
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	tokens.syntax_end(this);
}

bool production_rule::is_next(tokenizer &tokens, int i, void *data)
{
	return parse_prs::guard::is_next(tokens, i, data);
}

void production_rule::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<production_rule>())
	{
		tokens.register_syntax<production_rule>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::number>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		guard::register_syntax(tokens);
		parse_expression::expression::register_syntax(tokens);
		parse_expression::assignment::register_syntax(tokens);
	}
}

string production_rule::to_string(string tab) const
{
	string result = implicant.to_string(tab) + "->" + action.to_string(tab);
	if (assume.valid) {
		result += " {" + assume.to_string(tab) + "}";
	}

	if (keep or weak or force or pass or after != std::numeric_limits<uint64_t>::max()) {
		bool comma=true;
		result += " [";
		if (keep) {
			result += "keep";
			comma=false;
		}
		if (weak) {
			if (not comma) {
				result += ",";
			}
			result += "weak";
			comma=false;
		}
		if (force) {
			if (not comma) {
				result += ",";
			}
			result += "force";
			comma=false;
		}
		if (pass) {
			if (not comma) {
				result += ",";
			}
			result += "pass";
			comma=false;
		}
		if (after != std::numeric_limits<uint64_t>::max()) {
			if (not comma) {
				result += ",";
			}
			result += "after=";
			result += ::to_string(after);
			comma=false;
		}

		result += "]";
	}

	return result;
}

parse::syntax *production_rule::clone() const
{
	return new production_rule(*this);
}

}
