local EventBehaviour = require 'data.events.EventBehaviour'

--- @class Music
local Music = EventBehaviour:inherit();

function Music:new(actionParams)
    local obj = Music:super().new(self)
    obj._musicHandler = "Music";
    obj._actionParams = actionParams;
    return obj;
end

function Music:init(scene, entity, event)
    local ent = scene:getEntity(self._musicHandler);
    if (ent == nil) then
        return false;
    end
    self._source = AudioSource.get(ent);
    return not (self._source == nil);
end

function Music:act(game, scene, entity, event)
    --print("\nMusic act:");
    --print(self._actionParams.action);
    self._done = true;
    local action = self._actionParams.action;
    if (action == "play") then
        return self._source:play();
    elseif (action == "stop") then
        return self._source:stop();
    elseif (action == "resume") then
        return self._source:resume();
    elseif (action == "pause") then
        self._source:pause();
    elseif (action == "change") then
        self._source:changeClip(self._actionParams.clip);
    elseif (action == "volume") then
        self._source:setVolume(self._actionParams.volume);
    elseif (action == "loop") then
        self._source:setLoop(self._actionParams.loop);
    end
    return true;
end

return Music;