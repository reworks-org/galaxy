require "turtle"

local side = 225;
local maxx, maxy = 10, 20
pncl(colr(200,200,200))
line(-side, 0, side, 0) text("x", 0, side-15, 2)
line(0, -side, 0, side) text("y", 0, 5, -side+15)
pncl(colr(128,160,255))

function f(x) return math.sin(x)^-1 + 1*math.cos(x) end

local xp = -maxx
local yp = f(xp)
posn(xp*side/maxx, yp*(-side/maxy))
for x = -maxx, maxx, 0.05 do
  local y = f(x)
  move((x-xp)*side/maxx, (y-yp)*(-side/maxy))
  xp, yp = x, y
end