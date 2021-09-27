require("turtle")
local chartmap = require("chartmap")
local complex  = require("complex")
local common   = require("common")
local signals  = require("signals")

--[[
  Here goes the neurons activating function
  It should recieve number and return number
]]
local function act(x)
  return (2*(1 / (1 + math.exp(-x))-0.5))
end

--[[
  Here goes the derivative of the activating function
  It is usially done by hand and rigged here
]]
local function act_diff(x)
  return ((2 * math.exp(x))/(1+math.exp(x))^2)
end

--[[
  This is a custom function which you may use to test the error handling
  You can replace one of the arguments to "setActive" to test this
]]
function foo(x)
  local c = {}
  local d = x..c
  return 0
end  

--[[
  This is the data that we must train our neural net with
  For multiple inputs, 
]]
local tTrL2 = {
  {{0  , 0.5,   1}, {  0,  0.1}},
  {{0.7, 0.4, 0.1}, {  1, -0.3}},
  {{0.2, 0.1, 0.6}, { -1,  0.01}},
  {{1  ,-0.2,   1}, {0.5, -0.2}}
}

--[[
  Amadeus must know that the sequential numbers stored in "tTrL2[1]"
  must give the result in "tTrL2[2]", so it repeatedly trains itself
  with learning that for example "{0.2, 0.1, 0.6}" must result in { -1, 0.01}.
  Due to its activating function, "act" knows that the output must be in [-1,1].
]]
local a = signals.New("neuralnet", "Amadeus")
      a:addLayer({},{},{}):addLayer({1,2,3},{})
      a:setActive(act, act_diff, out):Dump()
a:trainLayer(2, tTrL2, 250000, true)
a:Dump()

--[[
 Now Amadeus has graduated from tTrL2 college and it is a
 net which knows the sequences stored in "tTrL2[i][j]"
 Let's try its skills and see what will it tell us
 for some other sequence as it is an expert :D
]]

for k = 1, #tTrL2 do
  local tOut = a:setValue(unpack(tTrL2[k][1])):Process()
  if(not common.isNil(tOut)) then tOut = tOut:getOut()
    local sI = table.concat(tTrL2[k][2],", ")
    local sO = table.concat(tOut, ", ")
    common.logStatus("Compare: <"..sI.." # "..sO..">")
  end
end

local tEst = a:setValue(0.14,-0.81,0.9):Process()
if(not common.isNil(tEst)) then tEst = tEst:getOut()
  common.logStatus("This is a new case ("..table.concat(a:getValueLayer(1),", ").."): "..table.concat(tEst, ", "))
end
