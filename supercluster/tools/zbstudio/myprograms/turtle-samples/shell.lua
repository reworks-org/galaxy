require "turtle"

show()
pncl(colr(255, 127, 00))
local step = 1
while dist() < 200 do
  move(step)
  turn(17)
  step = step + 0.05
end

wait()
