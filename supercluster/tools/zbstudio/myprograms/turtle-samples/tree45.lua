require "turtle"

local f=0.6
local a=45
function tree(d, n)
  if n>0 then
    move(d)

    turn(-a)
    tree(d*f, n-1)
    turn(a)

    turn(a)
    tree(d*f, n-1)
    turn(-a)

    move(-d)
  end
end

turn(-90)
move(-100)
tree(100, 8)

wait()
