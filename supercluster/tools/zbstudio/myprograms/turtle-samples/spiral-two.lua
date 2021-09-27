require "turtle"

local i = 0.0
local loops = 30

pncl(ranc())
posn(1, 0)

trtl() -- add one more turtle
posn(-1, 0)
turn(200)
pncl(ranc())

pick(1,2) -- pick both of the turtles

show()
while i < loops do
  turn(9)
  move(i)
  i = i + .035
end

wait()
