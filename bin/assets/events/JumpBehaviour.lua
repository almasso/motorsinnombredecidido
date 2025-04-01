local EventBehaviour = require 'assets.events.EventBehaviour'

--- @class Jump
local Jump = EventBehaviour:inherit();

function Jump:new(target)
    local obj = Jump:super().new(self)
    obj._target = target;
    return obj;
end

function Jump:act(game, scene, entity, event)
    print("\nJump act");
    self._done = true;
    event:jump(self._target);
    return true;
end

return Jump;