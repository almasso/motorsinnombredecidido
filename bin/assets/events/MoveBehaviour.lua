local EventBehaviour = require 'assets.events.EventBehaviour'

--- @class Move
local Move = EventBehaviour:inherit();

function Move:act(game, scene, entity, event)
    print("MoveBehaviour act");
    self._done = true;
    self._ended = true;
    return true;
end

return Move;