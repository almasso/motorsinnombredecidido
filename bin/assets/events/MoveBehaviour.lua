local EventBehaviour = require 'assets.events.EventBehaviour'

--- @class Move
local Move = EventBehaviour:inherit();

function Move:new(x, y)
    local obj = Move:super().new(self)
    obj._target = Vector2.new(x or 0, y or 0);
    return obj;
end

function Move:init(scene, entity, event)
    self._transform = Transform.get(entity);
    self._moveComponent = MovementComponent.get(entity);
    return (not (self._transform == nil)) and (not (self._moveComponent == nil));
end

function Move:act(game, scene, entity, event)
    print("\nMomement act");
    self._moveComponent:setTarget(self._target);
    self._done = true;
    return true;
end

function Move:ended(scene, entity, event)
    return self._transform.position.x == self._target.x and self._transform.position.y == self._target.y;
end

return Move;