local loadString = load

require("wx")
require("turtle")
local common  = require("common")
local complex = require("complex")
local chartmap = require("chartmap")
local colormap = require("colormap")

io.stdout:setvbuf("no")

local nFunc = 1

local tFunc = {
  {"(z^2 - 1)*(z - 2 - i)^2/(z^2 + 2 + 2*i)",4},
  {"(z^3 - 1)", 1.3},
  {"(1/(1+z))", 5},
  {"(1/(1+z^2))", 5},
  {"(1/(1-(z:getReal()+i*z:getImag())^2))", 5}
} 

-- Tinker stuff
local nAlp = 0.6
local dX, dY = 1,1
local  W,  H = 500, 500

-- Automatic stuff
local nRan = tFunc[nFunc][2]
local cI = complex.getNew(0,1) -- 0+i
local intX = chartmap.New("interval","WinX", -nRan, nRan, 0, W)
local intY = chartmap.New("interval","WinY", -nRan, nRan, H, 0)
local scOpe = chartmap.New("scope"):setInterval(intX, intY):setSize():setBorder()
      scOpe:setColor():setDelta(dX, dY):Draw(true, true, true):setSizeVtx(0)

local fFoo, sErr = loadString("return function(z, i) return "..tFunc[nFunc][1].." end")
if(not fFoo) then print("Load("..tostring(fFoo).."): "..sErr); return end
bSuc , fFoo = pcall(fFoo)
if(not bSuc) then print("Make("..tostring(bSuc).."): "..fFoo); return end

open("Complex domain coloring")
size(W, H); zero(0, 0)
updt(false) -- disable auto updates

scOpe:Draw(false, false, false)

local function getComplexColor(i,j)
  local nI = intX:Convert(i,true):getValue()
  local nJ = intY:Convert(j,true):getValue()
  local vC = complex.getNew(nI,nJ)
  local vF = fFoo(vC, cI)
  local nM, nP = vF:getPolar()
  local hslH = complex.toDeg(nP)
        hslM = ((hslH < 0) and (hslH+360) or hslH)
  local hslS, hslL = 1, (1-nAlp^nM)
  local r, g, b = colormap.getColorHSL(hslM, hslS, hslL)
  if(vF:isNan()) then -- Interpolate {0,0} as up-down-left-right
    local vC1, vF1, r1, g1, b1 = getComplexColor(i-1,j)
    local vC2, vF2, r2, g2, b2 = getComplexColor(i+1,j)
    local vC3, vF3, r3, g3, b3 = getComplexColor(i,j-1)
    local vC4, vF4, r4, g4, b4 = getComplexColor(i,j+1)
    r = (r1 + r2 + r3 + r4) / 4
    g = (g1 + g2 + g3 + g4) / 4
    b = (b1 + b2 + b3 + b4) / 4
  end; return vC, vF, r, g, b
end

for j = 0, H do
  for i = 0, W do -- Convert coordinates to complex mapping
    local vC, vF, r, g, b = getComplexColor(i, j)
    scOpe:drawPointXY(vC:getReal(),vC:getImag(),colr(r, g, b))
  end; updt()
end

wait()
