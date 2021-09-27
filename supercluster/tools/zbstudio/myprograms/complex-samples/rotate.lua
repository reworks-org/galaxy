require("turtle")
local crt = require("chartmap")
local cmp = require("complex")
local col = require("colormap")

io.stdout:setvbuf("no")

local W ,  H = 800, 800
local dX, dY =  1 , 1
local gAlp   = 200
local intX = crt.New("interval","WinX", -50, 50, 0, W)
local intY = crt.New("interval","WinY", -50, 50, H, 0)
local aAng, dA, nRad = 0, 15, 30
local C = cmp.getNew(nRad, 0)
local D = C:getNew()

-- Allocate colors
local clGrn = colr(col.getColorGreenRGB())
local clRed = colr(col.getColorRedRGB())
local clBlk = colr(col.getColorBlackRGB())
local clGry = colr(col.getColorPadRGB(gAlp))
local scOpe = crt.New("scope"):setInterval(intX, intY):setBorder()
      scOpe:setSize(W, H):setColor(clBlk, clGry):setDelta(dX, dY)

local function drawComplex(C,A,T)
  local r = C:getRound(0.1)
  local px = intX:Convert(r:getReal()):getValue()
  local py = intY:Convert(r:getImag()):getValue()
  local ox = intX:Convert(0):getValue()
  local oy = intY:Convert(0):getValue()
  pncl(clGrn); line(ox, oy, px, py)
  pncl(clRed); rect(px-2,py-2,5,5)
  if(T) then pncl(clBlk); text(A.." > "..tostring(r),r:getAngDeg(),px,py) end
end

cmp.setAction("ang", drawComplex)

open("Complex rotation")
size(W, H); zero(0, 0)
updt(false) -- disable auto updates

scOpe:Draw(true, true, true); updt()

while(aAng < 360) do
  C:getRotRad(cmp.toRad(aAng)):Action("ang",aAng,true)
  D:Set(C):RotRad(cmp.toRad(aAng)):Action("ang",aAng)
  D:Set(C):setAngRad(cmp.toRad(aAng)):Action("ang",aAng)
  C:getRotDeg(aAng):Action("ang",aAng)
  D:Set(C):RotDeg(aAng):Action("ang",aAng)
  D:Set(C):setAngDeg(aAng):Action("ang",aAng)
  aAng = aAng + dA
  updt(); wait(0.1)
end

wait()
