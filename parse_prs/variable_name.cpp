#include "variable_name.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>

namespace parse_prs {

variable_name::variable_name() {
	debug_name = "wv_variable_name";
}

variable_name::variable_name(std::string net) {
	debug_name = "wv_variable_name";
	valid = true;

	size_t tic = net.rfind('\'');
	if (tic != string::npos) {
		this->region = net.substr(tic+1);
		net = net.substr(0, tic);
	}

	if (not net.empty()) {
		size_t prev = 0u;
		size_t dot = net.find('.', prev);
		while (dot != string::npos and dot < net.size()) {
			names.push_back(net.substr(prev, dot-prev));
			prev = dot+1;
			dot = net.find('.', prev);
		}
		names.push_back(net.substr(prev));
	}
}

variable_name::variable_name(tokenizer &tokens, std::any data)
{
	debug_name = "wv_variable_name";
	parse(tokens, data);
}

variable_name::~variable_name()
{

}

void variable_name::parse(tokenizer &tokens, std::any data)
{
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect("'");

	tokens.increment(false);
	tokens.expect(".");

	tokens.increment(true);
	tokens.expect<member_name>();

	if (tokens.decrement(__FILE__, __LINE__))
		names.push_back(member_name(tokens, data));

	while (tokens.decrement(__FILE__, __LINE__))
	{
		tokens.next();

		tokens.increment(false);
		tokens.expect(".");

		tokens.increment(true);
		tokens.expect<member_name>();

		if (tokens.decrement(__FILE__, __LINE__))
			names.push_back(member_name(tokens, data));
	}

	if (tokens.decrement(__FILE__, __LINE__))
	{
		tokens.next();

		tokens.increment(true);
		tokens.expect<parse::number>();

		if (tokens.decrement(__FILE__, __LINE__))
			region = tokens.next();
	}

	tokens.syntax_end(this);
}

bool variable_name::is_next(tokenizer &tokens, int i, std::any data)
{
	return member_name::is_next(tokens, i, data);
}

void variable_name::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<variable_name>())
	{
		tokens.register_syntax<variable_name>();
		tokens.register_token<parse::symbol>();
		member_name::register_syntax(tokens);
	}
}

string variable_name::to_string(string tab) const
{
	string result;
	if (names.size() > 0)
		result = names[0].to_string(tab);

	for (int i = 1; i < (int)names.size(); i++)
		result += "." + names[i].to_string(tab);

	if (region != "0" && region != "")
		result += "'" + region;
	return result;
}

parse::syntax *variable_name::clone() const
{
	return new variable_name(*this);
}
}
