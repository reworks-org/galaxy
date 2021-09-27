require "turtle"

for i=1,5 do
  move(80)
  turn(1/5*360)
end

turn(45)
jump(40)

for i=1,5 do
  move(60)
  turn(2/5*360)
end

wait()