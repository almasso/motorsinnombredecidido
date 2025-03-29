local EventBehaviour = require 'assets.events.EventBehaviour'

--- @class Move
local Move = EventBehaviour:inherit();

function Move:act(game, scene, entity, event)
    print("MoveBehaviour act");
    self._done = true;
    return true;
end

function Move:ended(scene, entity, event)
    return self._done;
end

return Move;