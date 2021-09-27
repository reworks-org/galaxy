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

cmp.setAction("xy", drawComplex)
cmp.setAction("ab", drawComplexLine)

logStatus("Create a primary line to project on using the left mouse button (BLUE)")
logStatus("Create a point to project on the line using the right mouse button (RED)")
logStatus("By clicking on the chart the point selected will be drawn")
logStatus("On the coordinate system the OX and OY axises are drawn in black")
logStatus("The distance between every grey line on X is: "..tostring(dX))
logStatus("The distance between every grey line on Y is: "..tostring(dY))
logStatus("Press escape to clear all rays and refresh the coordinate system")

open("Complex point projection demo")
size(W,H)
zero(0, 0)
updt(false) -- disable auto updates

scOpe:Draw(true, true, true)

tDir, cOrg, drw = {}, cmp.getNew(), false

while true do
  wait(0.2)
  local key = char()
  local lx, ly = clck('ld')
  local rx, ry = clck('rd')
  if(lx and ly and #tDir < 2) then -- Reverse the interval conversion polarity
    lx = intX:Convert(lx,true):getValue() -- It helps by converting x,y from positive integers to the interval above
    ly = intY:Convert(ly,true):getValue() 
    local C = cmp.getNew(lx, ly)
    tDir[#tDir+1] = C; C:Action("xy", clOrg)
    if(#tDir == 2) then drw = true end
  elseif(drw and rx and ry) then -- Reverse the interval and convert x, y image position to a complex
    rx = intX:Convert(rx,true):getValue() 
    ry = intY:Convert(ry,true):getValue(); cOrg:Set(rx, ry)
    cOrg:Action("ab", tDir[1], clOrg)
    cOrg:Action("ab", tDir[2], clOrg)
    tDir[1]:Sub(cOrg); tDir[2]:Sub(cOrg)
    cOrg:Action("xy", clRel)
    local nS, nD = tDir[1]:getNorm(), tDir[2]:getNorm()
    local cB = tDir[1]:getBisect(tDir[2]):Div((nS * nD) / 16)
    logStatus("Bisector of "..tDir[1].." and "..tDir[2].." is equal to "..cB)
    cB:Add(cOrg):Action("xy", clMgn)
    cB:Action("ab", cOrg, clMgn); drw = false
  end
  if(key == 27) then -- The user hits esc
    wipe(); drw = true -- Wipe all the drawing and redraw the coordinate system
    tDir[1], tDir[2] = nil, nil; collectgarbage()
    scOpe:Draw(true, true, true)
  end
  updt()
end

wait();
