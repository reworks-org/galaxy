local common = require("common")

local S = 4 -- Start node ID in towns list
local T = {"A", "B", "C", "D"}
local W = { 
  A = {A = 0 , B = 20, C = 38, D = 61},
  B = {A = 20, B = 0 , C = 1, D = 34},
  C = {A = 27, B = 60, C = 0 , D = 16},
  D = {A = 35, B = 34, C = 12, D = 0 } 
}

  
local function trSum(vP)
  local nW = 0
  for iD = #vP, 1, -1  do
    local kB= vP[iD]
    local kE = (vP[iD+1] or vP[1])
    nW = nW + (W[kB][kE] or math.huge)
  end; return nW
end

local tP, iK = common.getPermute(unpack(T)), 1

for iP = 1, #tP do
  if(tP[iP][#T] == T[S]) then
    tP[iP], tP[iK] = tP[iK], tP[iP]
    tP[iP] = nil; iK = (iK + 1)
  else tP[iP] = nil end
end

for iP = 1, #tP do
  print(iP.." > "..table.concat(tP[iP],"-").." > "..trSum(tP[iP]))
end
