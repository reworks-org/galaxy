require("turtle")
local crt = require("chartmap")
local col = require("colormap")
local com = require("common")

local  W,  H = 1800, 400
local dX, dY = 4,0.12
local greyLevel  = 200
local minX, maxX = -180, 180
local minY, maxY = -1.2, 1.2
local intX  = crt.New("interval","WinX", minX, maxX, 0, W)
local intY  = crt.New("interval","WinY", minY, maxY, H, 0)
local clBlu = colr(col.getColorBlueRGB())
local clRed = colr(col.getColorRedRGB())
local clBlk = colr(col.getColorBlackRGB())
local clGrn = colr(col.getColorGreenRGB())
local clGry = colr(greyLevel,greyLevel,greyLevel)
local clMgn = colr(col.getColorMagenRGB())
local scOpe = crt.New("scope"):setInterval(intX, intY)
      scOpe:setSize(W, H):setColor(clBlk, clGry):setBorder():setDelta(dX, dY)
local tF, gnD2R   = {}, (math.pi / 180)

open("Piston internal signals. Sign as (BLUE), sine-wave as (RED) and tiangular as (GREEN)")
size(W, H); zero(0, 0)
updt(false) -- disable auto updates

local tT, tR, tO = {0, 45,90,135,180,-180,-135,-90,-45,-0}, {}, {}

tF[1] = {function(R, H) local nA = com.getAngNorm(R - H)
  local nB, aA = ((nA >= 0) and 1 or -1), math.abs(nA)
  return ((aA == 0 or aA == 180) and 0 or nB)
end, clBlu}

tF[2] = {function(R, H)
  return math.sin(gnD2R * com.getAngNorm(R - H))
end, clRed}

tF[3] = {function(R, H) local nN = com.getAngNorm(R - H)
  local nA, nM = -com.getAngNorm(nN + 180), math.abs(nN)
  return (((nM > 90) and nA or nN) / 90)
end, clGrn}

for r = -180, 180 do table.insert(tR, r) end

for i = 1, #tT do local h = tT[i]; wipe()
  text("Testing header ID "..("%3d"):format(i).." origin at "..("%3d"):format(h),0,0,0)
  com.tableClear(tO); scOpe:Draw(true, true, true)
  for k = 1, #tF do
    for j = 1, #tR do tO[j] = tF[k][1](tR[j], tT[i]) end
    scOpe:setColorDir(tF[k][2]):drawGraph(tO, tR)
  end
  updt(); wait(1)
end

wait()