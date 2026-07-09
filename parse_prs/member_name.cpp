#include "member_name.h"
#include <parse/default/instance.h>

namespace parse_prs {

member_name::member_name() {
	debug_name = "wv_member_name";
}

member_name::member_name(std::string field) {
	debug_name = "wv_member_name";
	valid = true;

	size_t open = name.find('[');
	name = field.substr(0u, open);
	while (open != string::npos and open < field.size()) {
		open += 1;
		size_t close = name.find(']', open);
		slices.push_back(atoi(name.substr(open, close-open).c_str()));
		open = close+1;
	}
}

member_name::member_name(tokenizer &tokens, void *data)
{
	debug_name = "wv_member_name";
	parse(tokens, data);
}

member_name::~member_name()
{

}

void member_name::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect<slice>();

	tokens.increment(true);
	tokens.expect<parse::instance>();

	if (tokens.decrement(__FILE__, __LINE__, data))
		name = tokens.next();

	while (tokens.decrement(__FILE__, __LINE__, data))
	{
		tokens.increment(false);
		tokens.expect<slice>();

		slices.push_back(slice(tokens, data));
	}

	tokens.syntax_end(this);
}

bool member_name::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next<parse::instance>(i);
}

void member_name::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<member_name>())
	{
		tokens.register_syntax<member_name>();
		tokens.register_token<parse::instance>();
		slice::register_syntax(tokens);
	}
}

string member_name::to_string(string tab) const
{
	string result = name;
	for (int i = 0; i < (int)slices.size(); i++)
		result += slices[i].to_string(tab);

	return result;
}

parse::syntax *member_name::clone() const
{
	return new member_name(*this);
}

}
