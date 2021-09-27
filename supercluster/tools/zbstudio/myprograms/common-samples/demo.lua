local common = require("common")

-- Imploding and expoloding strings
local t = common.stringExplode("The brown fox jumps over the lazy dog !"," ")
table.insert(t, 5, "high")
common.logStatus("Result: "..common.stringImplode(t, " "))

-- Add pure time delay without opening a turtle window via "wait"
common.timeDelay(5)

--[[
  This lets you chose what to return even if the values are booleans,
  which is not properly handled via (A and B or C), though in the 
  output arguments you can still chose what expression to return
  Arguments: 
    1) A condition to check (optional)
    2) Value if the condition is true (optional)
    3) Value if the condition is false (optional)
]]
common.logStatus("Bool: "..tostring(common.getPick(t~=nil, false, true)))
common.timeDelay(5)

for i = 1, 10 do
  --[[
    The random number/string generation seed creator has
    protected call can be invoked every second. That way
    it can take randomization to the maximum by reverting the
    number and using the the seconds for triggering the most significant bits
    The function randomGetNumber arguments:
      1) Print logs (optional)
  ]]
  common.randomSetSeed(true)
  --[[
    As you know according to lua random number generation manual,
    the first 2 or 3 numbers are not that "random" as requested.
    That's why we have to call the random generator some couple of
    times before returning the actual value
    The function randomGetNumber arguments:
      1) Lower limit for the "math.random" function (optional)
      2) Upper limit for the "math.random" function (optional)
      3) Dummy invoke times of "math.random" to generate the actual number (optional)
    The function "randomGetString" arguments:
      1) How long the generated string must be (optional) (0 for empty string)
      2) Controls the "randomGetNumber" third parameter when generating an index
  ]]
  local n = common.randomGetNumber(100)
  local s = common.randomGetString(80)
  common.logStatus(("ID: %4d <%s> #%d"):format(i, s, n))
  common.logStatus(("ID: %4d Random number in [0 , 1 ]: %f"):format(i, common.randomGetNumber()))
  common.logStatus(("ID: %4d Random number in [1 , 40]: %d"):format(i, common.randomGetNumber(40)))
  common.logStatus(("ID: %4d Random number in [50, 60]: %d"):format(i, common.randomGetNumber(50, 60)))
  common.logStatus(("ID: %4d Random number with 3 dummy calls in [50, 60]: %f"):format(i, common.randomGetNumber(50, 60, 3)))
  common.timeDelay(0.1)
end

common.timeDelay()
