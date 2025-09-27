#pragma once

#include <parse_expression/expression.h>
#include <parse_expression/assignment.h>
#include <parse_expression/composition.h>

namespace parse_prs {

static const size_t expr_group = 4;

using expression=parse_expression::expression_t<expr_group>;
using assignment=parse_expression::assignment_t<expr_group>;
using composition=parse_expression::composition_t<expr_group>;

void setup_expressions();

}
