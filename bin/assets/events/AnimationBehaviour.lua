local EventBehaviour = require 'assets.events.EventBehaviour'

--- @class Animation
local Animation = EventBehaviour:inherit();

function Animation:new(actionParams)
    local obj = Animation:super().new(self)
    obj._actionParams = actionParams;
    return obj;
end

function Animation:init(scene, entity, event)
    self._animator = Animator.get(entity);
    return not (self._animator == nil);
end

function Animation:act(game, scene, entity, event)
    --print("\nAnimation act:");
    --print(self._actionParams.action);
    self._done = true;
    local action = self._actionParams.action;
    if (action == "reset") then
        self._animator:reset();
    elseif (action == "change") then
        self._animator:changeAnimation(self._actionParams.animation);
    elseif (action == "play") then
        self._animator:setPlaying(true);
    elseif (action == "stop") then
        self._animator:setPlaying(false);
    end
    return true;
end

function Animation:ended(scene, entity, event)
    return self._animator:animationEnded();
end

return Animation;