require("wx")
require("turtle")
local crt = require("chartmap")
local cmp = require("complex")
local col = require("colormap")
local com = require("common")

io.stdout:setvbuf("no")

local logStatus = com.logStatus
local  W,  H = 1500, 300
local dX, dY = 1,1
local xySize = 3
local nTimeFR = 0.15
local nRayCast = 50
local nN1, nN2 = 1.333, 1 -- Water and air
local greyLevel  = 200
local minX, maxX = -50, 50
local minY, maxY = -10, 10
local cOrg  = cmp.getNew(0, -4)
local tLin  = {cmp.getNew(minX, 0), cmp.getNew(maxX, 0)}
local intX  = crt.New("interval","WinX", minX, maxX, 0, W)
local intY  = crt.New("interval","WinY", minY, maxY, H, 0)
local clBlu = colr(col.getColorBlueRGB())
local clRed = colr(col.getColorRedRGB())
local clBlk = colr(col.getColorBlackRGB())
local clGry = colr(greyLevel,greyLevel,greyLevel)
local clMgn = colr(col.getColorMagenRGB())
local scOpe = crt.New("scope"):setInterval(intX, intY)
      scOpe:setSize(W, H):setColor(clBlk, clGry):setBorder():setDelta(dX, dY)

local function drawComplex(C, Cl)
  local x = intX:Convert(C:getReal()):getValue()
  local y = intY:Convert(C:getImag()):getValue()
  pncl(Cl); rect(x-xySize,y-xySize,2*xySize+1,2*xySize+1)
end

local function drawComplexLine(S, E, Cl)
  local x1 = intX:Convert(S:getReal()):getValue()
  local y1 = intY:Convert(S:getImag()):getValue()
  local x2 = intX:Convert(E:getReal()):getValue()
  local y2 = intY:Convert(E:getImag()):getValue()
  pncl(Cl); line(x1, y1, x2, y2)
end

cmp.setAction("xy", drawComplex)
cmp.setAction("ab", drawComplexLine)

logStatus("This demo is a Grimzel bath ( physics ) refraction lab implementation")
logStatus("On the coordinate system the OX and OY axises are drawn in black")
logStatus("The distance between every grey line on X is: "..tostring(dX))
logStatus("The distance between every grey line on Y is: "..tostring(dY))
logStatus("Refraction indexes: http://hyperphysics.phy-astr.gsu.edu/hbase/Tables/indrf.html")

open("Grimzel bath refraction demo. What does a fish sees from its point of view ( RED )")
size(W, H); zero(0, 0)
updt(false) -- disable auto updates

scOpe:Draw(true, true, true)
cOrg:Action("xy", clMgn)
local eAng, nMul = (math.pi/2), cOrg:getNorm()
local dAng, nAng = (eAng / nRayCast), 0

-- Skip the logs about reflecting primary ray on interface
com.logSkipAdd("complex.getRefractRayRay: Angle mismatch")
com.logSkipAdd("complex.getRefractRayRay: Normal mismatch")

logStatus("Critical refraction angle is: "..cmp.toDeg(cmp.getRefractRayAngle(nN1, nN2)).." degrees.")

while(nAng < eAng) do
  local rD, lD = cOrg:getNeg():RotRad(-nAng), cOrg:getNeg():RotRad(nAng)
  local rX = cmp.getIntersectRayLine(cOrg, rD, tLin[1], tLin[2])
  local rR = cmp.getRefractRayLine(cOrg, rD, tLin[1], tLin[2], nN1, nN2)
  local lX = cmp.getIntersectRayLine(cOrg, lD, tLin[1], tLin[2])
  local lR = cmp.getRefractRayLine(cOrg, lD, tLin[1], tLin[2], nN1, nN2)
  if(not rR) then rR = cmp.getReflectRayLine(cOrg, rD, tLin[1], tLin[2]) end
  if(not lR) then lR = cmp.getReflectRayLine(cOrg, lD, tLin[1], tLin[2]) end
  rX:Action("xy", clBlu); lX:Action("xy", clBlu)
  cOrg:Action("ab", rX, clBlu); cOrg:Action("ab", lX, clBlu)
  local rE, lE = rX:getAdd(rR:Mul(2 * nMul)), lX:getAdd(lR:Mul(2 * nMul))
  rE:Action("xy", clRed); lE:Action("xy", clRed)
  rX:Action("ab", rE, clRed); lX:Action("ab", lE, clRed)
  nAng = nAng + dAng ; updt(); wait(nTimeFR)
end

wait()
