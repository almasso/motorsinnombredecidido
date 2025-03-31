local EventBehaviour = require 'assets.events.EventBehaviour'

--- @class WaitFor
local WaitFor = EventBehaviour:inherit();

function WaitFor:new(conditionParams)
    local obj = WaitFor:super().new(self)
    obj._conditionParams = conditionParams;
    return obj;
end

function WaitFor:init(scene, entity, event)
    self._condition = EventConditionFactory.Create(self._conditionParams, scene, entity, event);
    return not (self._condition == nil);
end

function WaitFor:act(game, scene, entity, event)
    print("WaitFor act");
    self._done = self._condition:met();
    return true;
end

return WaitFor;