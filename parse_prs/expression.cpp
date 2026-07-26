#include "expression.h"
#include <parse_expression/precedence.h>
#include <parse_expression/literal.h>
#include <parse/wrapper.h>

namespace parse_prs {

std::shared_ptr<parse_expression::config> expression_config::cfg = 
	std::make_shared<parse_expression::config>(parse_prs::makeExprConfig());

expression_config::expression_config() {
	debug_name = "prs";
}

expression_config::~expression_config() {
}

std::shared_ptr<parse_expression::config> composition_config::cfg = 
	std::make_shared<parse_expression::config>(parse_prs::makeCompConfig());

composition_config::composition_config() {
	debug_name = "prs";
}

composition_config::~composition_config() {
}

parse_expression::config makeExprConfig() {
	using operation_set=parse_expression::operation_set;

	parse_expression::config cfg;
	int CONSTANT = cfg.push<parse_expression::default_constant>("constant");
	int LITERAL = cfg.push<parse_expression::default_literal>("literal");

	cfg.base = {LITERAL, CONSTANT};

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "|", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "&", "");

	cfg.order.push(operation_set::UNARY);
	cfg.order.push_back("~", "", "", "");

	cfg.set_lvalue();

	cfg.order.push(operation_set::MODIFIER);
	cfg.order.push_back("", "'", "", "");

	cfg.order.push(operation_set::MODIFIER);
	cfg.order.push_back("", "(", ",", ")");
	cfg.order.push_back("", ".", "", "");
	cfg.order.push_back("", "[", ":", "]");
	
	cfg.order.push(operation_set::MODIFIER);
	cfg.order.push_back("", "::", "", "");

	return cfg;
}

parse_expression::config makeCompConfig() {
	parse_expression::config cfg;

	int GUARD = cfg.push<parse_expression::guard>("guard");
	int ASSIGN = cfg.push<assignment>("assign");

	cfg.base = {GUARD, ASSIGN};

	using operation_set=parse_expression::operation_set;

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", ":", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", ",", "");

	cfg.set_lvalue();

	return cfg;
}

}
