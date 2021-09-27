local com = require("common")
local smg = "Something just messed up !\nThe error is: %s\nThis is the stack traceback call:"

local function err()
  return ({}..3)
end

local function nerr()
  return "Yo"
end

local function cerr(f)
  local s, o = pcall(err)
  if(s) then return 0 end
  com.logCallStack(smg:format(o))
  return 1
end

local function a()
  local s, o = cerr(err)
  if(s) then return 0 end
  return 0
end

local function b()
  local c, d, e = 1, 5, a()
  return c+d+e
end

local function c()
  local a, b = 0, b()
  return a+b
end

com.logStatus("Result: "..tostring(c()))
