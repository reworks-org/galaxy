require("turtle")

local crt = require("chartmap")
local sig = require("signals")
local col = require("colormap")

io.stdout:setvbuf("no")

-- Manual part
local W, H       = 1200, 800 -- window dimensions
local minC, maxC = -25, 25   -- control limists
local stVal      = 15        -- Step responce final value               
local To         = 0.05      -- Sampling time
local wAit       = 0.01      -- Simulation speed-up
local tScale     = 30        -- End time relative to the sampling time
-- local tFixed     = 5      -- Fixed time
-- Automated part
local curTm, yTx, dyTx = 0, 0, 15
local clRed = colr(col.getColorRedRGB())
local clGrn = colr(col.getColorGreenRGB())
local clBlu = colr(col.getColorBlueRGB())
local clBlk = colr(col.getColorBlackRGB())
local endTm = tFixed and tFixed or (tScale * To)
local intX  = crt.New("interval","WinX",  0,endTm, 0, W)
local intY  = crt.New("interval","WinY", -1, maxC , H, 0)
local APR   = sig.New("plant",To,{8},{7 , 3, 1},"Hesitating plant"):Dump()
local PID   = sig.New("control",To,"Lin-QR"):Setup({0.051, 0.009, 6, minC, maxC}):setStruct(false,false):Dump()
local trRef = crt.New("tracer","Ref"):setInterval(intX, intY)
local trCon = crt.New("tracer","Con"):setInterval(intX, intY)
local trPV  = crt.New("tracer","PV" ):setInterval(intX, intY)
local scOpe = crt.New("scope"):setInterval(intX, intY)
      scOpe:setSize(W, H):setColor(clBlk, clGry):setBorder():setDelta(To, 1)
local function form(nV) return ("%03.2f"):format(nV) end
open("Trasition processes")
size(W,H)
zero(0, 0)
updt(false) -- disable auto updates

scOpe:Draw(false, false, true)

text("( Blue ) System reference", 0, 0, yTx); yTx = yTx + dyTx
text("( Green ) Control signal", 0, 0, yTx); yTx = yTx + dyTx
text("( Black ) Plant uncontrolled step responce", 0, 0, yTx); yTx = yTx + dyTx
text("( Red ) Plant controlled step responce", 0, 0, yTx); yTx = yTx + dyTx

local pvv, con, ref = 0, 0, 100

curTm = 0
while(curTm <= endTm) do
  if(curTm > 0.1 * endTm) then con = stVal else con = 0 end
  wait(wAit)
  pvv = APR:Process(con):getOutput()
  trPV:Write(curTm, pvv):Draw(clBlk)
  print("("..form(curTm)..") "..form(ref).." > "..form(pvv).." > "..form(con))
  curTm = curTm + To; updt()
end; APR:Reset(); trPV:Reset(); wait(0.5)

curTm, pvv = 0, 0
while(curTm <= endTm) do
  wait(wAit)
  if(curTm > 0.1 * endTm) then ref = stVal else ref = 0 end
  trRef:Write(curTm, ref):Draw(clBlu)
  con = PID:Process(ref,pvv):getControl()
  trCon:Write(curTm,con):Draw(clGrn)
  pvv = APR:Process(con):getOutput()
  trPV:Write(curTm, pvv):Draw(clRed)
  print("("..form(curTm)..") "..form(ref).." > "..form(pvv).." > "..
             form(con).." : {"..table.concat({("%5.2f, %5.2f, %5.2f"):format(PID:getTerms())}).."}")
  curTm = curTm + To; updt()
end

wait()
