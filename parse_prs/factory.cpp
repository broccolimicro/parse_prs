#include "factory.h"

#include "production_rule_set.h"

namespace parse_prs {

parse::syntax *produce(tokenizer &tokens, void *data) {
	return new production_rule_set(tokens, data);
}

void expect(tokenizer &tokens) {
	tokens.expect<production_rule_set>();
}

void setup_expressions() {
	parse_expression::precedence_set result;
	result.push(parse_expression::operation_set::BINARY);
	result.push_back("", "", "|", "");

	result.push(parse_expression::operation_set::BINARY);
	result.push_back("", "", "&", "");

	result.push(parse_expression::operation_set::UNARY);
	result.push_back("~", "", "", "");

	result.push(parse_expression::operation_set::MODIFIER);
	result.push_back("", "'", "", "");

	result.push(parse_expression::operation_set::MODIFIER);
	result.push_back("", "(", ",", ")");
	result.push_back("", ".", "", "");
	result.push_back("", "[", ":", "]");
	
	result.push(parse_expression::operation_set::MODIFIER);
	result.push_back("", "::", "", "");
	parse_expression::expression::register_precedence(result);
	parse_expression::assignment::lvalueLevel = 3;
}

void register_syntax(tokenizer &tokens) {
	setup_expressions();
	production_rule_set::register_syntax(tokens);
}

}
