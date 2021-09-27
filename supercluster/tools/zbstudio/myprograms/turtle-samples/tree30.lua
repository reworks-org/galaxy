require "turtle"

local f=0.5 -- how much shorter the next tree should be
local a=60 -- what angle to use to turn the branches

function tree(d, n) -- distance to grow, level for recursion
  if n == 0 then return end
  
  local width = pnsz(n) -- set pen size; save current value
  local color = pncl(colr(127,(8-n)*32,0)) -- set pen color

  move(2/3*d)
  turn(a)
  tree(d*f, n-1)
  turn(-a)

  move(1/3*d)
  turn(-a)
  tree(d*f, n-1)
  turn(a)

  turn(7)
  tree(d*f, n-1)
  turn(-7)
  move(-1/3*d) -- note that 1/3+2/3 is not always 1
  move(-2/3*d) -- so move back in the same way as forward

  pncl(color) -- restore pen color to make it work recursively
  pnsz(width) -- restore pen size to make it work recursively

  wait(0.001) -- slow down the drawing
end

turn(-90) -- point turtle up as trees grow up

-- grow a large tree
posn(0, 120)
tree(160, 7)

-- grow a small bush
posn(-80, 120)
tree(80, 6)

-- grow grass
for grass = -180,180 do
  posn(grass+rand(5),120+rand(25))
  tree(8,3)
end

save("tree")

wait()
