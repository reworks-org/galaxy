require "turtle"
local common   = require("common")
local getClamp = common.getClamp

local f = 0.75 -- how much shorter the next tree should be
local a = 20 -- angle to use to turn the branches
local l = 7 -- depth of the tree
-- may want to try these combinations
-- f, a, l = 0.7, 20, 8
-- f, a, l = 0.75, 25, 9

function tree(d, n) -- distance to grow, level for recursion
  if n == 0 then return end
  local green = getClamp(math.floor((10 - n)*24),0,255)
  local width = pnsz(n) -- set pen size; save current value
  local color = pncl(colr(127,green,0)) -- set pen color

  move(2/3*d)
  turn(a)
  tree(d*f, n-1)
  turn(-a)

  move(1/3*d)
  turn(-a)
  tree(d*f, n-1)
  turn(a)

  local angle = 7
  turn(angle)
  tree(d*f, n-1)
  turn(-angle)
  move(-1/3*d) -- as 1/3+2/3 is not always 1 because of rounding,
  move(-2/3*d) -- move back in the same way as forward

  pncl(color) -- restore pen color to make it work recursively
  pnsz(width) -- restore pen size to make it work recursively
  updt() -- make what's drawn visible
end

turn(-90) -- point turtle up as trees grow up

updt(false) -- disable auto updates
posn(0, 180)
tree(100, l)

wait()
