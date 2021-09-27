require("turtle")
local cmp = require("complex")
local col = require("colormap")
local crt = require("chartmap")
local sig = require("signals")

local xySize = 3
local dX, dY = 1,1
local greyLevel  = 200
local  W,  H = 1800, 800
local minX, maxX = -110, 110
local minY, maxY = -50, 50
local clBlu = colr(col.getColorBlueRGB())
local clRed = colr(col.getColorRedRGB())
local clBlk = colr(col.getColorBlackRGB())
local clGry = colr(greyLevel,greyLevel,greyLevel)
local clGrn = colr(col.getColorGreenRGB())
local clMgn = colr(col.getColorMagenRGB())
local intX  = crt.New("interval","WinX", minX, maxX, 0, W)
local intY  = crt.New("interval","WinY", minY, maxY, H, 0)
local scOpe = crt.New("scope"):setInterval(intX, intY)
      scOpe:setSize(W, H):setColor(clBlk, clGry):setBorder():setDelta(dX, dY)
local trWav = crt.New("tracer","Wave"):setInterval(intX,intY):Cache(450):setSizeVtx(1)
local vDwn = cmp.getNew(0,(minY-maxY))
local oDwn = cmp.getNew(0,maxY)

local function drawComplexLine(S, E, Cl)
  local x1 = intX:Convert(S:getReal()):getValue()
  local y1 = intY:Convert(S:getImag()):getValue()
  local x2 = intX:Convert(E:getReal()):getValue()
  local y2 = intY:Convert(E:getImag()):getValue()
  pncl(Cl); line(x1, y1, x2, y2)
end

local function drawComplex(C, Cl)
  local x = intX:Convert(C:getReal()):getValue()
  local y = intY:Convert(C:getImag()):getValue()
  pncl(Cl); rect(x-xySize,y-xySize,2*xySize+1,2*xySize+1)
end

cmp.setAction("xy", drawComplex)
cmp.setAction("ab", drawComplexLine)

--local w = sig.New("wiper",5, 0.03, 0, 0.1):setOrg(-55,0):toRand(50):Dump()
--local w = sig.New("wiper",35, 0.05, 0, 0.1):setOrg(-90,0):toSaw(20):Dump()
--local w = sig.New("wiper",15, 0.02, 0, 0.1):setOrg(-55,0):toSquare(20):Dump()
--local w = sig.New("wiper",25, 0.05, 0, 0.1):setOrg(-55,0):toTriangle(10):Dump()

local w = sig.New("wiper",15, 0.02, 0, 0.1):setOrg(-55,0):toSquare(20):Dump()

if(w) then
  open("FFT vector wiper graphing")
  size(W, H); zero(0, 0)

  updt(false) -- disable auto updates
  scOpe:Draw(true, true, true)
  local scrShot = snap() -- store snapshot
  local oS, oE = cmp.getNew(), cmp.getNew()
  local oT, bD = cmp.getNew(), false

  while(true) do
    undo(scrShot); w:Update()
    local vTip = w:getTip()
    oS:Set(oE); oE:Set(vTip)
    if(not bD) then bD = true else
      oE:Action("ab", oS, clMgn)
    end; scrShot = snap() -- Below that point items are deleted from the frame
    oT:Set(vTip):ProjectRay(oDwn, vDwn)
    w:Draw("ab", clRed)
    trWav:Move(2):Write(0, oT:getImag()):Draw(clGrn)
    oT:Action("xy", clBlu)
    oT:Action("ab", vTip, clBlu)
    oE:Action("xy", clBlu)
    updt(); wait(0.001)
  end
  
  wait()
else
  print("Remove the comment in front of FFT wiper generator")
end

