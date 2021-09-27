require "turtle"

local side = 225
local maxx, maxy = 2.5, 2.5

pncl(colr(200,200,200))
line(-side, 0, side, 0) text("x", 0, side-15, 2)
line(0, -side, 0, side) text("y", 0, 5, -side+15)
pncl(colr(128,160,255))

local x, y = -1, 0
posn(x*side/maxx, y*(-side/maxy)) 
text(x, 0, -5, 2)
local dt = 0.01
for _ = 1, 20, dt do
  local dx = y*dt
  local dy = (-0.5 * y - 5 * math.sin(x))*dt
  move(dx*side/maxx, dy*(-side/maxy))
  x, y = x+dx, y+dy
end