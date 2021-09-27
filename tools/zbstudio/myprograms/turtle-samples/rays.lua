require "turtle"

local shift = 5
local side = 200
for i=0, side/shift do
  line(-side, i*shift, side, -i*shift)
  line(-side, -i*shift, side,  i*shift)
  line(i*shift, -side, -i*shift, side)
  line(-i*shift, -side,  i*shift, side)
  wait(0.05)
end

save("squared-rays")

wait()
