/*
 * production_rule_set.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "production_rule_set.h"
#include <parse/default/number.h>
#include <parse/default/symbol.h>
#include <parse/default/new_line.h>

namespace parse_prs
{
production_rule_set::production_rule_set()
{
	debug_name = "production_rule_set";
}

production_rule_set::production_rule_set(tokenizer &tokens, void *data)
{
	debug_name = "production_rule_set";
	parse(tokens, data);
}

production_rule_set::~production_rule_set()
{

}

void production_rule_set::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect("{");
	tokens.expect<production_rule>();
	tokens.expect<parse::new_line>();

	const int REQUIRE = 0;
	const int ASSUME = 1;

	bool found = true;
	while (found) {
		found = false;
		tokens.increment(false);
		tokens.expect("require");
		tokens.expect("assume");
		tokens.expect<parse::new_line>();

		int constraint = -1;
		if (tokens.decrement(__FILE__, __LINE__, data)) {
			found = true;
			string value = tokens.next();
			if (value == "require") {
				constraint = REQUIRE;
			} else if (value == "assume") {
				constraint = ASSUME;
			} else {
				continue;
			}

			tokens.increment(true);
			tokens.expect<parse::new_line>();

			bool first = true;
			do {
				if (not first) {
					tokens.next();
				}
				first = false;

				tokens.increment(false);
				tokens.expect(",");

				tokens.increment(true);
				if (constraint == REQUIRE) {
					tokens.expect("driven");
					tokens.expect("stable");
					tokens.expect("noninterfering");
					tokens.expect("adiabatic");
				} else if (constraint == ASSUME) {
					tokens.expect("nobackflow");
					tokens.expect("static");
				}

				if (tokens.decrement(__FILE__, __LINE__, data)) {
					if (constraint == REQUIRE) {
						require.push_back(tokens.next());
					} else if (constraint == ASSUME) {
						assume.push_back(tokens.next());
					}
				}
			} while (tokens.decrement(__FILE__, __LINE__, data));
			
			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();
			}
		}
	}

	while (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (tokens.found("{")) {
			tokens.next();

			tokens.increment(true);
			tokens.expect<parse::number>();

			tokens.increment(true);
			tokens.expect("'");

			tokens.increment(true);
			tokens.expect("}");

			tokens.increment(true);
			tokens.expect<production_rule_set>();

			bool regionadded = false;
			if (tokens.decrement(__FILE__, __LINE__, data))
			{
				regions.push_back(production_rule_set(tokens, data));
				regionadded = true;
			}

			if (tokens.decrement(__FILE__, __LINE__, data))
				tokens.next();

			if (tokens.decrement(__FILE__, __LINE__, data))
				tokens.next();

			if (tokens.decrement(__FILE__, __LINE__, data) && regionadded)
				regions.back().region = tokens.next();

		} else if (tokens.found<parse::new_line>()) {
			tokens.next();
		} else {
			rules.push_back(production_rule(tokens, data));
		}

		tokens.increment(false);
		tokens.expect("{");
		tokens.expect<production_rule>();
		tokens.expect<parse::new_line>();
	}

	tokens.syntax_end(this);
}

bool production_rule_set::is_next(tokenizer &tokens, int i, void *data)
{
	while (tokens.is_next<parse::new_line>(i)) {
		i++;
	}

	return production_rule::is_next(tokens, i, data) || tokens.is_next("{");
}

void production_rule_set::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<production_rule_set>())
	{
		tokens.register_syntax<production_rule_set>();
		tokens.register_token<parse::number>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::new_line>(true);
		production_rule::register_syntax(tokens);
	}
}

string production_rule_set::to_string(string tab) const
{
	string result = "";

	if (not assume.empty()) {
		result += "assume ";
		for (auto i = assume.begin(); i != assume.end(); i++) {
			if (i != assume.begin()) {
				result += ", ";
			}
			result += *i;
		}
		result += "\n";
	}

	if (not require.empty()) {
		result += "require ";
		for (auto i = require.begin(); i != require.end(); i++) {
			if (i != require.begin()) {
				result += ", ";
			}
			result += *i;
		}
		result += "\n";
	}

	for (int i = 0; i < (int)regions.size(); i++)
		result += tab + regions[i].to_string(tab) + "\n";

	for (int i = 0; i < (int)rules.size(); i++)
		result += tab + rules[i].to_string(tab) + "\n";

	if (region != "")
		result = "{" + result + "}'" + region;

	return result;
}

parse::syntax *production_rule_set::clone() const
{
	return new production_rule_set(*this);
}

}
