-- Copyright (C) 2017 Deyan Dobromirov
-- Implements matrix manipulations

if not debug.getinfo(3) then
  print("This is a module to load with `local matrix = require('matrix')`.")
  os.exit(1)
end

local common          = require("common")
local tostring        = tostring
local tonumber        = tonumber
local matrix          = {}
local metaMatrix      = {}
local dataMatrix      = {}
local isZero          = common.isZero
local isTable         = common.isTable
local isString        = common.isString
local isNumber        = common.isNumber
local logStatus       = common.logStatus
local getClamp        = common.getClamp
local getType         = common.getType
local getPick         = common.getPick
local copyItem        = common.copyItem
local stringPadL      = common.stringPadL
local convSignString  = common.convSignString
local randomGetNumber = common.randomGetNumber
metaMatrix.__type     = "matrix.matrix"
metaMatrix.__index    = metaMatrix
dataMatrix.__symch    = "x"

function matrix.extend()
  dataMatrix.__extlb = require("extensions").matrix; return matrix
end

function matrix.isValid(oM)
  return (getmetatable(oM) == metaMatrix)
end

function matrix.getNew(tM)
  local mnR, mnC, self, mtData = 0, 0, {}
  setmetatable(self, metaMatrix)
  local extlb = dataMatrix.__extlb
  function self:getData() return mtData end
  function self:cpyData() return copyItem(mtData) end
  function self:getSize() return mnR, mnC end
  function self:getRows() return mnR end
  function self:getCols() return mnC end
  function self:setData(oM)
    if(matrix.isValid(oM)) then mtData = oM:cpyData()
    elseif(oM) then mtData = copyItem(oM)
    else --[[ Reinitialize mtData ]] end
    mnR, mnC = #mtData, #mtData[1]
    for iR = 1, mnR do for iC = 1, mnC do
      if(extlb) then mtData[iR][iC] = extlb.complexNew(mtData[iR][iC])
      else mtData[iR][iC] = (tonumber(mtData[iR][iC]) or 0) end
    end; end; return self
  end; return self:setData(tM)
end

function metaMatrix:isSymbolic()
  return isString(self:getData()[1][1])
end

function metaMatrix:isSquare()
  local nR, nC = self:getSize(); return (nR == nC)
end

function metaMatrix:isUnit()
  return true
end

function metaMatrix:Apply(fC, ...)
  local tData, nR, nC = self:getData(), self:getSize()
  for iR = 1, nR do for iC = 1, nC do
    local bS, nC = pcall(fC, tData, iR, iC, ...); if(not bS) then
      return logStatus("matrix.Apply: Failed {"..iR..","..iC.."}: "..nC, nil) end
    tData[iR][iC] = nC
  end; end; return self
end

function metaMatrix:getApply(fC, ...)
  return self:getNew():Apply(fC, ...)
end

function metaMatrix:Print(nS, sM)
  local sM = (sM and " "..tostring(sM) or "")
  local tData, nR, nC = self:getData(), self:getSize()
  logStatus("Matrix ["..nR.." x "..nC.."]"..sM)
  local nS = getClamp(tonumber(nS) or 0,0)
  for iR = 1, nR do local sL = ""
    for iC = 1, nC do
      sL = sL..", "..stringPadL(tostring(tData[iR][iC]), nS)
    end; logStatus("[ "..sL:sub(2,-1).." ]")
  end; return self
end

function metaMatrix:getNew(tM)
  return (tM and matrix.getNew(tM) or matrix.getNew(self:getData()))
end

--[[
  Negates all the elements of the matrix
]]
function metaMatrix:Neg()
  local tData, nR, nC = self:getData(), self:getSize()
  local extlb = dataMatrix.__extlb
  for iR = 1, nR do for iC = 1, nC do
    if(extlb) then tData[iR][iC]:Neg()
    else tData[iR][iC] = -tData[iR][iC] end
  end; end; return self
end

function metaMatrix:getNeg()
  return self:getNew():Neg()
end

--[[
  Modify the internal elements of the matrix to be used in symbolic mode
  Uses the flag /bS/ to toggle on/off the symbolic mode
]]
function metaMatrix:setSym(bS)
  local tData, nR, nC = self:getData(), self:getSize()
  local extlb = dataMatrix.__extlb
  for iR = 1, nR do for iC = 1, nC do
    if(bS) then
      if(extlb) then tData[iR][iC] = tostring(tData[iR][iC])
      else tData[iR][iC] = convSignString(tonumber(tData[iR][iC]) or 0) end     
    else -- When toggled convert back to a number
      if(extlb) then tData[iR][iC] = extlb.complexConvNew(tData[iR][iC])
      else tData[iR][iC] = (tonumber(tData[iR][iC]) or 0) end      
    end
  end; end; return self
end

--[[
  Modify destination row id /iD/, with the source id /iS/.
  Uses the coefficient nK to scale the linear system
  to adjust the destination row by id based on the source
]]
function metaMatrix:Modify(iD, iS, nK)
  local tData, nR, nC = self:getData(), self:getSize()
  if(not iS) then return logStatus("matrix.Modify: Source missing",nil) end
  if(not iD) then return logStatus("matrix.Modify: Destination missing",nil) end
  if(not nK) then return logStatus("matrix.Modify: Modifyer missing",nil) end
  local tS = tData[iS]; if(not iS) then
    return logStatus("matrix.Modify: Source invalid",nil) end
  local tD = tData[iD]; if(not tD) then
    return logStatus("matrix.Modify: Destination invalid",nil) end
  for iC = 1, nC do tD[iC] = tD[iC] + nK * tS[iC] end; return self
end

function metaMatrix:getModify(...)
  return self:getNew():Modify(...)
end

--[[
 * Scratches the matrix by row /nR/ and colum /nC/
 * and converts matrix to sub-matrix minor
]]
function metaMatrix:Minor(nR,nC)
  local tData = self:getData()
  local tM, cR, cC, eR, eC = {}, 1, 1, self:getSize()
  for iR = 1, eR do for iC = 1, eC do
    if(not (iR == nR or iC == nC)) then
      if(not tM[cR]) then tM[cR] = {} end
      tM[cR][cC] = tData[iR][iC]; cC = (cC + 1)
  end end; if(not (iR == nR or iC == nC)) then
  cC, cR = 1, (cR + 1); end; end; return self:setData(tM)
end

function metaMatrix:getMinor(...)
  return self:getNew():Minor(...)
end

function metaMatrix:getCofactor(nR,nC)
  return (-1)^(nR+nC)*(self:getMinor(nR,nC):getDet())
end

--[[
 * Calcolates matrix determinant by expanding on the row /vR/
]]
function metaMatrix:getDet(vR)
  local nR, nC, bS = self:getSize(); if(nR ~= nC) then
    return logStatus("matrix.getDet: Rectangle ["..nR.." x "..nC.."]", 0) end
  local tData = self:getData(); bS = isString(tData[1][1])
  if(not isTable(tData)) then return tData end
  if(nR == 1 and nC == 1) then return tData[1][1] end
  if(nR == 2 and nC == 2) then local R1, R2 = tData[1], tData[2]
    return ((R1[1] * R2[2]) - (R2[1] * R1[2])) end
  local vR = getClamp(tonumber(vR) or 1, 1)
  local tR, iR, nD = tData[vR], 1, 0
  for iC = 1, nC do nD = nD + tR[iC]*self:getCofactor(vR,iC) end; return nD
end

--[[
  Builds the haracteristic polinomial for matrix roots
]]
function metaMatrix:getPolynomial()
  local symch, oM = dataMatrix.__symch, self:getNew()
  return oM:setItemsSym(true):Apply(function(tData,iR,iC,sS) local vV = tData[iR][iC]
    return (iC==iR and "("..vV.."-"..sS..")" or "("..vV..")") end, symch):getDet()
end

function metaMatrix:Rand(nR, nC, nL, nU, vC) local tM = {}
  local nR = getClamp(tonumber(nR) or 1 ,1)
  local nC = getClamp(tonumber(nC) or nR,1)
  for iR = 1, nR do tM[iR] = {}; for iC = 1, nC do
    if(extlb) then tM[iR][iC] = extlb.complexGetRandom(nL, nU, vC)
    else tM[iR][iC] = randomGetNumber(nL, nU, vC) end
  end; end; return self:setData(tM)
end

function metaMatrix:getRand(...)
  return self:getNew():Rand(...)
end

--[[
 * Fill the matrix of size [nR, nC] with value vV
 * When bM is enabed fills only the main diagonal
]]
function metaMatrix:Fill(nR, nC, vV, bM)
  local tM, vR, vC, tData = {}
  local extlb = dataMatrix.__extlb
  if(nR or nC) then
   vR = getClamp(tonumber(nR) or 1 ,1)
   vC = getClamp(tonumber(nC) or nR,1)
  else tData, vR, vC = self:getData(), self:getSize() end
  for iR = 1, vR do if(not tData) then tM[iR] = {} end
    for iC = 1, vC do
      if((bM and iR==iC) or not bM) then
        if(extlb) then
          if(tData) then tData[iR][iC]:Set(vV)
          else tM[iR][iC] = extlb.complexNew():Set(vV) end
        else 
          if(tData) then tData[iR][iC] = vV
          else tM[iR][iC] = vV end
        end
      end
    end
  end; return (tData and self:setData() or self:setData(tM))
end

function metaMatrix:getFill(...)
  return self:getNew():Fill(...)
end

function metaMatrix:Zero(nR, nC)
  local extlb = dataMatrix.__extlb
  local nZ = (extlb and extlb.complexNew(0,0) or 0)
  return self:Fill(nR, nC, nZ)
end

function metaMatrix:getZero(...)
  return self:getNew():Zero(...)
end

function metaMatrix:Ones(nR, nC)
  local extlb = dataMatrix.__extlb
  local nO = (extlb and extlb.complexNew(1,0) or 1)
  return self:Fill(nR, nC, nO)
end

function metaMatrix:getOnes(...)
  return self:getNew():Ones(...)
end

function metaMatrix:Unit(nR, nC)
  local extlb = dataMatrix.__extlb
  local nO = (extlb and extlb.complexNew(1,0) or 1)
  return self:Zero():Fill(nR, nC, nO, true)
end

function metaMatrix:getUnit(...)
  return self:getNew():Unit(...)
end

function metaMatrix:Offset(oM, nK)
  local oR, oC = oM:getSize()
  local mR, mC = self:getSize()
  local mData, oData, nK = self:getData(), oM:getData(), (tonumber(nK) or 1)
  if(oR ~= mR) then return logStatus("matrix.Div: Row mismatch ["..mR.."x"..mC.."]+["..oR.."x"..oC.."]",nil) end
  if(oC ~= mC) then return logStatus("matrix.Div: Col mismatch ["..mR.."x"..mC.."]+["..oR.."x"..oC.."]",nil) end
  for iR = 1, mR do for iC = 1, mC do mData[iR][iC] = mData[iR][iC] + nK * oData[iR][iC] end end; return self
end

function metaMatrix:getOffset(...)
  return self:getNew():Offset(...)
end

function metaMatrix:Add(oM)
  return self:Offset(oM, 1)
end

function metaMatrix:getAdd(...)
  return self:getNew():Add(...)
end

function metaMatrix:Sub(oM)
  return self:Offset(oM, -1)
end

function metaMatrix:getSub(...)
  return self:getNew():Sub(...)
end

function metaMatrix:Exp()
  local mData = self:getData()
  local mR, mC = self:getSize()
  local extlb = dataMatrix.__extlb
  for iR = 1, mR do for iC = 1, mC do
    if(extlb) then mData[iR][iC]:Exp()
    else mData[iR][iC] = math.exp(mData[iR][iC]) end
  end end; return self
end

function metaMatrix:getExp()
  return self:getNew():Exp()
end

function metaMatrix:Log()
  local mData = self:getData()
  local mR, mC = self:getSize()
  local extlb = dataMatrix.__extlb
  for iR = 1, mR do for iC = 1, mC do
    if(extlb) then mData[iR][iC]:Log()
    else mData[iR][iC] = math.log(mData[iR][iC]) end
  end end; return self
end

function metaMatrix:getLog()
  return self:getNew():Log()
end

function metaMatrix:Mul(oM, bE)
  local extlb = dataMatrix.__extlb
  if(matrix.isValid(oM)) then
    local rA, cA = self:getSize()
    local rB, cB = oM:getSize(); if(not bE and cA ~= rB) then
      return logStatus("matrix.Mul: Dimension mismatch ["..rA.." x "..cA.."] * ["..rB.." x "..cB.."]",nil) end
    local tA, tB, tM = self:getData(), oM:getData(), {}
    for iA = 1, rA do if(not tM[iA]) then tM[iA] = {} end
      if(not bE) then
        for iB = 1, cB do local nV
          if(extlb) then nV = extlb.complexNew() else nV = 0 end
          for kB = 1, rB do nV = nV + (tA[iA][kB] * tB[kB][iB]) end
          tM[iA][iB] = nV
        end
      else
        for jA = 1, cA do local oB = ((tB[iA] and tB[iA][jA]) and tB[iA][jA] or 1)
          if(extlb) then vB = extlb.complexNew(oB)
          else vB = (tonumber(oB) or 1) end
          tM[iA][jA] = (tA[iA][jA] * vB)
        end
      end
    end; return self:setData(tM)
  else
    local nR, nC = self:getSize()
    local tA, nN = self:getData()
    if(extlb) then nN = extlb.complexNew(oM)
    else nN = (tonumber(oM) or 0) end
    for iR = 1, nR do for iC = 1, nC do
      if(extlb) then tA[iR][iC]:Mul(nN)
      else tA[iR][iC] = tA[iR][iC] * nN end
    end end; return self
  end
end

function metaMatrix:getMul(...)
  return self:getNew():Mul(...)
end

function metaMatrix:Trans()
  local tData, nR, nC = self:getData(), self:getSize()
  local nE = (nR > nC and nR or nC)
  for iC = 1, nE do for iR = 1, nE do if(iC < iR) then
    local vC = (tData[iC] and tData[iC][iR] or nil)
    local vR = (tData[iR] and tData[iR][iC] or nil)
    if(not tData[iC]) then tData[iC] = {} end
    if(not tData[iR]) then tData[iR] = {} end
    tData[iC][iR], tData[iR][iC] = vR, vC
  end; end; end;
  while(not tData[nE][1]) do tData[nE] = nil; nE = nE - 1 end
  return self:setData() -- Reinitialize data size
end

function metaMatrix:getTrans()
  return self:getNew():Trans()
end

function metaMatrix:Adj()
  local nR, nC = self:getSize(); if(nR ~= nC) then
    return logStatus("matrix.Adj: Rectangle ["..nR.." x "..nC.."]",nil) end
  local tData, tM = self:getData(), {}
  for iR = 1, nR do for iC = 1, nC do
    if(not tM[iR]) then tM[iR] = {} end
    tM[iR][iC] = self:getCofactor(iR,iC)
  end end; return self:setData(tM)
end

function metaMatrix:getAdj()
  return self:getNew():Adj()
end

function metaMatrix:Inv()
  local nD = self:getDet(); if(nD ~= 0) then
    return self:Adj():Trans():Mul(1/nD)
  else return logStatus("matrix.Inv: Not inverted",self) end
end

function metaMatrix:getInv()
  return self:getNew():Inv()
end

function metaMatrix:Div(oM, bE)
  local rA, cA = self:getSize()
  local rB, cB = oM:getSize(); if(not bE and cA ~= rB) then
    return logStatus("matrix.Div: Dimension mismatch ["..rA.." x "..cA.."] * ["..rB.." x "..cB.."]",nil) end
  if(matrix.isValid(oM)) then
    if(bE) then local tA, tM = self:getData(), oM:getData()
      for iR = 1, rA do
        for iC = 1, cA do local vM = ((tM[iR] and tM[iR][iC]) and tM[iR][iC] or 1)
          if(extlb) then vB = extlb.complexNew(vM)
          else vB = (tonumber(vM) or 1) end
          tA[iR][iC] = tA[iR][iC] / vM
        end
      end; return self:setData()
    end; return self:Mul(oM:getInv())
  else local extlb = dataMatrix.__extlb
    if(extlb) then return self:Mul(extlb.complexNew(oM):Rev())
    else return self:Mul(1/(tonumber(oM) or 0)) end
  end
end

function metaMatrix:getDiv(...)
  return self:getNew():Div(...)
end

metaMatrix.__call = function(oM, nR, nC, nS)
  local tData, nS = oM:getData(), tonumber(nS)
  local nR = getClamp(tonumber(nR) or 1 ,1)
  local nC = getClamp(tonumber(nC) or nR,1)
  local tR = tData[nR]; if(not tR) then
    return logStatus("matrix.Call: Missing row ["..nR.." x "..nC.."]",nil) end
  local nV = tR[nC]; if(not nV) then
    return logStatus("matrix.Call: Missing col ["..nR.." x "..nC.."]",nil) end
  if(nS) then tR[nC] = nS end; return nV -- Return the indexed column. Null if not found
end

function metaMatrix:Upper(bS)
  local tData, nR, nC = self:getData(), self:getSize()
  local tS, iR, eR, dR, iC, eC, dC = tData[1], 2, nR, 1
  if(bS) then iC, dC = nC, -1 else iC, dC = 1, 1 end
  while(iR <= eR) do local iD = iR
    while(iD <= eR) do local tD = tData[iD]
      if(not tD[iC]) then return self end
      if(not isZero(tD[iC])) then
        local nK = (tD[iC] / tS[iC])
        self:Modify(iD, iR-dR, -nK)
      end; iD = (iD + dR)
    end; tS, iC, iR = tData[iR], (iC + dC), (iR + dR)
  end; return self
end

function metaMatrix:getUpper(...)
  return self:getNew():Upper(...)
end

function metaMatrix:Lower(bS)
  local tData, nR, nC = self:getData(), self:getSize()
  local tS, iR, eR, dR, iC, dC = tData[nR], (nR-1), 1, -1
  if(bS) then iC, dC = 1, 1 else iC, dC = nC, -1 end
  while(iR >= eR) do local iD = iR
    while(iD >= eR) do local tD = tData[iD]
      if(not tD[iC]) then return self end
      if(not isZero(tD[iC])) then
        local nK = (tD[iC] / tS[iC])
        self:Modify(iD, iR-dR, -nK)
      end; iD = (iD + dR)
    end; tS, iC, iR = tData[iR], (iC + dC), (iR + dR)
  end; return self
end

function metaMatrix:getLower(...)
  return self:getNew():Lower(...)
end

function metaMatrix:Drop()
  local tData = self:getData()
  local iR, nR, nC = 1, self:getSize()
  while(iR <= nR) do local nZ = 0
    for iC = 1, nC do local vD = tData[iR][iC]
      if(isZero(vD)) then nZ = (nZ + 1) end end 
    if(nZ == nC) then nR = (nR - 1)
      table.remove(tData, iR)
    else iR = (iR + 1) end
  end; return self:setData()
end

function metaMatrix:getDrop()
  return self:getNew():Drop()
end

function metaMatrix:getRank()
  return self:getNew():Upper():Drop():getRows()
end

function metaMatrix:Solve(oB)
  local nR, nC = self:getSize(); if(nR ~= nC) then
    return logStatus("matrix.Solve: Rectangle ["..nR.." x "..nC.."]", nil) end
  return self:Inv():Mul(oB)
end

function metaMatrix:getSolve(...)
  return self:getNew():Solve(...)
end

function metaMatrix:getTrace()
  local mR, mC = self:getSize()
  local nE, nT, tData = math.min(mR, mC), 0, self:getData()
  for iE = 1, nE do nT = nT + tData[iE][iE] end; return nT
end

function metaMatrix:PowExp(oR, oI)
  local extlb, nK = dataMatrix.__extlb
  if(extlb) then nK = extlb.complexNew(oR, oI)
  else nK = (tonumber(oR) or 0) end
  return self:Log():Mul(nK):Exp()
end

function metaMatrix:getPowExp(...)
  return self:getNew():PowExp(...)
end

-- det(tI - A) = 0
function metaMatrix:getRoots()
  return {}
end

function metaMatrix:getEig()
  local nR, nC = self:getSize(); if(nR ~= nC) then
    return logStatus("matrix.getEig: Rectangle ["..nR.." x "..nC.."]", nil) end
  return self:getNew(), self:getNew()
end

function metaMatrix:Snip(nsR, neR, nsC, neC)
  local tD, nR, nC = self:getData(), self:getSize()
  local nsR, neR = (tonumber(nsR) or 0), (tonumber(neR) or 0)
  local nsC, neC = (tonumber(nsC) or 0), (tonumber(neC) or 0) 
  if(nsR > neR) then return logStatus("matrix.Snip: Row mismatch {"..nsR..", "..neR.."}", nil) end
  if(nsC > neC) then return logStatus("matrix.Snip: Col mismatch {"..nsC..", "..neC.."}", nil) end
  for iD = nsR, neR do local tR = tD[iD]; if(not tR) then
    return logStatus("matrix.Snip: Row size #"..nR, nil) end
    for iK = nsC, neC do local vN = tR[iK]; if(not vN) then
      return logStatus("matrix.Snip: Col size #"..nC, nil) end end
  end
  for iD = (neR+1), nR do table.remove(tD) end
  for iD = 1, (nsR-1) do table.remove(tD, 1) end
  for iD = 1, (neR-nsR+1) do local tR = tD[iD] 
    for iK = (neC+1), nR do table.remove(tR) end
    for iK = 1, (nsC-1) do table.remove(tR, 1) end    
  end; return self:setData()
end

function metaMatrix:getSnip(...)
  return self:getNew():Snip(...)
end

metaMatrix.__mul = function(oA,oB)
  return oA:getNew():Mul(oB)
end

metaMatrix.__div = function(oA,oB)
  return oA:getNew():Div(oB)
end

metaMatrix.__add = function(oA,oB)
  return oA:getNew():Add(oB)
end

metaMatrix.__sub = function(oA,oB)
  return oA:getNew():Sub(oB)
end

metaMatrix.__unm = function(oA,oB)
  return oA:getNew():Neg(oB)
end

metaMatrix.__tostring = function(oM) 
  local nR, nC = oM:getSize()
  local nZ, tD = oM:getRank(), tostring(oM:getData()):sub(8,-1)
  return "{"..tD.."}Matrix("..nZ..") ["..nR.." x "..nC.."]"
end

metaMatrix.__concat = function(oA,oB)
  return tostring(oA)..tostring(oB)
end

metaMatrix.__len = function(oM)
  return oM:getRank()
end

metaMatrix.__eq = function(oA,oB)
  local aR, aC = oA:getSize()
  local bR, bC = oB:getSize()
  if(aR ~= aC) then return false end
  if(bR ~= bC) then return false end
  local tA, tB = oA:getData(), oB:getData()
  if(not (tA and tB)) then return false end
  for iR = 1, aR do for iC = 1, aC do
    if(tA[iR][iC] ~= tB[iR][iC]) then return false end
  end end; return true -- Every element must be exact same
end

metaMatrix.__lt = function(oA,oB)
  local oA, oB, bF = oA, oB, false
  if(not matrix.isValid(oA)) then oA, oB, bF = oB, oA, true end
  if(not matrix.isValid(oB)) then local nR, nC = oA:getSize()
    if(nR == 1 and nC == 1) then local nV = oA:getData()[1][1]
      return getPick(bF, nV > oB, nV < oB)
    else local tA, tB = getType(oA), getType(oB); if(bF) then tA, tB = tB, tA end
      return logStatus("matrix.(<=): Cannot compare {"..tA.."} with {"..tB.."}",false)
    end; return false
  end; return oA:getDet() < oB:getDet()
end

metaMatrix.__le = function(oA,oB)
  local oA, oB, bF = oA, oB, false
  if(not matrix.isValid(oA)) then oA, oB, bF = oB, oA, true end
  if(not matrix.isValid(oB)) then local nR, nC = oA:getSize()
    if(nR == 1 and nC == 1) then local nV = oA:getData()[1][1]
      return getPick(bF, nV >= oB, nV <= oB)
    else local tA, tB = getType(oA), getType(oB); if(bF) then tA, tB = tB, tA end
      return logStatus("matrix.(<=): Cannot compare {"..tA.."} with {"..tB.."}",false)
    end; return false
  end; return oA:getDet() <= oB:getDet()
end

function matrix.convNew(vIn,...)
  if(matrix.isValid(vIn)) then return vIn:getNew(vIn) end
  local tyIn, tArg = type(vIn), {...}
  if(isType(tyIn, 5)) then -- Table

  end
end

return matrix
