local EventBehaviour = require 'assets.events.EventBehaviour'

--- @class ModifyLocalVariable
local ModifyLocalVariable = EventBehaviour:inherit();

function ModifyLocalVariable:new(variable, newValue)
    local obj = ModifyLocalVariable:super().new(self)
    obj._variable = variable;
    obj._newValue = newValue;
    return obj;
end

function ModifyLocalVariable:init(scene, entity, event)
    self._localVariables = LocalVariables.get(entity);
    return not (self._localVariables == nil);
end

function ModifyLocalVariable:act(game, scene, entity, event)
    --print("\nModifyLocalVariable act:");
    --print(self._variable);
    --print(self._newValue);
    self._done = true;
    return self._localVariables:setVariable(self._variable, self._newValue);
end

return ModifyLocalVariable;