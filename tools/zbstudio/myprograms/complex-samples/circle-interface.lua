require("wx")
require("turtle")
local crt = require("chartmap")
local cmp = require("complex")
local col = require("colormap")
local com = require("common")

io.stdout:setvbuf("no")

local logStatus = com.logStatus
local  W,  H = 800, 800
local greyLevel  = 200
local minX, maxX = -20, 20
local minY, maxY = -20, 20
local nNCirc, nNArea = 1.2, 1
local dX, dY, xySize = 1, 1, 3
local nMove = 10e-2
local nMaxIter = 100
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

local function drawInterface(cP, cN, nL)
  cP:Action("ab", cP+cN:getRight():Mul(nL/2), clBlk)
  cP:Action("ab", cP+cN:getLeft():Mul(nL/2) , clBlk)
  cP:Action("xy", clBlk)
end

local function drawRay(cO, cD, nL, clC)
  cO:Action("xy", clBlk)
  cO:Action("ab", cO+cD:getUnit():Mul(tonumber(nL) or 0), clC or clGrn)
end

local function getRefractIndex(cO, cP, nR)
  if(cO:isInCircle(cP, nR)) then
    return nNCirc, nNArea end
  return nNArea, nNCirc
end

local function drawTraceRayCircle(vO, vD, cP, nR)
  local nnS, nnE = getRefractIndex(vO, cP, nR)
  if(nnS * nnE == 0) then
    return logStatus("traceRayCircle: Refract index mismatch") end
  local xO, xD, nnC = vO:getNew(), vD:getNew(), 0
  local xX = cmp.getIntersectRayCircle(xO, xD, cP, nR)
  while(xX and nnC < nMaxIter) do nnC = nnC + 1
    local fR, fN = cmp.getRefractRayCircle(xO, xD, cP, nR, nnS, nnE, xX)
    if(not fR) then fR, fN = cmp.getReflectRayCircle(xO, xD, cP, nR, xX) end
    if(not fR) then
      logStatus("traceRayCircle: Refract mismatch"); break end
    if(fR:isNan()) then
      logStatus("traceRayCircle: Refract NaN"); break end
    if(not fN) then
      logStatus("traceRayCircle: Normal mismatch"); break end
    if(fN:isNan()) then
      logStatus("traceRayCircle: Normal NaN"); break end
    local xR = cmp.getReflectRayCircle(xO, xD, cP, nR)
    if(xR and xX) then drawRay(xX, xR, 5, clMgn) end
    nnS, nnE = getRefractIndex(xX:getAdd(fR:getMul(nMove)), cP, nR)
    xX:Action("ab",((nnC == 1) and xO:getAdd(xD) or xO),clGrn)
    xO:Set(xX); xD:Set(fR)
    xX = cmp.getIntersectRayCircle(xO:getAdd(xD:getMul(nMove)), xD, cP, nR)
    drawInterface(xO, fN, 5)
  end; drawRay(xO, xD, 5)
end

com.logSkipAdd("complex.getReflectRayRay: Angle mismatch")
com.logSkipAdd("complex.getRefractRayRay: Normal mismatch")

open("Complex rays circle intersect demo")
size(W, H)
zero(0, 0)
updt(false) -- disable auto updates

scOpe:Draw(true, true, true)

cRay1, cRay2, drw, rad = {}, {}, true, 0

while true do
  wait(0.2)
  local key = char()
  local lx, ly = clck('ld')
  local rx, ry = clck('rd')
  if(lx and ly and #cRay1 < 2) then -- Reverse the interval conversion polarity
    lx = intX:Convert(lx,true):getValue() -- It helps by converting x,y from positive integers to the interval above
    ly = intY:Convert(ly,true):getValue()
    local C = cmp.getNew(lx, ly)
    cRay1[#cRay1+1] = C; C:Action("xy", clBlu)
    if(#cRay1 == 2) then cRay1[1]:Action("ab", cRay1[2], clBlu) end
  elseif(rx and ry and #cRay2 < 2) then -- Reverse-convert x, y position to a complex number
    rx = intX:Convert(rx,true):getValue()
    ry = intY:Convert(ry,true):getValue()
    local C = cmp.getNew(rx, ry); C:Action("xy", clRed); cRay2[#cRay2+1] = C
    if(#cRay2 == 2) then cRay2[1]:Action("ab", cRay2[2], clRed)
      rad = (cRay2[2] - cRay2[1]):getNorm()
      scOpe:setColorDir(clRed)
      scOpe:drawComplexCircle(cRay2[1], rad)
    end
  end
  if(drw and #cRay1 == 2 and #cRay2 == 2) then
    local cD = (cRay1[2]-cRay1[1])
    local cR, cN, cX = cmp.getReflectRayCircle(cRay1[1], cD, cRay2[1], rad)
    if(cR) then local nL = cD:getNorm()
      drawTraceRayCircle(cRay1[1], cD, cRay2[1], rad)
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
