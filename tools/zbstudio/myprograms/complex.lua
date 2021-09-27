-- Copyright (C) 2017 Deyan Dobromirov
-- A complex functionalities library

if not debug.getinfo(3) then
  print("This is a module to load with `local complex = require('complex')`.")
  os.exit(1)
end

local common          = require("common")
local type            = type
local math            = math
local pcall           = pcall
local tonumber        = tonumber
local tostring        = tostring
local getmetatable    = getmetatable
local setmetatable    = setmetatable
local table           = table
local complex         = {}
local metaComplex     = {}
local metaData        = {}

local isNil           = common.isNil
local isBool          = common.isBool
local isType          = common.isType
local isTable         = common.isTable
local isNumber        = common.isNumber
local isString        = common.isString
local isFunction      = common.isFunction

local getPick         = common.getPick
local getSign         = common.getSign
local getRound        = common.getRound
local getClamp        = common.getClamp
local logStatus       = common.logStatus
local logString       = common.logString
local getSignNon      = common.getSignNon
local getValueKeys    = common.getValueKeys
local randomGetNumber = common.randomGetNumber
metaComplex.__type    = "complex.complex"
metaComplex.__index   = metaComplex

metaData.__valre = 0
metaData.__valim = 0
metaData.__cactf = {}
metaData.__ipmtx = {}
metaData.__valns = "X"
metaData.__curve = 100
metaData.__fulan = 360
metaData.__margn = 1e-10
metaData.__nanum = (0/0)
metaData.__basef = "%s,%s"
metaData.__getpi = math.pi
metaData.__expvl = math.exp(1)
metaData.__infum = math.huge
metaData.__fulpi = (2 * metaData.__getpi)
metaData.__bords = {"{([<|/","})]>|/"}
metaData.__ssyms = {"i", "I", "j", "J", "k", "K"}
metaData.__radeg = (180 / metaData.__getpi)
metaData.__kreal = {1,"Real","real","Re","re","R","r","X","x"}
metaData.__kimag = {2,"Imag","imag","Im","im","I","i","Y","y"}

function complex.extend()
  metaData.__extlb = require("extensions").complex; return complex
end

function complex.isValid(cNum)
  return (getmetatable(cNum) == metaComplex)
end

function complex.getType(cNum)
  if(not cNum) then return metaComplex.__type end
  local tM = getmetatable(cNum)
  return ((tM and tM.__type) and tostring(tM.__type) or type(cNum))
end

function complex.setMargin(nM)
  metaData.__margn = math.abs(tonumber(nM) or 0)
end

function complex.getMargin()
  return metaData.__margn
end

local function getUnpackStack(R, I, E)
  if(complex.isValid(R)) then
    local nR, nI = R:getParts(); return nR, nI, I
  elseif(isTable(R)) then
    local nR = (getValueKeys(R, metaData.__kreal) or 0)
    local nI = (getValueKeys(R, metaData.__kimag) or 0); return nR, nI, I
  end; return (tonumber(R) or metaData.__valre), (tonumber(I) or metaData.__valim), E
end

function complex.getNew(nRe, nIm)
  local Re, Im, self = 0, 0, {}
  setmetatable(self, metaComplex)

  if(complex.isValid(nRe)) then -- Copy-constructor
    Re, Im = nRe:getParts()
  elseif(isBool(nRe)) then -- Boolean to false=0/true=1
    Re, Im = (nRe and 1 or 0), (nIm and 1 or 0)
  elseif(isFunction(nRe)) then -- Function with table arguments nIm
    local bS, nR, nI = pcall(nRe, nIm)
    if(not bS) then return logStatus("complex.getNew: "..nR, nil) end
    if(complex.isValid(nR)) then Re, Im = nR:getParts() else
      Re = tonumber(nR) or metaData.__valre
      Im = tonumber(nI) or metaData.__valim
    end -- The function return value is not complex object
  else -- Number or string conveted to number
    Re = tonumber(nRe) or metaData.__valre
    Im = tonumber(nIm) or metaData.__valim
  end

  function self:setReal(R)  Re = (tonumber(R) or metaData.__valre); return self end
  function self:setImag(I)  Im = (tonumber(I) or metaData.__valim); return self end
  function self:getReal()   return Re end
  function self:getImag()   return Im end
  function self:getParts()  return Re, Im end

  function self:Set(R, I)
    local R, I = getUnpackStack(R, I)
    Re, Im = R, I; return self
  end

  function self:Add(R, I)
    local R, I = getUnpackStack(R, I)
    Re, Im = (Re + R), (Im + I); return self
  end

  function self:Sub(R, I)
    local R, I = getUnpackStack(R, I)
    Re, Im = (Re - R), (Im - I); return self
  end

  function self:Rsz(vN) local nN = tonumber(vN)
    if(nN) then Re, Im = (Re * nN), (Im * nN) end; return self
  end

  function self:Mul(R, I, E)
    local C, D, U = getUnpackStack(R, I, E)
    if(U) then Re, Im = (Re*C), (Im*D) else
      Re, Im = (Re*C - Im*D), (Re*D + Im*C)
    end; return self
  end

  function self:Mid(R, I)
    local C, D = getUnpackStack(R, I)
    Re, Im = ((Re + C) / 2), ((Im + D) / 2); return self
  end

  function self:Div(R, I, E)
    local C, D, U = getUnpackStack(R, I, E)
    if(U) then Re, Im = (Re/C), (Im/D) else local Z = (C*C + D*D)
      Re, Im = ((Re*C + Im*D) / Z), ((Im*C - Re*D) / Z)
    end; return self
  end

  function self:Frac(R, I)
    local C, D = getUnpackStack(R, I)
    local rI, rF = math.modf(R and C or Re)
    local iI, iF = math.modf(R and D or Im)
    Re, Im = rF, iF; return self
  end

  function self:Trunc(R, I, E)
    local C, D, U = getUnpackStack(R, I, E)
    local P = (U and math.ceil  or math.floor)
    local N = (U and math.floor or math.ceil )
    C, D = (R and C or Re), (R and D or Im)
    Re = (C > 0 and P(C) or N(C))
    Im = (D > 0 and P(D) or N(D))
    return self
  end

  function self:Mod(R, I, E)
    local C, D, U = getUnpackStack(R, I, E)
    if(U) then
      Re, Im = (Re % C), (Im % D)
    else local Z = self:getDiv(C, D)
      return self:Sub(Z:Floor():Mul(C, D))
    end
    return self
  end

  function self:Rev()
    local N = self:getNorm2()
    Re, Im = (Re/N), (-Im/N); return self
  end

  function self:Pow(R, I, E)
    local C, D, U = getUnpackStack(R, I, E)
    if(U) then Re, Im = (Re^C), (Im^D) else
      local N, G = self:getNorm2(), self:getAngRad()
      local eK = N^(C/2) * math.exp(-D*G)
      local eC = (C*G + 0.5*D*math.log(N))
      Re, Im = (eK * math.cos(eC)), (eK * math.sin(eC))
    end; return self
  end

  function self:Exp(R, I, E)
    local X = metaData.__expvl
    local C, D, U = getUnpackStack(R, I, E)
    if(not R) then C, D = self:getParts() end
    return self:setReal(X):setImag(0):Pow(C, D, U)
  end

  return self
end

function metaComplex:Action(aK,...)
  if(not aK) then return false, self end
  local fDr = metaData.__cactf[aK]; if(not common.isFunction(fDr)) then
    return logStatus("complex.Action: No function", false), self end
  return pcall(fDr,self,...)
end

function metaComplex:getNew(nR, nI)
  local N = complex.getNew(self); if(nR or nI) then
    local R, I = getUnpackStack(nR, nI); N:Set(R, I)
  end; return N
end

function metaComplex:Random(nL, nU, vC)
  local R = randomGetNumber(nL, nU, vC)
  local I = randomGetNumber(nL, nU, vC)
  return self:setReal(R):setImag(I)
end

function metaComplex:getRandom(...)
  return self:getNew():Random(...)
end

function metaComplex:Apply(fF, bR, bI)
  local R, I = self:getParts()
  local br, sR, vR = getPick(isNil(bR), true, bR)
  local bi, sI, vI = getPick(isNil(bI), true, bI)
  if(br) then sR, vR = pcall(fF, R); if(not sR) then
    return logStatus("complex.Apply(R): Failed: "..vR, self) end end
  if(bi) then sI, vI = pcall(fF, I); if(not sI) then
    return logStatus("complex.Apply(I): Failed: "..vI, self) end end
  R, I = ((br) and vR or R), ((bi) and vI or I)
  return self:setReal(R):setImag(I)
end

function metaComplex:getType () return metaComplex.__type end
function metaComplex:NegRe   () return self:setReal(-self:getReal()) end
function metaComplex:NegIm   () return self:setImag(-self:getImag()) end
function metaComplex:Conj    () return self:NegIm() end
function metaComplex:Neg     () return self:NegRe():NegIm() end
function metaComplex:getNegRe() return self:getNew():NegRe() end
function metaComplex:getNegIm() return self:getNew():NegIm() end
function metaComplex:getConj () return self:getNew():Conj() end
function metaComplex:getNeg  () return self:getNew():Neg() end

function metaComplex:getNorm2()
  local R, I = self:getParts(); return (R*R + I*I) end

function metaComplex:getNorm() return math.sqrt(self:getNorm2()) end

function metaComplex:setNorm(nN)
  return self:Rsz((tonumber(nN) or 0) / self:getNorm())
end

function metaComplex:Unit(R, I)
  if(R or I) then self:Set(R, I) end
  return self:Rsz(1/self:getNorm())
end

function metaComplex:getUnit(...)
  return self:getNew():Unit(...)
end

function metaComplex:getDot(R, I)
  local sR, sI = self:getParts()
  local vR, vI = getUnpackStack(R, I)
  return (sR*vR + sI*vI)
end

function metaComplex:getAngRadVec(cV)
  return (cV:getAngRad() - self:getAngRad())
end

function metaComplex:getMid(...)
  return self:getNew():Mid(...)
end

function metaComplex:Mean(...) local tV = {...}
  local fV, cV = tV[1], self:getNew(0,0)
  if(isTable(fV)) then tV = tV[1] end
  local nV = #tV; if(nV <= 0) then return self end
  for iD = 1, nV do cV:Add(tV[iD]) end
  return self:Set(cV:Rsz(1/nV))
end

function metaComplex:getMean(...)
  return self:getNew():Mean(...)
end

function metaComplex:getDist2(R, I)
  local C, D = self:getParts()
  local R, I = getUnpackStack(R, I)
  return ((C - R)^2 + (D - I)^2)
end

function metaComplex:getDist(R, I)
  return math.sqrt(self:getDist2(R, I))
end

function metaComplex:getCross(R, I)
  local C, D = self:getParts()
  local R, I = getUnpackStack(R, I)
  return (C*I - D*R)
end

function metaComplex:Sign(bE, bC, bN)
  if(bE) then local R, I = self:getParts()
    return ((R ~= 0) and getSign(R) or getSign(I)) end
  if(bC) then return self:Apply(getSign) end
  if(bN) then return self:Apply(getSignNon) end
  return self:Unit()
end

function metaComplex:getSign(...)
  return self:getNew():Sign(...)
end

function metaComplex:Swap()
  local R, I = self:getParts()
  return self:setReal(I):setImag(R)
end

function metaComplex:getSwap()
  return self:getNew():Swap()
end

function metaComplex:Right()
  return self:Swap():NegIm()
end

function metaComplex:getRight()
  return self:getNew():Right()
end

function metaComplex:Left()
  return self:Swap():NegRe()
end

function metaComplex:getLeft()
  return self:getNew():Left()
end

function metaComplex:getSet(...)
  return self:getNew():Set(...)
end

function metaComplex:getAdd(...)
  return self:getNew():Add(...)
end

function metaComplex:getSub(...)
  return self:getNew():Sub(...)
end

function metaComplex:getRsz(...)
  return self:getNew():Rsz(...)
end

function metaComplex:getMul(...)
  return self:getNew():Mul(...)
end

function metaComplex:getDiv(...)
  return self:getNew():Div(...)
end

function metaComplex:getFrac(...)
  return self:getNew():Frac(...)
end

function metaComplex:getTrunc(...)
  return self:getNew():Trunc(...)
end

function metaComplex:getMod(...)
  return self:getNew():Mod(...)
end

function metaComplex:getRev(...)
  return self:getNew():Rev(...)
end

function metaComplex:getPow(...)
  return self:getNew():Pow(...)
end

function metaComplex:getExp(...)
  return self:getNew():Exp(...)
end

function metaComplex:AddPyth(...)
  local cP = self:getNew(...)
  return self:Pow(2):Add(cP:Pow(2)):Pow(0.5)
end

function metaComplex:getAddPyth(...)
  return self:getNew():AddPyth(...)
end

function metaComplex:Margin(nM)
  local nR, nI = self:getParts()
  local nM = math.abs(tonumber(nM) or metaData.__margn)
  if(math.abs(nR) < nM) then nR = 0 end
  if(math.abs(nI) < nM) then nI = 0 end
  return self:Set(nR, nI)
end

function metaComplex:getMargin(...)
  return self:getNew():Margin(...)
end

function metaComplex:Deviation(sMsg, ...)
  local tV, nV, cT, nM = {...}, 0, self:getNew(), metaData.__margn
  local bC = complex.isValid(tV[1]); if(not bC) then tV = tV[1] end; nV = #tV
  for iD = 1, nV do local nD = cT:Set(self):Sub(tV[iD]):getNorm(); if(nD > nM) then
    logStatus("complex."..tostring(sMsg)..":"..tV[iD].."["..iD.."]: Displaced by "..nD) end
  end; return self
end

function metaComplex:Nudge(vM, vR, vI)
  local nM = (tonumber(vM) or metaData.__margn)
  return self:Add(self:getUnit(vR, vI):Mul(nM))
end

function metaComplex:getNudge(...)
  return self:getNew():Nudge(...)
end

function metaComplex:Bisect(...)
  local cD = self:getNew(...)
  local nS, nD = self:getNorm(), cD:getNorm()
  return self:Mul(nD):Add(cD:Mul(nS))
end

function metaComplex:getBisect(...)
  return self:getNew():Bisect(...)
end

function metaComplex:Sin()
  local R, I = self:getParts()
  local rE = math.sin(R)*math.cosh(I)
  local iM = math.cos(R)*math.sinh(I)
  return self:setReal(rE):setImag(iM)
end

function metaComplex:getSin()
  return self:getNew():Sin()
end

function metaComplex:ArcSin()
  local Z = self:getPow(2):Neg():Add(1):Pow(0.5)
  return self:Mul(0, 1):Add(Z):Log():Mul(0, -1)
end

function metaComplex:getArcSin()
  return self:getNew():ArcSin()
end

function metaComplex:Cos()
  local R, I = self:getParts()
  local rE =  math.cos(R)*math.cosh(I)
  local iM = -math.sin(R)*math.sinh(I)
  return self:setReal(rE):setImag(iM)
end

function metaComplex:getCos()
  return self:getNew():Cos()
end

function metaComplex:ArcCos()
  local Z = self:getPow(2):Neg():Add(1):Pow(0.5)
  return self:Add(Z:Mul(0, 1)):Log():Mul(0, -1)
end

function metaComplex:getArcCos()
  return self:getNew():ArcCos()
end

function metaComplex:Tang()
  local cC = self:getCos()
  return self:Sin():Div(cC)
end

function metaComplex:getTang()
  return self:getNew():Tang()
end

function metaComplex:ArcTang()
  local cD = self:getAdd(0, 1)
  local cN = self:getNeg():Add(0, 1)
  return self:Set(cN:Div(cD):Log():Mul(0, -0.5))
end

function metaComplex:getArcTang()
  return self:getNew():ArcTang()
end

function metaComplex:Cotg()
  return self:Tang():Rev()
end

function metaComplex:getCotg()
  return self:getNew():Cotg()
end

function metaComplex:ArcCotg()
  return self:Rev():ArcTang()
end

function metaComplex:getArcCotg()
  return self:getNew():ArcCotg()
end

function metaComplex:SinH()
  local cE = self:getExp()
  return self:Set(cE):Sub(cE:Rev()):Rsz(0.5)
end

function metaComplex:getSinH()
  return self:getNew():SinH()
end

function metaComplex:ArcSinH()
  local cZ = self:getPow(2):Add(1):Pow(0.5)
  return self:Add(cZ):Log()
end

function metaComplex:getArcSinH()
  return self:getNew():ArcSinH()
end

function metaComplex:CosH()
  local cE = self:getExp()
  return self:Set(cE):Add(cE:Rev()):Rsz(0.5)
end

function metaComplex:getCosH()
  return self:getNew():CosH()
end

function metaComplex:ArcCosH()
  local cP = self:getAdd(1):Pow(0.5)
  local cN = self:getSub(1):Pow(0.5)
  return self:Add(cP:Mul(cN)):Log()
end

function metaComplex:getArcCosH()
  return self:getNew():ArcCosH()
end

function metaComplex:TangH()
  local cC = self:getCosH()
  return self:SinH():Div(cC)
end

function metaComplex:getTangH()
  return self:getNew():TangH()
end

function metaComplex:ArcTangH()
  local cN = self:getNeg():Add(1):Log()
  return self:Add(1):Log():Sub(cN):Mul(0.5)
end

function metaComplex:getArcTangH()
  return self:getNew():ArcTangH()
end

function metaComplex:CotgH()
  return self:TangH():Rev()
end

function metaComplex:getCotgH()
  return self:getNew():CotgH()
end

function metaComplex:ArcCotgH() self:Rev()
  local cN = self:getNeg():Add(1):Log()
  return self:Add(1):Log():Sub(cN):Mul(0.5)
end

function metaComplex:getArcCotgH()
  return self:getNew():ArcCotgH()
end

function metaComplex:Log(nK)
  local P, R, T = metaData.__getpi, self:getPolar()
  return self:setReal(math.log(R)):setImag(T+2*(tonumber(nK) or 0)*P)
end

function metaComplex:getLog(...)
  return self:getNew():Log(...)
end

function metaComplex:getApply(...)
  return self:getNew():Apply(...)
end

function metaComplex:Abs(bR, bI)
  return self:Apply(math.abs, bR, bI)
end

function metaComplex:getAbs(...)
  return self:getNew():Abs(...)
end

function metaComplex:Floor(bR, bI)
  return self:Apply(math.floor, bR, bI)
end

function metaComplex:getFloor(...)
  return self:getNew():Floor(...)
end

function metaComplex:Ceil(bR, bI)
  return self:Apply(math.ceil, bR, bI)
end

function metaComplex:getCeil(...)
  return self:getNew():Ceil(...)
end

function metaComplex:getAngRad()
  local R, I = self:getParts(); return math.atan2(I, R) end

function metaComplex:getTable(kR, kI)
  local kR, kI = (kR or metaData.__kreal[1]), (kI or metaData.__kimag[1])
  local R , I  = self:getParts(); return {[kR] = R, [kI] = I}
end

function metaComplex:Println(sF,sS,sE)
  local nR, nI = self:getParts()
  local fB, sF = metaData.__basef, tostring(sF or "%f")
  local sS, sE = tostring(sS or "{"), tostring(sE or "}")
  return logStatus(sS..(fB:format(sF,sF):format(nR,nI))..sE, self)
end

function metaComplex:Print(sF,sS,sE)
  local nR, nI = self:getParts()
  local fB, sF = metaData.__basef, tostring(sF or "%f")
  local sS, sE = tostring(sS or "{"), tostring(sE or "}")
  return logString(sS..(fB:format(sF,sF):format(nR,nI))..sE, self)
end

function metaComplex:Euler(vR, vA)
  if(vR or vA) then self:Set(vR, vA) end
  local nR, vA = self:getParts()
  return self:Set(math.cos(vA), math.sin(vA)):Rsz(nR)
end

function metaComplex:getEuler(...)
  return self:getNew():Euler(...)
end

function metaComplex:Round(nF)
  local R, I = self:getParts()
  return self:setReal(getRound(R, nF)):setImag(getRound(I, nF))
end

function metaComplex:getRound(...)
  return self:getNew():Round(...)
end

function metaComplex:getPolar()
  return self:getNorm(), self:getAngRad()
end

function metaComplex:setAngRad(nA)
  local nR, nP = self:getNorm(), (tonumber(nA) or 0)
  return self:setReal(math.cos(nP)):setImag(math.sin(nP)):Rsz(nR)
end

function metaComplex:RotRad(nA)
  return self:setAngRad(self:getAngRad() + (tonumber(nA) or 0))
end

function metaComplex:getRotRad(...)
  return self:getNew():RotRad(...)
end

function metaComplex:setPolarRad(nN, nA)
  return self:Set((tonumber(nN) or 0), 0):setAngRad(nA)
end

function metaComplex:ProjectRay(cO, cD)
  local cV = self:getSub(cO)
  local nK = cV:getCross(cD) / cD:getNorm2()
  return self:Add(cD:getMul(nK, -nK, true):Swap())
end

function metaComplex:getProjectRay(...)
  return self:getNew():ProjectRay(...)
end

function metaComplex:ProjectLine(cS, cE)
  return self:ProjectRay(cS, cE:getSub(cS))
end

function metaComplex:getProjectLine(...)
  return self:getNew():ProjectLine(...)
end

function metaComplex:ProjectCircle(cC, nR)
  return self:Sub(cC):Unit():Mul(nR):Add(cC)
end

function metaComplex:getProjectCircle(...)
  return self:getNew():ProjectCircle(...)
end

function metaComplex:getLayRay(cO, cD)
  return self:getSub(cO):getCross(cD)
end

function metaComplex:getLayLine(cS, cE)
  return self:getLayRay(cS, cE:getSub(cS))
end

function metaComplex:MirrorPoint(...)
  return self:Add(self:getSet(...):Sub(self):Rsz(2))
end

function metaComplex:getMirrorPoint(...)
  return self:getNew():MirrorPoint(...)
end

function metaComplex:MirrorRay(cO, cD)
  local cP = self:getProjectRay(cO, cD)
  return self:Add(cP:Sub(self):Rsz(2))
end

function metaComplex:getMirrorRay(...)
  return self:getNew():MirrorRay(...)
end

function metaComplex:MirrorLine(cS, cE)
  return self:MirrorRay(cS, cE:getSub(cS))
end

function metaComplex:getMirrorLine(...)
  return self:getNew():MirrorLine(...)
end

function metaComplex:getAreaParallelogram(...)
  return math.abs(self:getCross(...))
end

function metaComplex:getAreaTriangle(...)
  return math.abs(self:getAreaParallelogram(...) / 2)
end

function complex.getAreaShoelace(...)
  local tV, nP, nN = {...}, 0, 0
  if(isTable(tV[1])) then tV = tV[1] end
  local nE = #tV; tV[nE+1] = tV[1]
  for ID = 1, nE do
    local cB, cN = tV[ID], tV[ID+1]
    nP = nP + (cB:getReal()*cN:getImag())
    nN = nN + (cB:getImag()*cN:getReal())
  end; return math.abs(0.5 * (nP - nN))
end

function complex.getAreaHeron(...)
  local tV = {...}; if(isTable(tV[1])) then tV = tV[1] end
  local nV = #tV; if(nV < 3) then local sV = tostring(nV or "")
    return logStatus("complex.getAreaHeron: Vertexes lacking <"..sV..">", 0) end
  if(nV > 3) then local sV = tostring(nV or "")
    logStatus("complex.getAreaHeron: Vertexes extra <"..sV..">") end
  local nA = tV[1]:getSub(tV[2]):getNorm2()
  local nB = tV[2]:getSub(tV[3]):getNorm2()
  local nC = tV[3]:getSub(tV[1]):getNorm2()
  local nD = (4 * (nA*nB + nA*nC + nB*nC))
  local nE = ((nA + nB + nC)^2)
  return math.abs(0.25 * math.sqrt(nD - nE))
end

function metaComplex:isAmongLine(cS, cE, bF)
  local nM = metaData.__margn
  if(math.abs(self:getLayLine(cS, cE)) < nM) then
    local dV = cE:getSub(cS)
    local dS = self:getSub(cS):getDot(dV)
    local dE = self:getSub(cE):getDot(dV)
    if(not bF and dS * dE > 0) then return false end
    return true
  end; return false
end

function metaComplex:isAmongPoint(vR, vI)
  return (self:getSub(vR, vI):getNorm() < metaData.__margn)
end

function metaComplex:Zero()
  return self:setReal(0):setImag(0)
end

function metaComplex:getZero()
  return self:getNew():Zero()
end

function metaComplex:isZeroReal()
  return (math.abs(self:getReal()) < metaData.__margn)
end

function metaComplex:isZeroImag()
  return (math.abs(self:getImag()) < metaData.__margn)
end

function metaComplex:isZero(bR, bI)
  local bR = getPick(isNil(bR), true, bR)
  local bI = getPick(isNil(bI), true, bI)
  local zR, zI = self:isZeroReal(), self:isZeroImag()
  if(bR and bI) then return (zR and zI) end
  if(bR) then return zR end; if(bI) then return zI end
  return logStatus("complex.isZero: Not applicable", nil)
end

function metaComplex:isInfReal(bR)
  local mH, nR = metaData.__infum, self:getReal()
  if(bR) then return (nR == -mH) end
  return (nR == mH)
end

function metaComplex:isInfImag(bI)
  local mH, nI = metaData.__infum, self:getImag()
  if(bI) then return (nI == -mH) end
  return (nI == mH)
end

function metaComplex:isInf(bR, bI)
  return (self:isInfReal(bR) and self:isInfImag(bI))
end

function metaComplex:Inf(bR, bI)
  local nH, sR, sI = metaData.__infum, self:getParts()
  local nR = getPick(isNil(bR), sR, getPick(bR, -nH, nH))
  local nI = getPick(isNil(bI), sI, getPick(bI, -nH, nH))
  return self:setReal(nR):setImag(nI)
end

function metaComplex:getInf(...)
  return self:getNew():Inf(...)
end

function metaComplex:isNanReal()
  local nR = self:getReal(); return (nR ~= nR)
end

function metaComplex:isNanImag()
  local nI = self:getImag(); return (nI ~= nI)
end

function metaComplex:isNan()
  return (self:isNanReal() and self:isNanImag())
end

function metaComplex:Nan(bR, bI)
  local sR, sI = self:getParts()
  local nN = metaData.__nanum
  local nR = getPick(bR, nN, sR)
  local nI = getPick(bI, nN, sI)
  return self:setReal(nR):setImag(nI)
end

function metaComplex:getNan(...)
  return self:getNew():Nan(...)
end

function metaComplex:isAmongRay(cO, cD, bF)
  local nM = metaData.__margn
  if(math.abs(self:getLayRay(cO, cD)) < nM) then
    local dO = self:getSub(cO):getDot(cD)
    local dE = cO:getAdd(cD):Sub(self):Neg():getDot(cD)
    if(dO < 0 and dE < 0) then return false end
    if(not bF and dO > 0 and dE > 0) then return false end
    return true
  end; return false
end

function metaComplex:isOrthogonal(vC)
  return (math.abs(self:getDot(vC)) < metaData.__margn)
end

function metaComplex:isCollinear(vC)
  return (math.abs(self:getCross(vC)) < metaData.__margn)
end

function metaComplex:isInCircle(cO, vR)
  local nM = metaData.__margn
  local nR = getClamp(tonumber(vR) or 0, 0)
  local nN = self:getSub(cO):getNorm()
  return (nN < (nR+nM))
end

function metaComplex:isAmongCircle(cO, vR)
  local nM = metaData.__margn
  local nN = self:getSub(cO):getNorm()
  local nR = getClamp(tonumber(vR) or 0, 0)
  return ((nN < (nR+nM)) and (nN > (nR-nM)))
end

function metaComplex:getRoots(nNm)
  local nN = math.floor(tonumber(nNm) or 0)
  if(nN > 0) then local tRt = {}
    local nPw, nA  = (1 / nN), (metaData.__fulpi / nN)
    local nRa = self:getNorm()   ^ nPw
    local nAn = self:getAngRad() * nPw
    for k = 1, nN do
      local cRe, cIm = (nRa * math.cos(nAn)), (nRa * math.sin(nAn))
      tRt[k], nAn = self:getNew(cRe,cIm), (nAn + nA)
    end; return tRt
  end; return logStatus("complex.getRoots: Invalid <"..nN..">")
end

function metaComplex:getFormat(...)
  local tArg = {...}
  local sMod = tostring(tArg[1] or ""):lower()
  if(isType(sMod, 5)) then
    local tvB = metaData.__bords
    local tkR, tkI = metaData.__kreal, metaData.__kimag
    local sN, R, I = tostring(tArg[3] or "%f"), self:getParts()
    local iS = math.floor((tvB[1]..tvB[2]):len()/2)
          iB = getClamp(tonumber(tArg[4] or 1), 1, iS)
    local eS = math.floor((#tkR + #tkI)/2)
          iD = getClamp((tonumber(tArg[2]) or 1), 1, eS)
    local sF, sB = tvB[1]:sub(iB,iB), tvB[2]:sub(iB,iB)
    local kR, kI = (tArg[5] or tkR[iD]), (tArg[6] or tkI[iD])
    if(not (kR and kI)) then return tostring(self) end
    local qR, qI = isString(kR), isString(kI)
          kR = qR and ("\""..kR.."\"") or tostring(kR)
          kI = qI and ("\""..kI.."\"") or tostring(kI)
    return (sF.."["..kR.."]="..sN:format(R)..
               ",["..kI.."]="..sN:format(I)..sB)
  elseif(isType(sMod, 3)) then
    local S, R, I = metaData.__ssyms, self:getParts()
    local mI, bS = (getSign(I) * I), tArg[3]
    local iD = getClamp(tonumber(tArg[2]) or 1, 1, #S)
    local kI = tostring(tArg[4] or S[iD])
    local sI = ((getSign(I) < 0) and "-" or "+")
    if(bS) then return (R..sI..kI..mI)
    else return (R..sI..mI..kI) end
  end; return tostring(self)
end

metaComplex.__len = function(cNum) return cNum:getNorm() end

metaComplex.__call = function(cNum, sMth, ...)
  return pcall(cNum[tostring(sMth)], cNum, ...)
end

metaComplex.__tostring = function(cNum)
  local R = tostring(cNum:getReal() or metaData.__valns)
  local I = tostring(cNum:getImag() or metaData.__valns)
  return "{"..R..","..I.."}"
end

metaComplex.__unm = function(cNum)
  return complex.getNew(cNum):Neg()
end

metaComplex.__add = function(C1,C2)
  return complex.getNew(C1):Add(C2)
end

metaComplex.__sub = function(C1,C2)
  return complex.getNew(C1):Sub(C2)
end

metaComplex.__mul = function(C1,C2)
  return complex.getNew(C1):Mul(C2)
end

metaComplex.__div = function(C1,C2)
  return complex.getNew(C1):Div(C2)
end

metaComplex.__mod = function(C1,C2)
  return complex.getNew(C1):Mod(C2)
end

metaComplex.__pow = function(C1,C2)
  return complex.getNew(C1):Pow(C2)
end

metaComplex.__concat = function(A,B)
  return tostring(A)..tostring(B)
end

metaComplex.__eq = function(C1,C2)
  local R1, I1 = getUnpackStack(C1)
  local R2, I2 = getUnpackStack(C2)
  if(R1 == R2 and I1 == I2) then return true end
  return false
end

metaComplex.__le = function(C1,C2)
  local R1, I1 = getUnpackStack(C1)
  local R2, I2 = getUnpackStack(C2)
  if(I1 == 0 and I2 == 0) then return (R1 <= R2) end
  if(R1 <= R2 and I1 <= I2) then return true end
  return false
end

metaComplex.__lt = function(C1,C2)
  local R1, I1 = getUnpackStack(C1)
  local R2, I2 = getUnpackStack(C2)
  if(I1 == 0 and I2 == 0) then return (R1 < R2) end
  if(R1 < R2 and I1 < I2) then return true end
  return false
end

function complex.getIntersectRayRay(cO1, cD1, cO2, cD2)
  local dD = cD1:getCross(cD2); if(dD == 0) then
    return logStatus("complex.getIntersectRayRay: Rays parallel", nil) end
  local dO = cO2:getNew():Sub(cO1)
  local nT, nU = (dO:getCross(cD2) / dD), (dO:getCross(cD1) / dD)
  return dO:Set(cO1):Add(cD1:getRsz(nT)), nT, nU
end

function complex.getIntersectRayLine(cO, cD, cS, cE)
  return complex.getIntersectRayRay(cO, cD, cS, cE:getSub(cS))
end

function complex.getIntersectLineRay(cS, cE, cO, cD)
  return complex.getIntersectRayRay(cS, cE:getSub(cS), cO, cD)
end

function complex.getIntersectLineLine(cS1, cE1, cS2, cE2)
  return complex.getIntersectRayRay(cS1, cE1:getSub(cS1), cS2, cE2:getSub(cS2))
end

function complex.getSnapRayRay(cO1, cD1, cO2, cD2, scOpe)
  local cS, nD = cO1:getProjectRay(cO2, cD2), cD1:getNorm2()
  local cE = cD2:getUnit():Mul(cD1:getNorm()):Add(cS)
  if(cS:getSub(cO1):getNorm2() > nD) then
    return logStatus("complex.getSnapRayRay: Radius mismatch", nil) end
  local nM, iK = metaData.__margn, 0
  local nH, cM = cE:getSub(cS):getNorm2(), cS:getMid(cE)
  while(nH > nM) do
    if(cM:getSub(cO1):getNorm2() > nD) then
      cE:Set(cM); cM:Mid(cS) else cS:Set(cM); cM:Mid(cE) end
    iK, nH = (iK + 1), cE:getSub(cS):getNorm2()
  end; return cM, iK
end

function complex.getSnapRayLine(cO, cD, cS, cE)
  return complex.getSnapRayRay(cO, cD, cS, cE:getSub(cS))
end

function complex.getSnapLineRay(cS, cE, cO, cD)
  return complex.getSnapRayRay(cS, cE:getSub(cS), cO, cD)
end

function complex.getSnapLineLine(cS1, cE1, cS2, cE2)
  return complex.getSnapRayRay(cS1, cE1:getSub(cS1), cS2, cE2:getSub(cS2))
end

function complex.getReflectRayRay(cO1, cD1, cO2, cD2)
  local cN = cO1:getProjectRay(cO2, cD2):Neg():Add(cO1):Unit()
  if(cN:getDot(cD1) > 0) then -- Ray points away from the reflection wall
    return logStatus("complex.getReflectRayRay: Angle mismatch", nil, cN) end
  local uD = cD1:getUnit() -- Get unit ray direction
  local cR = uD:Sub(cN:getNew():Mul(2 * uD:getDot(cN))):Unit()
  if(cR:getDot(cN) < 0) then -- Reflection goes trough the wall
    return logStatus("complex.getReflectRayRay: Normal mismatch", nil, cN) end
  return cR, cN
end

function complex.getReflectRayLine(cO, cD, cS, cE)
  return complex.getReflectRayRay(cO, cD, cS, cE:getSub(cS))
end

function complex.getReflectLineRay(cS, cE, cO, cD)
  return complex.getReflectRayRay(cS, cE:getSub(cS), cO, cD)
end

function complex.getReflectLineLine(cS1, cE1, cS2, cE2)
  return complex.getReflectRayRay(cS1, cE1:getSub(cS1), cS2, cE2:getSub(cS2))
end

function complex.getIntersectRayCircle(cO, cD, cC, nR)
  local nA = cD:getNorm2(); if(nA < metaData.__margn) then
    return logStatus("complex.getIntersectRayCircle: Norm less than margin", nil) end
  local cR = cO:getNew():Sub(cC)
  local nB, nC = 2 * cD:getDot(cR), (cR:getNorm2() - nR^2)
  local nD = (nB^2 - 4*nA*nC); if(nD < 0) then
    return logStatus("complex.getIntersectRayCircle: Imaginary roots", nil) end
  local dA = (1/(2*nA)); nD, nB = dA*math.sqrt(nD), -nB*dA
  local xM = cD:getNew():Mul(nB - nD):Add(cO)
  local xP = cD:getNew():Mul(nB + nD):Add(cO)
  if(cO:isInCircle(cC, nR)) then return xP, xM end
  return xM, xP -- Outside the circle
end

function complex.getIntersectLineCircle(cS, cE, cC, nR)
  return complex.getIntersectRayCircle(cS, cE:getSub(cS), cC, nR)
end

function complex.getIntersectCircleCircle(cO1, nR1, cO2, nR2)
  local cS, cA = cO2:getSub(cO1), cO2:getAdd(cO1)
  local nD, nRA, nRS = cS:getNorm2(), (nR1 + nR2), (nR1 - nR2); if(nRA^2 < nD) then
    return logStatus("complex.getIntersectCircleCircle: Intersection missing", nil) end
  local dR = (nRA^2 - nD) * (nD - nRS^2); if(dR < 0) then
    return logStatus("complex.getIntersectCircleCircle: Irrational area", nil) end
  local nK = 0.25 * math.sqrt(dR)
  local cV = cS:getSwap():Mul(2, -2, true):Rsz(nK / nD)
  local mR = (0.5 * (nR1^2 - nR2^2)) / nD
  local xB = cA:Rsz(0.5):Add(cS:Rsz(mR))
  return xB:getAdd(cV), xB:getSub(cV), xB
end

function complex.getReflectRayCircle(cO, cD, cC, nR, xU)
  local xX = (xU and xU or complex.getIntersectRayCircle(cO, cD, cC, nR))
  if(not complex.isValid(xX)) then -- Validate interesction point
    return logStatus("complex.getReflectRayCircle: Intersect mismatch", nil) end
  local cX = xX:getSub(cC):Right() -- Fifth argument may reuse intersection
  local cR, cN = complex.getReflectRayRay(cO, cD, xX, cX)
  return cR, cN, cX:Set(xX) -- Return a copy of the intersection point
end

function complex.getReflectLineCircle(cS, cE, cC, nR)
  return complex.getReflectRayCircle(cS, cE:getSub(cS), cC, nR)
end

function complex.getRefractRayRatio(vI, vO)
  return ((tonumber(vO) or 0) / (tonumber(vI) or 0))
end

function complex.getRefractRayAngle(...)
  local nR = complex.getRefractRayRatio(...)
  if(math.abs(nR) > 1) then nR = (1 / nR) end
  return math.asin(nR) -- Always less than one
end

function complex.getRefractRayRay(cO1, cD1, cO2, cD2, ...)
  local uD = cD1:getUnit() -- Reference to unit direction
  local cN = cO1:getProjectRay(cO2, cD2):Neg():Add(cO1):Unit()
  local nR, sI = complex.getRefractRayRatio(...), cN:getCross(uD:Neg())
  local sO = sI / nR; if(math.abs(sO) > 1) then
    return logStatus("complex.getRefractRayRay: Angle mismatch", nil, cN) end
  if(cN:getDot(uD) < 0) then -- Check negated because of cross product
    return logStatus("complex.getRefractRayRay: Normal mismatch", nil, cN) end
  return cN:getNeg():RotRad(math.asin(sO)), cN
end

function complex.getRefractRayLine(cO, cD, cS, cE, ...)
  return complex.getRefractRayRay(cO, cD, cS, cE:getSub(cS), ...)
end

function complex.getRefractLineRay(cS, cE, cO, cD, ...)
  return complex.getRefractRayRay(cS, cE:getSub(cS), cO, cD, ...)
end

function complex.getRefractLineLine(cS1, cE1, cS2, cE2, ...)
  return complex.getRefractRayRay(cS1, cE1:getSub(cS1), cS2, cE2:getSub(cS2), ...)
end

function complex.getRefractRayCircle(cO, cD, cC, nR, vI, vO, xU)
  local xX = (xU and xU or complex.getIntersectRayCircle(cO, cD, cC, nR))
  if(not complex.isValid(xX)) then
    return logStatus("complex.getRefractRayCircle: Intersect mismatch", nil) end
  local cX = xX:getSub(cC):Right()
  local cR, cN = complex.getRefractRayRay(cO, cD, xX, cX, vI, vO)
  return cR, cN, cX:Set(xX)
end

function complex.getRefractLineCircle(cS, cE, cC, nR, vI, vO, xU)
  return complex.getRefractRayRay(cS, cE:getSub(cS), cC, nR, vI, vO, xU)
end

function complex.getAngRadFull()
  return metaData.__fulpi
end

function complex.getAngRadHalf()
  return metaData.__getpi
end

function complex.getAngRadFullFrac(nF)
  return (metaData.__fulpi / nF)
end

function complex.getAngRadHalfFrac(nF)
  return (metaData.__getpi / nF)
end

function complex.toDeg(nRad)
  if(math.deg) then return math.deg(nRad) end
  return (tonumber(nRad) or 0) * metaData.__radeg
end

function complex.toRad(nDeg)
  if(math.rad) then return math.rad(nDeg) end
  return (tonumber(nDeg) or 0) / metaData.__radeg
end

function metaComplex:getAngDeg() return complex.toDeg(self:getAngRad()) end

function metaComplex:setAngDeg(nA)
  return self:setAngRad(complex.toRad(tonumber(nA) or 0))
end

function metaComplex:RotDeg(nA)
  return self:RotRad(complex.toRad(tonumber(nA) or 0))
end

function metaComplex:getRotDeg(...)
  return self:getNew():RotDeg(...)
end

function metaComplex:setPolarDeg(nN, nA)
  return self:Set((tonumber(nN) or 0), 0):setAngDeg(nA)
end

function metaComplex:getAngDegVec(cV)
  return complex.toDeg(self:getAngRadVec(cV))
end

function metaComplex:getMatrix()
  local R, I = self:getParts(); return {{R, -I}, {I, R}}
end

function complex.setAction(aK, fD)
  if(not aK) then return logStatus("complex.setAction: Miss-key", false) end
  if(isFunction(fD)) then metaData.__cactf[aK] = fD; return true end
  return logStatus("complex.setAction: Non-function", false)
end

local function stringValidComplex(sStr)
  local Str = sStr:gsub("%s","") -- Remove hollows
  local S, E, B = 1, Str:len(), metaData.__bords
  while(S < E) do
    local CS, CE = Str:sub(S,S), Str:sub(E,E)
    local FS, FE = B[1]:find(CS,1,true), B[2]:find(CE,1,true)
    if((not FS) and FE) then
      return logStatus("stringValidComplex: Unbalanced end #"..CS..CE.."#",nil) end
    if((not FE) and FS) then
      return logStatus("stringValidComplex: Unbalanced beg #"..CS..CE.."#",nil) end
    if(FS and FE and FS > 0 and FE > 0) then
      if(FS == FE) then S = S + 1; E = E - 1
      else return logStatus("stringValidComplex: Bracket mismatch #"..CS..CE.."#",nil) end
    elseif(not (FS and FE)) then break end
  end; return Str, S, E
end

local function stringToComplex(sStr, nS, nE, sDel)
  local Del = tostring(sDel or ","):sub(1,1)
  local S, E, D = nS, nE, sStr:find(Del)
  if((not D) or (D < S) or (D > E)) then
    return complex.getNew(tonumber(sStr:sub(S,E)) or metaData.__valre, metaData.__valim) end
  return complex.getNew(tonumber(sStr:sub(S,D-1)) or metaData.__valre,
                        tonumber(sStr:sub(D+1,E)) or metaData.__valim)
end

local function stringToComplexI(sStr, nS, nE, nI)
  if(nI == 0) then
    return logStatus("stringToComplexI: Plain format mismatch [a+ib] or [a+bi]",nil) end
  local M = (nI - 1); local C = sStr:sub(M,M) -- There will be no delimiter symbols here
  if(nI == nE) then  -- (-0.7-2.9i) Skip symbols until +/- is reached
    while(C ~= "+" and C ~= "-" and M > 0) do M = M - 1; C = sStr:sub(M,M) end
    local vR, vI = sStr:sub(nS,M-1), sStr:sub(M,nE-1) -- Automatically change real part
              vI = (tonumber(vI) and vI or (vI.."1")) -- Process cases for (+i,-i,i)
    return complex.getNew(tonumber(vR) or metaData.__valre,
                          tonumber(vI) or metaData.__valim)
  else -- (-0.7-i2.9)
    local vR, vI = sStr:sub(nS,M-1), (C..sStr:sub(nI+1,nE))
    return complex.getNew(tonumber(vR) or metaData.__valre,
                          tonumber(vI) or metaData.__valim)
  end
end

local function tableToComplex(tTab, kRe, kIm)
  if(not tTab) then
    return logStatus("tableToComplex: Table missing", nil) end
  local R = getValueKeys(tTab, metaData.__kreal, kRe)
  local I = getValueKeys(tTab, metaData.__kimag, kIm)
  if(R or I) then
    return complex.getNew(tonumber(R) or metaData.__valre,
                          tonumber(I) or metaData.__valim) end
  return logStatus("tableToComplex: Table format not supported", complex.getNew())
end

function complex.getRandom(nL, nU, vC)
  local R = randomGetNumber(nL, nU, vC)
  local I = randomGetNumber(nL, nU, vC)
  return complex.getNew(R, I)
end

function complex.convNew(vIn, ...)
  if(complex.isValid(vIn)) then return vIn:getNew() end
  local tArg = {...}
  if(isNil(vIn)) then return complex.getNew(0,0)
  elseif(isNumber(vIn)) then return complex.getNew(vIn,tArg[1])
  elseif(isTable(vIn)) then return tableToComplex(vIn, tArg[1], tArg[2])
  elseif(isBool(vIn)) then return complex.getNew(vIn and 1 or 0,tArg[1] and 1 or 0)
  elseif(isFunction(vIn)) then local bS, vR, vI = pcall(vIn, ...)
    if(not bS) then return logStatus("complex.convNew: Function: "..vR,nil) end
    return complex.convNew(vR, vI) -- Translator function generating converter format
  elseif(isString(vIn)) then -- Remove brackets and leave the values
    local Str, S, E = stringValidComplex(vIn:gsub("*","")); if(not Str) then
      return logStatus("complex.convNew: Failed to validate <"..tostring(vIn)..">",nil) end
    Str = Str:sub(S, E); E = (E - S + 1); S = 1 -- Refresh string indexes
    local Sim, I = metaData.__ssyms    -- Prepare to find imaginary unit
    for ID = 1, #Sim do local val = Sim[ID]
      I = Str:find(val, S, true) or I; if(I) then break end end
    if(I and (I > 0)) then return stringToComplexI(Str, S, E, I)
    else return stringToComplex(Str, S, E, tArg[1]) end
  end
  return logStatus("complex.convNew: Type <"..type(vIn).."> not supported",nil)
end

local function getUnpackSplit(...)
  local tA, tC, nC, iC = {...}, {}, 0, 1
  if(complex.isValid(tA[1])) then
    while(complex.isValid(tA[1])) do tC[iC] = tA[1]
      table.remove(tA, 1); iC = iC + 1
    end; nC = (iC-1)
  else
    if(isTable(tA[1])) then
      tC = tA[1]; nC = #tA[1]; table.remove(tA, 1); end
  end; return tC, nC, unpack(tA)
end

local function getBezierCurveVertexRec(nS, tV)
  local tD, tP, nD = {}, {}, (#tV-1)
  for ID = 1, nD do tD[ID] = tV[ID+1]:getNew():Sub(tV[ID]) end
  for ID = 1, nD do tP[ID] = tV[ID]:getAdd(tD[ID]:getRsz(nS)) end
  if(nD > 1) then return getBezierCurveVertexRec(nS, tP) end
  return tP[1], tD[1]
end

function complex.getBezierCurve(...)
  local tV, nV, nT = getUnpackSplit(...)
  nT = math.floor(tonumber(nT) or metaData.__curve); if(nT < 2) then
    return logStatus("complex.getBezierCurve: Samples <"..nT.."> less than two",nil) end
  if(not (tV[1] and tV[2])) then
    return logStatus("complex.getBezierCurve: Two vertexes are needed",nil) end
  if(not complex.isValid(tV[1])) then
    return logStatus("complex.getBezierCurve: First vertex invalid <"..type(tV[1])..">",nil) end
  if(not complex.isValid(tV[2])) then
    return logStatus("complex.getBezierCurve: Second vertex invalid <"..type(tV[2])..">",nil) end
  local ID, cT, dT, tS = 1, 0, (1/nT), {}
  tS[ID] = {tV[ID]:getNew(), tV[ID+1]:getSub(tV[ID]), 0}
  cT, ID = (cT + dT), (ID + 1); while(cT < 1) do
    local vP, vD = getBezierCurveVertexRec(cT, tV)
    tS[ID] = {vP, vD, cT}; cT, ID = (cT + dT), (ID + 1)
  end; tS[ID] = {tV[nV]:getNew(), tV[nV]:getSub(tV[nV-1]), 1}; return tS
end

function complex.getCatmullRomCurveTangent(cS, cE, nT, nA)
  return ((cE:getNew():Sub(cS):getNorm()^(tonumber(nA) or 0.5))+nT)
end

function complex.getCatmullRomCurveSegment(cP0, cP1, cP2, cP3, nN, nA)
  local nT0, tC = 0, {} -- Start point is always zero
  local nT1 = complex.getCatmullRomCurveTangent(cP0, cP1, nT0, nA)
  local nT2 = complex.getCatmullRomCurveTangent(cP1, cP2, nT1, nA)
  local nT3 = complex.getCatmullRomCurveTangent(cP2, cP3, nT2, nA)
  local tTN = common.tableArrGetLinearSpace(nT1, nT2, nN)
  local cB1, cB2 = cP0:getNew(), cP0:getNew()
  local cA1, cA2, cA3 = cP0:getNew(), cP0:getNew(), cP0:getNew()
  for iD = 1, #tTN do tC[iD] = cP0:getNew(); local vTn, cTn = tTN[iD], tC[iD]
    cA1:Set(cP0):Mul((nT1-vTn)/(nT1-nT0)):Add(cP1:getMul((vTn-nT0)/(nT1-nT0)))
    cA2:Set(cP1):Mul((nT2-vTn)/(nT2-nT1)):Add(cP2:getMul((vTn-nT1)/(nT2-nT1)))
    cA3:Set(cP2):Mul((nT3-vTn)/(nT3-nT2)):Add(cP3:getMul((vTn-nT2)/(nT3-nT2)))
    cB1:Set(cA1):Mul((nT2-vTn)/(nT2-nT0)):Add(cA2:getMul((vTn-nT0)/(nT2-nT0)))
    cB2:Set(cA2):Mul((nT3-vTn)/(nT3-nT1)):Add(cA3:getMul((vTn-nT1)/(nT3-nT1)))
    cTn:Set(cB1):Mul((nT2-vTn)/(nT2-nT1)):Add(cB2:getMul((vTn-nT1)/(nT2-nT1)))
  end; return tC
end

function complex.getCatmullRomCurve(...)
  local tV, nV, nT, nA = getUnpackSplit(...)
  nT = math.floor(tonumber(nT) or metaData.__curve); if(nT < 0) then
    return logStatus("complex.getCatmullRomCurve: Samples count invalid <"..tostring(nT)..">",nil) end
  if(not (tV[1] and tV[2])) then
    return logStatus("complex.getCatmullRomCurve: Two vertexes are needed",nil) end
  if(not complex.isValid(tV[1])) then
    return logStatus("complex.getCatmullRomCurve: First vertex invalid <"..type(tV[1])..">",nil) end
  if(not complex.isValid(tV[2])) then
    return logStatus("complex.getCatmullRomCurve: Second vertex invalid <"..type(tV[2])..">",nil) end
  local vM, iC, tC = metaData.__margn, 1, {}
  local cS = tV[1]:getNew():Sub(tV[2]):Unit():Mul(vM):Add(tV[1])
  local cE = tV[nV]:getNew():Sub(tV[nV-1]):Unit():Mul(vM):Add(tV[nV])
  table.insert(tV, 1, cS); table.insert(tV, cE); nV = (nV + 2);
  for iD = 1, (nV-3) do
    local cA, cB, cC, cD = tV[iD], tV[iD+1], tV[iD+2], tV[iD+3]
    local tS = complex.getCatmullRomCurveSegment(cA, cB, cC, cD, nT, nA)
    for iK = 1, (nT+1) do tC[iC] = tS[iK]; iC = (iC + 1) end
  end; tC[iC] = tV[nV-1]:getNew();
  table.remove(tV, 1); table.remove(tV); return tC
end

function complex.getRegularPolygon(nN, cD, cO)
  local iD, eN = 2, (tonumber(nN) or 0); if(eN <= 0) then
    return logStatus("complex.getRegularPolygon: Vertexes #"..tostring(nN),nil) end
  local bO = (cO and (complex.isValid(cO) or tonumber(cO)) or false)
  local bD = (cD and (complex.isValid(cD) or tonumber(cD)) or false)
  local vD = complex.getNew(1, 0); if(bD) then vD:Set(cD) end
  local tV, nD, dD = {vD:getNew()}, (metaData.__fulpi / eN), 0
  if(bO) then tV[1]:Add(cO) end -- Offset first vertex
  while(iD <= eN) do dD = (dD + nD) -- Prepare for rotation
    tV[iD] = vD:getNew():RotRad(dD); if(bO) then tV[iD]:Add(cO) end
    if(iD ~= eN) then -- Do not overwrite the overlapped vertex
      tV[eN] = vD:getNew():RotRad(-dD); if(bO) then tV[eN]:Add(cO) end
    end; iD, eN = (iD + 1), (eN - 1)
  end; return tV
end

--[[
  Calculates the outer circle center of a polygon
  When the resulted vertices are displaced a message is
  displayed and deviation is displayed how far the margin spreads
  for the vertex selected. Returns the mean of all intersected vertices
  https://en.wikipedia.org/wiki/Circumscribed_circle
]]
function metaComplex:Circumcenter(...)
  local tV, nV = getUnpackSplit(...)
  local tI, tN, iK, tO = {}, {S = tV[nV], E = tV[1]}, 0
  for iD = 1, nV do local cC, cN = tV[iD], (tV[iD+1] or tV[1])
    tO = tN; tN = {S = cC, E = cN}; iK = (iK + 1)
    local nM, nD = tN.E:getMid(tN.S), tN.E:getSub(tN.S):Right()
    local oM, oD = tO.E:getMid(tO.S), tO.E:getSub(tO.S):Right()
    tI[iK] = complex.getIntersectRayRay(nM, nD, oM, oD)
  end; return self:Mean(tI):Deviation("Circumcenter", tI)
end

function metaComplex:getCircumcenter(...)
  return self:getNew():Circumcenter(...)
end

--[[
  Calculates the altitudes center of a polygon
  When the polygon altitude vertices are displaced a message is
  displayed and deviation is displayed how far the margin spreads
  for the vertex selected. Returns the mean of all intersected vertices
  https://en.wikipedia.org/wiki/Altitude_(triangle)
]]
function metaComplex:Orthocenter(...)
  local nM = metaData.__margn
  local dC, dN = self:getNew(), self:getNew()
  local tO, tI, tV, nV = {}, {}, getUnpackSplit(...)
  for iD = 1, nV do
    dC:Set(tV[iD+1] or tV[1]); dN:Set(tV[iD-1] or tV[nV])
    tO[iD] = tV[iD]:getProjectLine(dC, dN)
  end
  for iD = 1, nV do local nN = (tV[iD+1] and (iD+1) or 1)
    dC:Set(tO[iD]):Sub(tV[iD]); dN:Set(tO[nN]):Sub(tV[nN])
    tI[iD] = complex.getIntersectRayRay(tV[iD], dC, tV[nN], dN)
  end; return self:Mean(tI):Deviation("Orthocenter", tI)
end

function metaComplex:getOrthocenter(...)
  return self:getNew():Orthocenter(...)
end

--[[
  Calculates the median center of a polygon
  When the polygon median vertices are displaced a message is
  displayed and deviation is displayed how far the margin spreads
  for the vertex selected. Returns the mean of all intersected vertices
  https://en.wikipedia.org/wiki/Centroid
]]
function metaComplex:Centroid(...)
  local tO, tI, tV, nV = {}, {}, getUnpackSplit(...)
  for iD = 1, nV do tO[iD] = (tV[iD+1] or tV[1]):getMid(tV[iD-1] or tV[nV]) end
  for iD = 1, nV do local nN = (tV[iD+1] and (iD+1) or 1)
    local dC, dN = tO[iD]:getSub(tV[iD]), tO[nN]:getSub(tV[nN])
    tI[iD] = complex.getIntersectRayRay(tV[iD], dC, tV[nN], dN) end
  return self:Mean(tI):Deviation("Centroid", tI)
end

function metaComplex:getCentroid(...)
  return self:getNew():Centroid(...)
end

--[[
  Calculates the inner circle center of a polygon
  When the polygon cannot contain the circle a message is
  displayed and deviation is displayed how far the margin spreads
  for the vertex selected. Returns the mean of all intersected vertices
  https://en.wikipedia.org/wiki/Incenter#Definition_and_construction
]]
function metaComplex:Incenter(...)
  local nM = metaData.__margn
  local tO, tI, tV, nV = {}, {}, getUnpackSplit(...)
  local dC, dN = self:getNew(), self:getNew()
  for iD = 1, nV do
    dC:Set(tV[iD-1] or tV[nV]):Sub(tV[iD])
    dN:Set(tV[iD+1] or tV[ 1]):Sub(tV[iD])
    tO[iD] = dC:getBisect(dN) end
  for iD = 1, nV do local iN = (tV[iD+1] and (iD+1) or 1)
    tI[iD] = complex.getIntersectRayRay(tV[iD], tO[iD], tV[iN], tO[iN])
  end; return self:Mean(tI):Deviation("Incenter", tI)
end

function metaComplex:getIncenter(...)
  return self:getNew():Incenter(...)
end

--[[
  Calculates the inner circle center of a polygon
  When the polygon cannot contain the circle a message is
  displayed and deviation is displayed how far the margin spreads
  for the vertex selected. Returns the mean of all intersected vertices
  https://en.wikipedia.org/wiki/Nine-point_circle
]]
function metaComplex:NinePointCenter(...)
  local cH = self:Orthocenter(...)
  local tI, tV, nV = {}, getUnpackSplit(...)
  for iD = 1, nV do local iG = (3*(iD-1)+1)
    local cP, cN = (tV[iD+1] or tV[1]), (tV[iD-1] or tV[nV])
    tI[iG  ] = tV[iD]:getProjectLine(cP, cN)
    tI[iG+1] = cP:getMid(cN)
    tI[iG+2] = cH:getMid(tV[iD])
  end; return self:Circumcenter(tI)
end

function metaComplex:getNinePointCenter(...)
  return self:getNew():NinePointCenter(...)
end

function metaComplex:CenterMass(...)
  local tV, vT, iD, iS = {...}, self:getNew(0,0), 1, 2
  if(not isTable(tV[1])) then tV[1] = self:getNew(tV[1]) end
  local bC, tM = complex.isValid(tV[1]), {}
  if(not bC) then tV, tM, iS = tV[1], tV[2], 1 end
  while(tV[iD]) do
    local vC = self:getNew(tV[iD])
    local vM = (tonumber(bC and tV[iD+1] or tM[iD]) or 0)
    vT:Add(vC:Mul(vM)); iD = iD + iS
  end; iD = (iD - iS)/iS
  return self:Set(vT):Rsz(1/iD)
end

function metaComplex:getCenterMass(...)
  return self:getNew():CenterMass(...)
end

--[[ Interpolates a z = f(x,y) scalar over a 2D surface
 y2 c12-c22 The 2D interpolated point is betwen c[xy]
 y1 c11-c21 The point X is between x1 and x2
     x1  x2 The point Y us between y1 and y2
 Storage is done as tI{F=(01),(11),(00),(10)}
 The arguments q[xy] are the values the function has in c[xy]
 tV > The complex points of all four corners
 nV > Length od the array ( usually 4 )
 tI > Values of the function/derivate in tV
 bC > Check for the point being on square
]]
function metaComplex:getInterpolation(...)
  local tV, nV, tI ,nH, bC = getUnpackSplit(...)
  if(bC) then local nM = metaData.__margn -- Validate function square borders area
    if(math.abs(tV[1]:getReal() - tV[3]:getReal()) > nM) then
      return logStatus("complex.getInterpolation["..nH.."]: Vertex X1 mismatch",nil) end
    if(math.abs(tV[2]:getReal() - tV[4]:getReal()) > nM) then
      return logStatus("complex.getInterpolation["..nH.."]: Vertex X2 mismatch",nil) end
    if(math.abs(tV[3]:getImag() - tV[4]:getImag()) > nM) then
      return logStatus("complex.getInterpolation["..nH.."]: Vertex Y1 mismatch",nil) end
    if(math.abs(tV[1]:getImag() - tV[2]:getImag()) > nM) then
      return logStatus("complex.getInterpolation["..nH.."]: Vertex Y2 mismatch",nil) end
  end; nH, extlb = getRound(tonumber(nH or 1), 1), metaData.__extlb
  if(nH == 1) then local cT = self:getNew() -- Nearest neighbor
    local nD, nV = cT:Sub(tV[1]):getNorm2(), (tonumber(tI.F[1]) or 0)
    for iD = 2, 4 do cT:Set(self):Sub(tV[iD])
      local nT = cT:getNorm2(); if(nT < nD) then
        nD, nV = nT, (tonumber(tI.F[iD]) or 0) end; end; return nV
  elseif(nH == 2) then local x, y = self:getParts()
    local x1 = (tV[1]:getReal() + tV[3]:getReal()) / 2
    local x2 = (tV[2]:getReal() + tV[4]:getReal()) / 2
    local y1 = (tV[3]:getImag() + tV[4]:getImag()) / 2
    local y2 = (tV[1]:getImag() + tV[2]:getImag()) / 2
    local ax, bx = ((x2 - x)/(x2 - x1)), ((x - x1)/(x2 - x1))
    local ay, by = ((y2 - y)/(y2 - y1)), ((y - y1)/(y2 - y1))
    local f1 = (ax*(tonumber(tI.F[3]) or 0) + bx*(tonumber(tI.F[4]) or 0))
    local f2 = (ax*(tonumber(tI.F[1]) or 0) + bx*(tonumber(tI.F[2]) or 0))
    return ((ay*f1)+(by*f2))
  elseif(nH == 3) then
    if(extlb) then return extlb.getInterpolation(self,nH,tI)
    else return logStatus("complex.getInterpolation["..nH.."]: Extension missing",nil) end
  end; return logStatus("complex.getInterpolation["..nH.."]: Mode mismatch",nil)
end

return complex
