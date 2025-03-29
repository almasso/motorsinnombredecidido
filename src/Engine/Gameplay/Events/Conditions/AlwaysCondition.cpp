#include "AlwaysCondition.h"

bool AlwaysCondition::init(sol::table const& params) {
    return true;
}

bool AlwaysCondition::met() {
    return true;
}

