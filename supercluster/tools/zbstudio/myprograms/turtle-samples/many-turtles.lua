require "turtle"

local distance, angle, step = 140, 15, 15
-- You may experiment with 140, 45, 15
-- or with 40, 175, 15

updt(false) -- disable auto updates
for i = 1, 1000 do
  rant(i) -- add a random turtle
  posn(rand(2)*distance-distance/2, rand(2)*distance-distance/2)
end
pick() -- pick all turtles
for i = 1, 1000 do
  turn(rand(angle))
  move(rand(step))
  updt()
end
wait()