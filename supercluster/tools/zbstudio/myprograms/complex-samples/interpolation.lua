require("wx")
require("turtle")
local com = require("common")
local col = require("colormap")
local crt = require("chartmap")
local cmp = require("complex").extend()

-- 1 : Nearest neighbour ( ZoH )
-- 2 : Bilinear interpolation ( FoH )
-- 3 : Bicubic interpolation ( SoH )
local nOH = 2

local W,  H = 500, 500
local greyLevel  = 200
local gnDrwStep = 0.002
local minX, maxX = 0, 1
local minY, maxY = 0, 1
local dX, dY, xySize, bStop = 1, 1, 3, false
local clRed = colr(col.getColorRedRGB())
local clBlk = colr(col.getColorBlackRGB())
local clGry = colr(col.getColorPadRGB(greyLevel))
local intX  = crt.New("interval","WinX", minX, maxX, 0, W)
local intY  = crt.New("interval","WinY", minY, maxY, H, 0)
local scOpe = crt.New("scope"):setBorder(minX, maxX, minY, maxY)
      scOpe:setSize(W, H):setColor(clBlk, clGry):setInterval(intX, intY):setDelta(dX, dY):setSizeVtx(0)
local tIntn = {"Neighbour", "Bilinear", "Bicubic"}

local tMap = {
  {0  ,  0,180},
  {0  ,255,255},
  {0  ,255,  0},
  {255,255,  0},
  {255,  0,  0},
  {180,  0,  0}
}

col.setColorMap("interp", col.getColorMapGradient(tMap, 20))
local nTot = col.getColorMap("interp").Size

com.logStatus("https://en.wikipedia.org/wiki/Bilinear_interpolation")
com.logStatus("Interpolated pallete map size: "..nTot); nTot = nTot - 1

local tI = {
  F   = {1   , 0.5,   0,   1},
  Fx  = {3   ,  -5,   7, 0.5},
  Fy  = {8   , 3.5, 4.1,  -1},
  Fxy = {-2.6, 1.5, 0.6,  -8}
}

local cZ = cmp.getNew()
local tArea = {cZ:getNew(0,1),
               cZ:getNew(1,1),
               cZ:getNew(0,0),
               cZ:getNew(1,0), tI, nOH, false}

open("Complex surface interpolation : "..tIntn[nOH])
size(W, H)
zero(0, 0)
updt(false) -- disable auto updates
com.setTic()

for j = 1, 0, -gnDrwStep do
  if(not bStop) then
    for i = 0, 1, gnDrwStep do  
      local nV = cZ:getNew(i,j):getInterpolation(unpack(tArea))
      if(not nV) then com.logStatus("Did you extend the complex module with `.extend()` ?")
        bStop = true; break
      else
        local nI = com.getClamp(com.getRound(nV*nTot, 1), 1, nTot)
        local r, g, b = col.getColorMap("interp", nI)
        scOpe:drawPointXY(i, j, colr(r, g, b))
      end
    end; updt()
  end
end

com.logStatus("Elapsed: "..com.getToc())

-- To prodice a PNG snapshot, uncomment the line below
-- save(com.stringGetChunkPath().."snapshot")

wait()
