#include "OnStartCondition.h"

OnStartCondition::OnStartCondition() :
    alreadyMet(false) {
}

bool OnStartCondition::init(sol::table const& params) {
    alreadyMet = false;
    return true;
}

void OnStartCondition::reset() {
    alreadyMet = false;
}

bool OnStartCondition::met() {
    if (alreadyMet)
        return false;
    alreadyMet = true;
    return true;
}

OnStartCondition::~OnStartCondition() = default;

