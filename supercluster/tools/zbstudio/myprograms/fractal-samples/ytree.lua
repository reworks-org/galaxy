require("turtle")
local fractal = require("fractal")

local D, W, H, T = 11, 900,900, 0
local oTree = fractal.New("ytree", D, colr(100, 50, 255))
if(oTree) then -- disable auto updates
  open("Binary tree branching")
  size(W,H); zero(0, 0); updt(true)
  oTree:Allocate():Draw(W/2,0,W/4,H/4,wait,T)
end

wait()
