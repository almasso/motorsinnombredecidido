local EventBehaviour = require 'assets.events.EventBehaviour'

--- @class Move
local Move = EventBehaviour:inherit();

function Move:act(game, scene, entity, event)
    print("MoveBehaviour act");
    self._done = true;
    self._ended = false;
    return true;
end

function Move:updateEnd(scene, entity, event)
    self._ended = true;
    print("MoveBehaviour end");
end

return Move;