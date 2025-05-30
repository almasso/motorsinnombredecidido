local Class = require 'data.events.Class'

---@class EventBehaviour
local EventBehaviour = Class:inherit()

---Constructor
---@return EventBehaviour object New instance of the EventBehaviour
function EventBehaviour:new()
    local obj = EventBehaviour:super().new(self)
    obj._done = false;
    return obj;
end

function EventBehaviour:init(scene, entity, event)
    return true;
end

function EventBehaviour:onStart()
    self._done = false;
    return true;
end

function EventBehaviour:act(game, scene, entity, event)
    return true;
end

function EventBehaviour:done()
    return self._done;
end

function EventBehaviour:ended(scene, entity, event)
    return self._done;
end

return EventBehaviour