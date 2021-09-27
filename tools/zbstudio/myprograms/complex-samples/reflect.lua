require("wx")
require("turtle")
local crt = require("chartmap")
local cmp = require("complex")
local col = require("colormap")
local com = require("common")

io.stdout:setvbuf("no")

local logStatus = com.logStatus
local  W,  H = 800, 800
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

logStatus("Create a mirror line to reflect off using the right mouse button (RED)")
logStatus("Create a ray to reflect of the mirror using the left mouse button (BLUE)")
logStatus("By clicking on the chart the point selected will be drawn")
logStatus("On the coordinate system the OX and OY axises are drawn in black")
logStatus("The distance between every grey line on X is: "..tostring(dX))
logStatus("The distance between every grey line on Y is: "..tostring(dY))
logStatus("Press escape to clear all rays and refresh the coordinate system")

open("Complex ray reflection demo")
size(W, H); zero(0, 0)
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
    local C = cmp.getNew(rx, ry)
    C:Action("xy", clRel); cRay2[#cRay2+1] = C
    if(#cRay2 == 2) then cRay2[1]:Action("ab", cRay2[2], clRel) end
  end
  if(drw and #cRay1 == 2 and #cRay2 == 2) then
    local cD1, cD2 = (cRay1[2] - cRay1[1]), (cRay2[2] - cRay2[1])
    local cX, nT, nU = cmp.getIntersectRayRay(cRay1[1], cD1, cRay2[1], cD2)
    if(cX) then local nN = cD1:getNorm()
      local cR, cN = cmp.getReflectRayLine(cRay1[1], cD1, cRay2[1], cRay2[2])
      if(cR) then 
        local eR, eN = (cR*nN+cX), (cN*nN/2+cX)
        cX:Action("xy", clMgn); eR:Action("xy", clMgn)
        cX:Action("ab", eR, clOrg); cX:Action("ab", eN, clBlk)
        logStatus("The complex reflection is <"..tostring(cR).."/"..tostring(eR)..">")
      else
        logStatus("The complex reflection cannot happen")
      end
    else
      logStatus("The complex reflection is parallel <"..tostring(cN).."/"..tostring(cR)..">")
    end 
    drw = false
  end
  if(key == 27) then -- The user hits esc
    wipe(); drw = true
    cRay1[1], cRay1[2] = nil, nil
    cRay2[1], cRay2[2] = nil, nil; collectgarbage()
    scOpe:Draw(true, true, true)
  end
  updt()
end

wait()
