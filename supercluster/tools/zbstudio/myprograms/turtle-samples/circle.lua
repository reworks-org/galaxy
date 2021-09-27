require "turtle"

show()
local angle = 1
for i = 1, 360/angle+1 do
  turn(angle)
  move(angle)
end
hide()

wait()
