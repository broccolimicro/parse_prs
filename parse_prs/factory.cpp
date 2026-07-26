#include "factory.h"

#include "production_rule_set.h"

namespace parse_prs {

const parse::factory factory(parse::schema::from<production_rule_set>());

}
