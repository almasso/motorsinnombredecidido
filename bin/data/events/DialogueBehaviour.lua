local EventBehaviour = require 'data.events.EventBehaviour'

--- @class Dialogue
local Dialogue = EventBehaviour:inherit();

function Dialogue:new(text)
    local obj = Dialogue:super().new(self)
    obj._textBoxHandler = "TextBox";
    obj._text = text;
    return obj;
end

function Dialogue:init(scene, entity, event)
    self._textBoxEntity = scene:getEntity(self._textBoxHandler);
    if (self._textBoxEntity == nil) then
        return false;
    end
    --print(self._textBoxEntity);
    self._textBoxComponent = TextBox.get(self._textBoxEntity);
    --print(self._textBoxComponent);
    return not (self._textBoxComponent == nil);
end

function Dialogue:act(game, scene, entity, event)
    --print("\nDialogue act");
    self._done = true;
    self._textBoxComponent:setText(self._text);
    return true
end

function Dialogue:ended(scene, entity, event)
    return self._textBoxComponent:ended();
end

return Dialogue;