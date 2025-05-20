---Base table prepared for Object Oriented Programming
---@class Class
local Class = {}

---Class constructor
---@return Class object New instance of the class
function Class:new()
    self.__index = self
    local obj = {}
    setmetatable(obj, self)
    return obj
end

---Creates a base table for an inheritted class
---@return table subclass Base table for an inheritted class 
function Class:inherit()
    local subclass = { }
    subclass.__index = subclass
    setmetatable(subclass, { __index = self })
    return subclass
end

---Accesses the super class
---@return Class super Super class
function Class:super()
    return getmetatable(self).__index
end

return Class