local Class = require 'assets.events.Class'

---@class EventBehaviour
local EventBehaviour = Class:inherit()

---Constructor
---@return EventBehaviour object New instance of the EventBehaviour
function EventBehaviour:new()
    local obj = EventBehaviour:super().new(self)
    obj._ended = false;
    obj._done = false;
    return obj;
end

function EventBehaviour:act(game, scene, entity, event)
    return true;
end

function EventBehaviour:done()
    return self._done;
end

function EventBehaviour:ended()
    return self._ended;
end

return EventBehaviour