/*
 * factory.h
 *
 *  Created on: Aug 16, 2015
 *      Author: nbingham
 */

#include "production_rule_set.h"
#include "production_rule.h"

#ifndef factory_h
#define factory_h

namespace parse_prs
{
	parse::syntax *factory(tokenizer &tokens, void *data)
	{
		return new production_rule_set(tokens, data);
	};
}

#endif
