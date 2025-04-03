local EventBehaviour = require 'assets.events.EventBehaviour'

--- @class PlaySFX
local PlaySFX = EventBehaviour:inherit();

function PlaySFX:new(sfxHandler)
    local obj = PlaySFX:super().new(self)
    obj._sfxHandler = sfxHandler;
    return obj;
end

function PlaySFX:init(scene, entity, event)
    local ent = scene:getEntity(self._sfxHandler);
    if (ent == nil) then
        return false;
    end
    self._source = AudioSource.get(ent);
    return not (self._source == nil);
end

function PlaySFX:act(game, scene, entity, event)
    --print("\nPlaySFX act:");
    self._done = true;
    return self._source:play();
end

return PlaySFX;