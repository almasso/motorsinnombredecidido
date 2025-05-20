local EventBehaviour = require 'data.events.EventBehaviour'

--- @class ModifyVariable
local ModifyVariable = EventBehaviour:inherit();

function ModifyVariable:new(variable, newValue, isPlayerVariable)
    local obj = ModifyVariable:super().new(self)
    obj._variable = variable;
    obj._newValue = newValue;
    obj._isPlayerVariable = isPlayerVariable;
    obj._playerHandler = "Player"
    return obj;
end

function ModifyVariable:init(scene, entity, event)
    if (self._isPlayerVariable) then
        local player = scene:getEntity(self._playerHandler);
        self._localVariables = LocalVariables.get(player);
    else
        self._localVariables = LocalVariables.get(entity);
    end
    return not (self._localVariables == nil);
end

function ModifyVariable:act(game, scene, entity, event)
    self._done = true;
    return self._localVariables:setVariable(self._variable, self._newValue);
end

return ModifyVariable;