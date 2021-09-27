require "turtle"

size(500, 250)
local x, y = size()
posn(-x/2+5,y/2-5)
pncl(colr(127,127,127))
pnsz(2)

local function F (n)
  if n == 0 then 
    move(2)
  else
    F(n-1)
    turn(-90)
    F(n-1)
    turn(90)
    F(n-1)
    turn(90)
    F(n-1)
    turn(-90)
    F(n-1)
  end
end

F(5)

wait()
