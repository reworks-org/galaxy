require("turtle")

local common   = require("common")
local colormap = require("colormap")
local chartmap = require("chartmap")

local suc, out

suc, out = common.getCall("pi", 100)
common.logStatus("Number PI     : "..tostring(out))
suc, out = common.getCall("exp", 100)
common.logStatus("Exponent base : "..tostring(out))
suc, out = common.getCall("phi", 100)
common.logStatus("Golden ratio  : "..tostring(out))
suc, out = common.getCall("lol", 100)
common.logStatus("Something     : "..tostring(out))

function approxPHI(itr, top)
  if(top == itr) then return 1 end
  return (1 + (1 / approxPHI(itr+1, top)))
end

function outRet(itr)
  return approxPHI(0, itr)
end

common.setCall("Custom user functional",approxPHI, outRet)
suc, out = common.getCall("Custom user functional", 100)
common.logStatus("Custom estimator example: "..out)


local logStatus = common.logStatus
local  W,  H = 800, 400
local minX, maxX = 0, 25
local minY, maxY = 1, 2.05
local intX  = chartmap.New("interval","WinX", minX, maxX, 0, W)
local intY  = chartmap.New("interval","WinY", minY, maxY, H, 0)
local trEst = chartmap.New("tracer","Est"):setInterval(intX, intY)
local scEst = math.floor(intY:Convert(outRet(maxX)):getValue())
local clBlu = colr(colormap.getColorBlueRGB())
local clRed = colr(colormap.getColorRedRGB())

open("Functional estimator")
size(W,H); zero(0, 0); updt(false) -- disable auto updates

pncl(clBlu); line(0, scEst, W, scEst)

for I = minX, maxX do
  suc, out = common.getCall("Custom user functional", I)
  trEst:Write(I, out):Draw(clRed)
  local xc, yc = trEst:getChart()
  local xv, yv = trEst:getValue()
  local ang  = ((I==0 or I==1) and 0  or 90)
  local adjx = ((I==0 or I==1) and 8 or -7)
  local adjy = ((I==0 or I==1) and -17  or -7)
  text(("%10.9f"):format(yv),ang,xc+adjx, yc+adjy)
  
  updt()
end

wait()
