require "turtle"

local x,y = size() -- get the current screen size
local maxc = 256
updt(false) -- disable auto updates
zero(0, 0) -- change origin such that (0,0) is now in the left top corner
for row = 0, x do
  for col = 0, y do
    local res = row^2 + col^2 -- also try using "row * col"
    local b, g, r = res / (maxc^2) % maxc, res / maxc % maxc, res % maxc
    pncl(colr(r, g, b)) -- set pen color
    pixl(row, col) -- draw a pixel using the current pen
  end
  updt()
end
wait()
