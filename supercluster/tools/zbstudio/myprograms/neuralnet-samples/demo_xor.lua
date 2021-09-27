require("turtle")
local signals  = require("signals")

--[[
  Here goes the neurons activating function
  It should recieve number and return number
]]
local function act(x)
  return (1 / (1 + math.exp(-x)))
end

--[[
  This is the data that we must train our neural net with
  For multiple inputs, 
]]
local tTr = {
  {{2, 2}, {0}},
  {{1, 1}, {0}},
  {{2, 1}, {1}},
  {{1, 2}, {1}}
}

--[[
  Amadeus must know that the sequential numbers stored in "tTr[1]"
  must give the result in "tTr[2]", so it repeatedly trains itself
  with learning that for example "{0, 1}" must result in {1}.
  Due to its activating function, "act" knows that the output must be in [0,1].
  https://stevenmiller888.github.io/mind-how-to-build-a-neural-network/
]]
local a = signals.New("neuralnet", "XOR")
      a:addLayer({},{}):addLayer({},{},{})
      a:addLayer({}):setActive(act, out)
a:Dump()
a:Train(tTr, 10, true):Dump()

print(a:setValue(2,2):getOut()[1])
print(a:setValue(1,1):getOut()[1])
print(a:setValue(2,1):getOut()[1])
print(a:setValue(1,2):getOut()[1])


