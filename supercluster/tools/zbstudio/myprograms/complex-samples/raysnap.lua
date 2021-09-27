require("turtle")
local cmp = require("complex")
local com = require("common")
local col = require("colormap")
local crt = require("chartmap")

local dX,dY = 1,1
local W , H = 600, 600
local minX, maxX = -8, 8
local minY, maxY = -8, 8
local greyLevel  = 200
local intX  = crt.New("interval","WinX", minX, maxX, 0, W)
local intY  = crt.New("interval","WinY", minY, maxY, H, 0)
local clOrg = colr(col.getColorBlueRGB())
local clRel = colr(col.getColorRedRGB())
local clBlk = colr(col.getColorBlackRGB())
local clGrn = colr(col.getColorGreenRGB())
local clGry = colr(greyLevel,greyLevel,greyLevel)
local clMgn = colr(col.getColorMagenRGB())
local scOpe = crt.New("scope"):setInterval(intX, intY):setBorder(minX, maxX, minY, maxY)
      scOpe:setSize(W, H):setColor(clBlk, clGry):setDelta(dX, dY)
      
com.logStatus("Create a circle to intersect using the right mouse button (RED)")
com.logStatus("Create a ray to intersect using the left mouse button (BLUE)")
com.logStatus("By clicking on the chart the point selected will be drawn")
com.logStatus("On the coordinate system the OX and OY axises are drawn in black")
com.logStatus("The distance between every grey line on X is: "..tostring(dX))
com.logStatus("The distance between every grey line on Y is: "..tostring(dY))
com.logStatus("Press escape to clear all rays and refresh the coordinate system")

local function drawComplex(C, Cl)
  scOpe:setColorDir(Cl)
  scOpe:drawComplexPoint(C)
end

local function drawComplexLine(S, E, Cl)
  scOpe:setColorDir(Cl)
  scOpe:drawComplex(S, E)
end

cmp.setAction("xy", drawComplex)
cmp.setAction("ab", drawComplexLine)

open("Complex rays direction snap demo")
size(W, H)
zero(0, 0)
updt(false) -- disable auto updates

scOpe:Draw(true, true, true)

cRay1, cRay2, drw, R1 = {}, {}, true, 0

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
    if(#cRay1 == 2) then cRay1[1]:Action("ab", cRay1[2], clOrg)
      R1 = cRay1[2]:getSub(cRay1[1]):getNorm()
      scOpe:setColorDir(clOrg)
      scOpe:drawComplexCircle(cRay1[1], R1)
      scOpe:setColorDir(clMgn)
    end
  elseif(rx and ry and #cRay2 < 2) then -- Reverse-convert x, y position to a complex number
    rx = intX:Convert(rx,true):getValue()
    ry = intY:Convert(ry,true):getValue()
    local C = cmp.getNew(rx, ry)
    C:Action("xy", clRel); cRay2[#cRay2+1] = C
    if(#cRay2 == 2) then cRay2[1]:Action("ab", cRay2[2], clRel) end
  end
  if(drw and #cRay1 == 2 and #cRay2 == 2) then
    local cD1, cD2 = (cRay1[2] - cRay1[1]), (cRay2[2] - cRay2[1])
    local cS , nK = cmp.getSnapRayRay(cRay1[1], cD1, cRay2[1], cD2, scOpe)
    if(cS) then local onOne, onTwo
      cS:Action("ab", cRay1[1], clGrn)
      cS:Action("xy", clGrn)
      com.logStatus("Ray snappred at : "..cS)
      com.logStatus("Iterations used : "..nK)
      com.logStatus("Vector target   : "..cD1:getNorm())
      com.logStatus("Lenght actual   : "..(cS - cRay1[1]):getNorm())
      onOne = cS:isAmongLine(cRay1[1], cRay1[2])
      onTwo = cS:isAmongLine(cRay2[1], cRay2[2])
      com.logStatus("The point is "..(onOne and "ON" or "OFF").." the first line (BLUE)")
      com.logStatus("The point is "..(onTwo and "ON" or "OFF").." the second line (RED)")
      onOne = cS:isAmongLine(cRay1[1], cRay1[2], true)
      onTwo = cS:isAmongLine(cRay2[1], cRay2[2], true)
      com.logStatus("The point (full-size) is "..(onOne and "ON" or "OFF").." the first line (BLUE)")
      com.logStatus("The point (full-size) is "..(onTwo and "ON" or "OFF").." the second line (RED)")
      onOne = cS:isAmongRay(cRay1[1], cRay1[2]-cRay1[1])
      onTwo = cS:isAmongRay(cRay2[1], cRay2[2]-cRay2[1])
      com.logStatus("The point is "..(onOne and "ON" or "OFF").." the first ray (BLUE)")
      com.logStatus("The point is "..(onTwo and "ON" or "OFF").." the second ray (RED)")
      onOne = cS:isAmongRay(cRay1[1], cRay1[2]-cRay1[1],true)
      onTwo = cS:isAmongRay(cRay2[1], cRay2[2]-cRay2[1],true)
      com.logStatus("The point (full-size) is "..(onOne and "ON" or "OFF").." the first ray (BLUE)")
      com.logStatus("The point (full-size) is "..(onTwo and "ON" or "OFF").." the second ray (RED)")  
    else
      com.logStatus("Ray being snapped is too short")
    end
    drw = false
  end
  if(key == 27) then -- The user hits esc
    scOpe:setColorDir(clOrg)
    wipe(); drw = true
    cRay1[1], cRay1[2] = nil, nil
    cRay2[1], cRay2[2] = nil, nil; collectgarbage()
    scOpe:Draw(true, true, true)
  end
  updt()
end

wait()
