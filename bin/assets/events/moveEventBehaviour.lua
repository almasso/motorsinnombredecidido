--- @class Move
local Move = {
    _done = false,
    _ended = false
};

function Move:new()
    self.__index = self
    local obj = {}
    setmetatable(obj, self)
    return obj
end

function Move:act(game, scene, entity, event)
    print("MOVE");
    self._done = true;
    self._ended = true;
    return 0;
end

function Move:done()
    return self._done;
end

function Move:ended()
    return self._ended;
end

return Move;