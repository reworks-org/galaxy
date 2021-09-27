require "turtle"

local x,y = size() -- get the current screen size
updt(false) -- disable auto updates
zero(0, 0) -- change origin such that (0,0) is now in the left top corner
load('turtle-samples/zerobrane')
pnsz(3)
for row = 0, x, 3 do
  for col = 0, y, 3 do
    pnpx(row, col)
    line(row, col, row+rand(7), col-rand(7))
  end
  updt()
end
wait()
