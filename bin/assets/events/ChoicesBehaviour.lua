local EventBehaviour = require 'assets.events.EventBehaviour'

--- @class Choices
local Choices = EventBehaviour:inherit()

function Choices:new(variable,options,optionTexts)
    local obj = Choices:super().new(self)
    obj._buttonsHandler = "Choice"
    obj._variable = variable
    obj._options = options
    obj._optionTexts = optionTexts
    obj._ended = false
    return obj
end

function Choices:init(scene, entity, event)
    self._buttons = {}
    for i = 1, #self._options do
        local button = scene:getEntity(self._buttonsHandler..i)
        if button == nil then
            return false;
        end
        self._buttons[i] = button
    end
    self._variablesComponent = LocalVariables.get(entity)
    return self._variablesComponent ~= nil
end

function Choices:act(game, scene, entity, event)
    for i = 1, #self._buttons do
        local button = self._buttons[i]
        if button == nil then
            return false;
        end
        local buttonComponent = Button.get(button)
        if buttonComponent == nil then
            return false;
        end
        buttonComponent:setCallback(
            function(params)
                return self:choose(params[1])
            end)
        local textComponent = Text.get(button)
        if textComponent == nil then
            return false;
        end
        local text = self._optionTexts[i]
        if text == nil then
            return false
        end
        textComponent:setText(text)
        button:setActive(true)
    end
    self._done = true
    return true
end

function Choices:ended(scene, entity, event)
    return self._ended
end

function Choices:choose(choice)
    local value = self._options[choice]
    if value == nil then
        return false
    end
    self._variablesComponent:setVariable(self._variable, value)
    for i, button in ipairs(self._buttons) do
        button:setActive(false)
    end
    self._ended = true
    return true
end

return Choices