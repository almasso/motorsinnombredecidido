local EventBehaviour = require 'assets.events.EventBehaviour'

--- @class JumpIf
local JumpIf = EventBehaviour:inherit();

function JumpIf:new(target, conditionParams)
    local obj = JumpIf:super().new(self)
    obj._target = target;
    obj._conditionParams = conditionParams;
    return obj;
end

function JumpIf:init(scene, entity, event)
    self._condition = EventConditionFactory.Create(self._conditionParams, scene, entity, event);
    return not (self._condition == nil);
end

function JumpIf:act(game, scene, entity, event)
    print("\nJumpIf act:");
    print(self._conditionParams.type);
    self._done = true;
    if (self._condition:met()) then
        event:jump(self._target);
    end
    return true;
end

return JumpIf;