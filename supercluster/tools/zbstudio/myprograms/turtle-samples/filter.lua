require "turtle"

local x,y = size() -- get the current screen size
updt(false) -- disable auto updates
zero(0, 0) -- change origin such that (0,0) is now in the left top corner
load('turtle-samples/zerobrane')
for col = 0, x do
  for row = 0, y do
    pnpx(col, row)
    local color = pncl()
    local r, g, b = color:Red(), color:Green(), color:Blue()
    -- invert all colors
    pncl(colr(255-r, 255-g, 255-b)) -- set pen color
    pixl(col, row) -- draw a pixel using the current pen
  end
  updt()
end
wait()
