require("wx")
require("turtle")
local com = require('common')

local nW, nH = 1000, 600

local tHan = {
  ID = {"A", "B", "C"},
  A = {},
  B = {},
  C = {},
  Set = {
    ["nDisk"] = 10, -- The amount of disks to get moved
    ["nWait"] = 0.01, -- The amout of time to wait before a move
    ["clBase"] = colr(139,069,019), -- Base pylon color
    ["clDisk"] = colr(000,255,000), -- Disks color
    ["dW"] = 10, -- The distance between all pylons and window edges X
    ["dH"] = 20, -- The distance between all pylons and window edges Y
    ["dP"] = 15, -- The width of one pylon stick or plate
    ["dL"] = 11, -- Text letter width
    ["dT"] = 19, -- Pylon name text offset delta
    ["wT"] = 70, -- Fixed width of the first, top, smallest disk
    ["wB"] = 0,  -- Fixed width of the last, bottom, largest disk
    ["dD"] = 0,  -- The disk size width delta until the bottom disk is reached
    ["bW"] = 0,  -- Pylon base support width
    ["bH"] = 0,  -- Pylon hight
    ["tS"] = os.clock() -- Start time of the process
  }
}

local function newPylon(iD, nW, nH)
  return {ID=iD,W=nW,H=nH,M=0,P=0}
end

local function drawState()
  local tSet = tHan.Set
  local dP = tSet["dP"]
  local clDisk = tSet["clDisk"]
  for iD = 1, #tHan.ID do
    local key = tHan.ID[iD]
    local val = tHan[key]
    for iK = 1, #val do local dsk = val[iK]
      local dX, dY = (val.C-dsk.W/2), (val.Y-iK*(dP+dsk.H))
      pncl(clDisk); rect(dX,dY,dsk.W,dsk.H)
      text(com.getRound(dsk.P,0.001).."%",0,dX+1,dY-1)
    end
  end
end

local function drawBase()
  local dW = tHan.Set["dW"]
  local dH = tHan.Set["dH"]
  local dP = tHan.Set["dP"]
  local dT = tHan.Set["dT"]
  local bW = tHan.Set["bW"]
  local bH = tHan.Set["bH"]
  local tS = tHan.Set["tS"]
  local dL = tHan.Set["dL"]
  local nM = tHan.Set["nMove"]
  local clBase = tHan.Set["clBase"]; wipe()
  for iD = 1, #tHan.ID do
    local key = tHan.ID[iD]
    local val = tHan[key]
    local xP = (val.X + (bW / 2)) - dP/2
    pncl(clBase); rect(xP,dH,dP,bH)
    text(tHan.ID[iD],0,xP+((dP-dL)/2),dH-dT)
    pncl(clBase); rect(val.X,val.Y,bW,dP)
    text("Time: "..tostring(os.clock()-tS).."\n".."Move: "..tostring(nM))
  end
end

local function doMove(ID, tS, tD)
  local nWait = tHan.Set["nWait"]
  local nMove = tHan.Set["nMove"] + 1
  local nD, nS = #tD, #tS
  if(nWait and nWait > 0) then wait(nWait) end
  tS[nS].M = tS[nS].M + 1
  tS[nS].P = ((tS[nS].M / nMove) * 100)
  tD[nD+1] = tS[nS]; tS[nS] = nil
  tHan.Set["nMove"] = nMove
  drawBase(); drawState(); updt()
end

local function doHanoj(ID, tS, tD, tT)
  if(ID == 1) then doMove(ID, tS, tD) else
    doHanoj(ID-1, tS, tT, tD)
    doMove(ID, tS, tD)
    doHanoj(ID-1, tT, tD, tS)
  end
end

local function goHanoj(tH)
  local tSet = tHan.Set; tHan.Set["nMove"] = 0
  local nD = tHan.Set["nDisk"]
  local A,B,C = tH[tH.ID[1]], tH[tH.ID[2]], tH[tH.ID[3]]
  tSet["bW"] = ((nW - 4*tSet["dW"]) / 3)
  tSet["bH"] = (nH - 2*tSet["dH"])
  tSet["wB"] = (tSet["bW"] - 2*tSet["dP"])
  tSet["dD"] = (tSet["wB"] - tSet["wT"])/(nD-1)
  A[1] = newPylon(1,tSet["wT"],tSet["dP"])
  for iD = 2, (nD-1) do
    A[iD] = newPylon(iD,(A[iD-1].W+tSet["dD"]),tSet["dP"])
  end
  A[nD] = newPylon(nD,tSet["wB"],tSet["dP"])
  com.tableArrReverse(A)
  local xB, yB = tSet["dW"], (nH - tSet["dH"] - tSet["dP"])
  for iD = 1, #tHan.ID do
    local key = tHan.ID[iD]
    local val = tHan[key]
    val.name, val.X, val.Y = key, xB, yB
    val.C = (xB + tSet["bW"]/2)
    xB = xB + tSet["dW"] + tSet["bW"]
  end  
  drawBase(); drawState();  
  doHanoj(nD, A, C, B)
  drawBase(); drawState();
end

open("The towers of Hanoj")
size(nW, nH); zero(0, 0)
updt(false) -- disable auto updates
goHanoj(tHan)
wait()
