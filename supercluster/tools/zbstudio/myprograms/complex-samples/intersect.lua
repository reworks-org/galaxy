require("wx")
require("turtle")
local crt = require("chartmap")
local cmp = require("complex")
local col = require("colormap")
local com = require("common")

io.stdout:setvbuf("no")

local logStatus = com.logStatus
local  W,  H = 400, 400
local dX, dY = 1,1
local xySize = 3
local greyLevel  = 200
local minX, maxX = -20, 20
local minY, maxY = -20, 20
local intX  = crt.New("interval","WinX", minX, maxX, 0, W)
local intY  = crt.New("interval","WinY", minY, maxY, H, 0)
local clOrg = colr(col.getColorBlueRGB())
local clRel = colr(col.getColorRedRGB())
local clBlk = colr(col.getColorBlackRGB())
local clGry = colr(greyLevel,greyLevel,greyLevel)
local clMgn = colr(col.getColorMagenRGB())
local scOpe = crt.New("scope"):setBorder(minX, maxX, minY, maxY)
      scOpe:setSize(W, H):setColor(clBlk, clGry):setInterval(intX, intY):setDelta(dX, dY)

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

local function drawComplexArrow(O, D, Cl, nA, nL)
  local nA, cD = (nA or 10), D:getNew()
  local xO = intX:Convert(O:getReal()):getValue()
  local yO = intY:Convert(O:getImag()):getValue()
  local cO, cD = complex.getNew(xO,yO)
  cD:Set(D); cD:RotAngRad(-nA); cD:Neg(); cD:Add(cO)
  pncl(Cl); line(cO:getReal(), cO:getImag(), cD:getReal(), cD:getImag())
  cD:Set(D); cD:RotAngRad(2 * nA); cD:Neg(); cD:Add(cO)
  pncl(Cl); line(cO:getReal(), cO:getImag(), cD:getReal(), cD:getImag())
end

cmp.setAction("xy", drawComplex)
cmp.setAction("ab", drawComplexLine)
cmp.setAction("rw", drawComplexArrow)

logStatus("Create a primary ray to intersect using the left mouse button (BLUE)")
logStatus("Create a secondary ray to intersect using the right mouse button (RED)")
logStatus("By clicking on the chart the point selected will be drawn")
logStatus("On the coordinate system the OX and OY axises are drawn in black")
logStatus("The distance between every grey line on X is: "..tostring(dX))
logStatus("The distance between every grey line on Y is: "..tostring(dY))
logStatus("Press escape to clear all rays and refresh the coordinate system")

open("Complex rays intersection demo")
size(W,H)
zero(0, 0)
updt(false) -- disable auto updates

scOpe:Draw(true, true, true)

cRay1, cRay2, drw = {}, {}, true

while true do
  wait(0.2)
  local key = char()
  local lx, ly = clck('ld')
  local rx, ry = clck('rd')
  if(lx and ly and #cRay1 < 2) then -- Reverse the interval conversion polarity
    lx = intX:Convert(lx,true):getValue() -- It helps by converting x,y from positive integers to the interval above
    ly = intY:Convert(ly,true):getValue()
    local C = cmp.getNew(lx, ly)
    cRay1[#cRay1+1] = C; C:Action("xy", clOrg)
    if(#cRay1 == 2) then cRay1[1]:Action("ab", cRay1[2], clOrg) end
  elseif(rx and ry and #cRay2 < 2) then -- Reverse-convert x, y position to a complex number
    rx = intX:Convert(rx,true):getValue()
    ry = intY:Convert(ry,true):getValue()
    local C = cmp.getNew(rx, ry); C:Action("xy", clRel); cRay2[#cRay2+1] = C
    if(#cRay2 == 2) then cRay2[1]:Action("ab", cRay2[2], clRel) end
  end
  if(drw and #cRay1 == 2 and #cRay2 == 2) then
    local cD1, cD2 = (cRay1[2] - cRay1[1]), (cRay2[2] - cRay2[1])
    local XX, nT, nU = cmp.getIntersectRayRay(cRay1[1], cD1, cRay2[1], cD2)
    if(XX) then XX:Action("xy", clMgn)
      logStatus("The complex intersection is "..tostring(XX))
      local onOne = XX:isAmongLine(cRay1[1], cRay1[2])
      local onTwo = XX:isAmongLine(cRay2[1], cRay2[2])
      logStatus("The point is "..(onOne and "ON" or "OFF").." the first line (BLUE)")
      logStatus("The point is "..(onTwo and "ON" or "OFF").." the second line (RED)")
      onOne = XX:isAmongLine(cRay1[1], cRay1[2], true)
      onTwo = XX:isAmongLine(cRay2[1], cRay2[2], true)
      logStatus("The point (full-size) is "..(onOne and "ON" or "OFF").." the first line (BLUE)")
      logStatus("The point (full-size) is "..(onTwo and "ON" or "OFF").." the second line (RED)")
      onOne = XX:isAmongRay(cRay1[1], cRay1[2]-cRay1[1])
      onTwo = XX:isAmongRay(cRay2[1], cRay2[2]-cRay2[1])
      logStatus("The point is "..(onOne and "ON" or "OFF").." the first ray (BLUE)")
      logStatus("The point is "..(onTwo and "ON" or "OFF").." the second ray (RED)")
      onOne = XX:isAmongRay(cRay1[1], cRay1[2]-cRay1[1],true)
      onTwo = XX:isAmongRay(cRay2[1], cRay2[2]-cRay2[1],true)
      logStatus("The point (full-size) is "..(onOne and "ON" or "OFF").." the first ray (BLUE)")
      logStatus("The point (full-size) is "..(onTwo and "ON" or "OFF").." the second ray (RED)")         
    end; drw = false
  end
  if(key == 27) then -- The user hits esc
    wipe(); drw = true
    cRay1[1], cRay1[2] = nil, nil
    cRay2[1], cRay2[2] = nil, nil; collectgarbage()
    scOpe:Draw(true, true, true)
  end
  updt()
end

wait();
