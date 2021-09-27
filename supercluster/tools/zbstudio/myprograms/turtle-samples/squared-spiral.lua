require "turtle"

pncl(colr(127,127,127))

local dist = 0.01
local angle = 89.5
local incr = 0.01
local side = size()/2

for i = 1, 184 do
  move(dist * side)
  turn(angle)
  dist = dist + incr
  wait(0.002)
end

save("squared-spiral")

wait()
