require("turtle")
local crt = require("chartmap")
local cmp = require("complex")
local col = require("colormap")

io.stdout:setvbuf("no")

local W, H = 800, 400

open("Complex ballistics")
size(W,H)
zero(0, 0)
updt(false) -- disable auto updates

local drgCoef = 2
local nMarg   = 30
local nMaxItr = 1000
local nTimeFR = 0.1
local drwText = true
local V0 = cmp.getNew(200,200):Print("%7.3f","Initial velocity : {","}\n")
local P  = cmp.convNew("0+j0"):Print("%7.3f","Position         : {","}\n")
local G  = cmp.convNew({0,-9.8}):Print("%7.3f","Gravity          : {","}\n")
local V  = cmp.convNew(V0):Print("%7.3f","Moment velocity  : {","}\n")
local Wv = cmp.convNew(-3,3):Print("%7.3f","Wind velocity    : {","}\n")
local minX, maxX, minY, maxY, traJ = 0, 0, 0, 0, {cmp.getNew(P)}

for ID = 1, nMaxItr do
  traJ[ID] = P:getNew()
  if(P:getImag() < 0) then break end
  V:Add(G)                         -- Apply gravity
  V:Add(V:getUnit():Mul(-drgCoef)) -- Calculate drag
  V:Add(Wv)                        -- Apply wind
  P:Add(V)                         -- Calcilate position
  local xP, yP = P:getParts()
  if(xP >= maxX) then maxX = xP end
  if(yP >= maxY) then maxY = yP end
  if(xP <= minX) then minX = xP end
  if(yP <= minY) then minY = yP end
end

local oX,aN = cmp.getNew(1), 0
local clGrn = colr(col.getColorGreenRGB())
local intX  = crt.New("interval","WinX", minX, maxX, nMarg, W-nMarg)
local intY  = crt.New("interval","WinY", minY, maxY, H-2*nMarg, 0)
local trAj  = crt.New("tracer","Trajectory"):setInterval(intX, intY)
local zEro  = intY:Convert(0):getValue()

line(0, zEro, W, zEro)

for ID = 1, #traJ do wait(nTimeFR)
  local cPos = traJ[ID]
  if(traJ[ID+1]) then
    aN = oX:getAngDegVec(traJ[ID+1] - traJ[ID])-90 end
  local Re, Im = cPos:Round(0.001):getParts()
  trAj:Write(Re, Im):Draw(clGrn)
  if(drwText) then intX:Convert(Re); intY:Convert(Im)
    text(tostring(cPos),aN,intX:getValue(),intY:getValue()) end
  updt()
end

wait()
