require("wx")
require("turtle")
local crt = require("chartmap")
local cmp = require("complex")
local col = require("colormap")
local com = require("common")

io.stdout:setvbuf("no")

local logStatus = com.logStatus
local  W,  H = 400, 400
local greyLevel  = 200
local minX, maxX = -20, 20
local minY, maxY = -20, 20
local dX, dY, xySize = 1, 1, 3
local intX  = crt.New("interval","WinX", minX, maxX, 0, W)
local intY  = crt.New("interval","WinY", minY, maxY, H, 0)
local clBlu = colr(col.getColorBlueRGB())
local clRed = colr(col.getColorRedRGB())
local clBlk = colr(col.getColorBlackRGB())
local clGry = colr(col.getColorPadRGB(greyLevel))
local clMgn = colr(col.getColorMagenRGB())
local clGrn = colr(col.getColorGreenRGB())
local clCya = colr(col.getColorCyanRGB())
local scOpe = crt.New("scope"):setBorder(minX, maxX, minY, maxY)
      scOpe:setSize(W, H):setColor(clBlk, clGry):setInterval(intX, intY):setDelta(dX, dY)

local function drawComplex(C, Cl, vN)
  local nN = (tonumber(vN) or xySize)
  local x = intX:Convert(C:getReal()):getValue()
  local y = intY:Convert(C:getImag()):getValue()
  pncl(Cl); rect(x-nN,y-nN,2*nN,2*nN)
end

local function drawComplexLine(S, E, Cl)
  local x1 = intX:Convert(S:getReal()):getValue()
  local y1 = intY:Convert(S:getImag()):getValue()
  local x2 = intX:Convert(E:getReal()):getValue()
  local y2 = intY:Convert(E:getImag()):getValue()
  pncl(Cl); line(x1, y1, x2, y2)
end

local function getCoordConvert(C)
  local cx = intX:Convert(C:getReal()):getValue()
  local cy = intY:Convert(C:getImag()):getValue()
  local cN = cmp.getNew(cx, cy)
  return cN
end

cmp.setAction("xy", drawComplex)
cmp.setAction("ab", drawComplexLine)
cmp.setAction("conv", getCoordConvert)

logStatus("Create a circle to intersect using the right mouse button (RED)")
logStatus("Create a ray to intersect using the left mouse button (BLUE)")
logStatus("By clicking on the chart the point selected will be drawn")
logStatus("On the coordinate system the OX and OY axises are drawn in black")
logStatus("The distance between every grey line on X is: "..tostring(dX))
logStatus("The distance between every grey line on Y is: "..tostring(dY))
logStatus("Press escape to clear all rays and refresh the coordinate system")

open("Complex rays circle intersect demo")
size(W, H)
zero(0, 0)
updt(false) -- disable auto updates

scOpe:Draw(true, true, true)

cRay1, cRay2, drw, R1, R2 = {}, {}, true, 0, 0

while true do
  wait(0.2)
  local key = char()
  local lx, ly = clck('ld')
  local rx, ry = clck('rd')
  if(lx and ly and #cRay1 < 2) then -- Reverse the interval conversion polarity
    lx = intX:Convert(lx,true):getValue()
    ly = intY:Convert(ly,true):getValue()
    local C = cmp.getNew(lx, ly); C:Action("xy", clBlu); cRay1[#cRay1+1] = C
    if(#cRay1 == 2) then cRay1[1]:Action("ab", cRay1[2], clBlu)
      R1 = cRay1[2]:getSub(cRay1[1]):getNorm()
      scOpe:setColorDir(clBlu)
      scOpe:drawComplexCircle(cRay1[1], R1)
    end
  elseif(rx and ry and #cRay2 < 2) then -- Reverse-convert x, y position to a complex number
    rx = intX:Convert(rx,true):getValue()
    ry = intY:Convert(ry,true):getValue()
    local C = cmp.getNew(rx, ry); C:Action("xy", clRed); cRay2[#cRay2+1] = C
    if(#cRay2 == 2) then cRay2[1]:Action("ab", cRay2[2], clRed)
      R2 = cRay2[2]:getSub(cRay2[1]):getNorm()
      scOpe:setColorDir(clRed)
      scOpe:drawComplexCircle(cRay2[1], R2)
    end
  end
  if(drw and #cRay1 == 2 and #cRay2 == 2) then
    local xR, xL, xX = cmp.getIntersectCircleCircle(cRay1[1], R1, cRay2[1], R2)
    if(xR) then
      xR:Action("ab", xL, clMgn); cRay1[1]:Action("ab", cRay2[1], clMgn)
      xR:Action("xy", clGrn); xL:Action("xy", clBlk); xX:Action("xy", clMgn)
      logStatus("Intersection add (right) is "..xR)
      logStatus("Intersection sub (left)  is "..xL)
    else
      logStatus("The needed conditions are not met for the intersection to happen")
    end; drw = false
  end
  if(key == 27) then -- The user hits esc
    scOpe:setColorDir(clBlu)
    wipe(); drw = true
    cRay1[1], cRay1[2] = nil, nil
    cRay2[1], cRay2[2] = nil, nil; collectgarbage()
    scOpe:Draw(true, true, true)
  end
  updt()
end

wait()
