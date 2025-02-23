#include "factory.h"

#include "production_rule_set.h"

namespace parse_prs {

parse::syntax *produce(tokenizer &tokens, void *data) {
	return new production_rule_set(tokens, data);
}

void expect(tokenizer &tokens) {
	tokens.expect<production_rule_set>();
}

void register_syntax(tokenizer &tokens) {
	production_rule_set::register_syntax(tokens);
}

}
