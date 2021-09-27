require "turtle"

local x,y = zero() -- get current origin
local w,h = size()
zero(0, 0) -- change origin such that (0,0) is now in the left top corner
pnsz(2) pncl(colr(0, 0, 255))
move(w) turn(90)
move(h) turn(90)
move(w) turn(90)
move(h) turn(90)
wait()
