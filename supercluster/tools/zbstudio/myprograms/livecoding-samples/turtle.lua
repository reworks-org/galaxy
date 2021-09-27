require "turtle"

local angles = 5
for _ = 1, angles do
  move(600/angles)
  turn(360/angles)
end
text(angles)