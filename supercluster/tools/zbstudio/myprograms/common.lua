-- Copyright (C) 2017 Deyan Dobromirov
-- A common functionalities library

local os           = os
local math         = math
local type         = type
local next         = next
local pcall        = pcall
local pairs        = pairs
local select       = select
local tonumber     = tonumber
local tostring     = tostring
local getmetatable = getmetatable
local common       = {}
local metaCommon   = {}

if not debug.getinfo(3) then
  print("This is a module to load with `local common = require('common')`.")
  os.exit(1)
end

metaCommon.__time = 0
metaCommon.__clok = 0
metaCommon.__func = {}
metaCommon.__sort = {}
metaCommon.__marg = 1e-10
metaCommon.__prct = {">","|"}
metaCommon.__fmdr = "%s?.lua"
metaCommon.__fmtb = "[%s]:%d {%s} [%d]<%s>[%s](%d)"
metaCommon.__type = {"number", "boolean", "string", "function", "table", "nil", "userdata"}
metaCommon.__syms = "1234567890abcdefghijklmnopqrstuvwxyxABCDEFGHIJKLMNOPQRSTUVWXYZ"
metaCommon.__metatable = "common.lib"
metaCommon.__nlog = {__top = 0}
metaCommon.__rmod = {{"*all"   , "Reads the whole file"},
                     {"*line"  , "Reads the next line (default)"},
                     {"*number", "Reads a number"},
                     ["*all"]=true,["*line"]=true,["*number"]=true}

metaCommon.__func["pi"] = {}
metaCommon.__func["pi"].foo = function (itr, top)
  if(top == itr) then return 1 end
  local bs, nu = ((2 * itr) + 1), ((itr + 1) ^ 2)
  return bs + nu / metaCommon.__func["pi"].foo(itr+1, top)
end
metaCommon.__func["pi"].out = function(itr)
  return (4 / metaCommon.__func["pi"].foo(0, itr))
end

metaCommon.__func["exp"] = {}
metaCommon.__func["exp"].foo = function (itr, top)
  if(top == itr) then return 1 end; local fac = 1
  for I = 1, itr do fac = fac * I end
  return (1/fac + metaCommon.__func["exp"].foo(itr+1, top))
end
metaCommon.__func["exp"].out = function(itr)
  return metaCommon.__func["exp"].foo(1, itr)
end

metaCommon.__func["phi"] = {}
metaCommon.__func["phi"].foo = function (itr, top)
  if(top == itr) then return 1 end
  return (1 + (1 / metaCommon.__func["phi"].foo(itr+1, top)))
end
metaCommon.__func["phi"].out = function(itr)
  return metaCommon.__func["phi"].foo(0, itr)
end

function common.isNil(nVal)
  return (nVal == nil)
end

function common.isNan(nVal)
  return (nVal ~= nVal)
end

function common.isInf(nVal)
  if(nVal ==  math.huge) then return true,  1 end
  if(nVal == -math.huge) then return true, -1 end
  return false
end

function common.isTable(tVal)
  return (type(tVal) == metaCommon.__type[5])
end

function common.isDryTable(tVal)
  if(not common.isTable(tVal)) then return false end
  return (next(tVal) == nil)
end

function common.isString(sVal)
  local sTy = metaCommon.__type[3]
  return (getmetatable(sTy) == getmetatable(sVal))
end

function common.isDryString(sVal)
  if(not common.isString(sVal)) then return false end
  return (sVal == "")
end

function common.isNumber(nVal)
  if(not tonumber(nVal)) then return false end
  if(nil ~= getmetatable(nVal)) then return false end
  return (type(nVal) == metaCommon.__type[1])
end

function common.isInteger(nVal)
  if(not common.isNumber(nVal)) then return false end
  local nW, nF = math.modf(nVal); return (nF == 0)
end

function common.isFunction(fVal)
  return (type(fVal) == metaCommon.__type[4])
end

function common.isBool(bVal)
  if(bVal == true ) then return true end
  if(bVal == false) then return true end
  return false
end

function common.isZero(vVal)
  return (not ((vVal + vVal) ~= vVal))
end

function common.isType(sT, iD)
  return (sT == metaCommon.__type[iD])
end

function common.logSkipAdd(...)
  local tArg, tNlg = {...}, metaCommon.__nlog
  for key, val in pairs(tArg) do
    table.insert(tNlg, tostring(val))
    tNlg.__top = tNlg.__top + 1
  end
end

function common.logSkipClear(...)
  local tNlg = metaCommon.__nlog
  if(common.isDryTable(tNlg)) then tNlg.__top = 0
    for key, val in pairs(tNlg) do tNlg[key] = nil end
  else local tArg = {...}
    for key, val in pairs(tArg) do
      local sVal = tostring(val); for ind, now in pairs(tNlg) do
        if(tostring(now):find(sVal)) then tNlg[ind] = nil end
      end
    end
  end; local nTop = tNlg.__top
  while(not tNlg[nTop] and nTop > 0) do nTop = nTop - 1 end
  tNlg.__top = nTop; collectgarbage();
end

function common.logString(anyMsg, ...)
  local tNlg = metaCommon.__nlog
  local nB, nT, sM = 1, tNlg.__top, tostring(anyMsg)
  while(nB <= nT) do
    local vB, vT = tNlg[nB], tNlg[nT]
    if(vB and sM:find(vB)) then return ... end
    if(vT and sM:find(vT)) then return ... end
    nB, nT = (nB + 1), (nT - 1)
  end; io.write(sM); return ...
end

function common.logStatus(anyMsg, ...)
  return common.logString(tostring(anyMsg).."\n", ...)
end

function common.logUnpackInfo(tInf)
  return tInf.source     , tInf.currentline, tInf.what,
         tInf.linedefined, tInf.namewhat   , tInf.name, tInf.nparams
end

function common.logCallStack(sMsg, ...)
  local iLev, sFmt = 1, metaCommon.__fmtb
  if(sMsg) then common.logStatus(tostring(sMsg)) end
  while(true) do local tInf = debug.getinfo(iLev)
    if(not tInf) then break end
    if tInf.what ~= "C" then
      common.logStatus(sFmt:format(common.logUnpackInfo(tInf)))
    end; iLev = iLev + 1
  end; return ...
end

function common.logConcat(anyMsg,aDel, ...)
  local tPar, tDat = metaCommon.__prct, {...}
  local sDel = tostring(aDel or tPar[2])
  io.write(tostring(anyMsg)..tPar[1])
  for ID = 1, #tDat do
    io.write(tostring(tDat[ID] or ""))
    if(tDat[ID+1]) then io.write(sDel) end
  end; io.write("\n")
end

function common.logCase(bCond, ...)
  if(bCond) then common.logConcat(...) end
end

-- http://lua-users.org/wiki/MathLibraryTutorial
function common.randomSetSeed(bL)
  local nT = os.time()
  if((nT - metaCommon.__time) > 0) then
    local nS = tonumber(tostring(nT):reverse():sub(1,6))
    if(bL) then common.logStatus("common.randomSetSeed: #"..nS) end
    math.randomseed(nS); metaCommon.__seed = nS
    metaCommon.__time = nT; return nS
  end; return 0
end

function common.randomGetSeed(sS)
  return (metaCommon.__seed or 0)
end

function common.randomSetString(sS)
  metaCommon.__syms = tostring(sS or "")
end

function common.randomGetNumber(nL, nU, vC)
  local iC = math.floor(tonumber(vC) or 0)
  for iD = 1, iC do math.random() end
  if(nL and nU) then return math.random(nL, nU)
  elseif(nL and not nU) then return math.random(nL) end
  return math.random()
end

function common.randomGetString(vE, vN)
  local iN = math.floor(tonumber(vN) or 0)
  local iE = math.floor(tonumber(vE) or 0)
  local sS = metaCommon.__syms
  local sR, nL = "", sS:len()
  for iD = 1, iE do
    local rN = common.randomGetNumber(1, nL, iN)
    sR = sR..sS:sub(rN, rN)
  end; return sR
end

-- Returns true when string includes atleast one letter
function common.stringHasLetter(sS)
  return (sS:find("%a") ~= nil)
end

-- Returns true when charactters are not affected by upper
function common.stringIsUpper(sS)
  return (sS:upper() == sS)
end

-- Returns true when charactters are not affected by lower
function common.stringIsLower(sS)
  return (sS:lower() == sS)
end

function common.stringImplode(tLst,sDel)
  local ID, sStr, sDel = 1, "", tostring(sDel or "")
  while(tLst and tLst[ID]) do sStr = sStr..tLst[ID]; ID = ID + 1
    if(tLst[ID] and not common.isDryString(sDel)) then sStr = sStr..sDel end
  end; return sStr
end

function common.stringExplode(sStr,sDel)
  local tLst, sC, iDx, ID, dL = {""}, "", 1, 1, (sDel:len()-1)
  while(sC) do sC = sStr:sub(iDx,iDx+dL)
    if(common.isDryString(sC)) then return tLst
    elseif(sC == sDel) then ID = ID + 1; tLst[ID], iDx = "", (iDx + dL)
    else tLst[ID] = tLst[ID]..sC:sub(1,1) end; iDx = iDx + 1
  end; return tLst
end

function common.stringCenter(sStr, vN, vC, bS)
  local nN = common.getClamp(tonumber(vN) or 0, sStr:len())
  local sC = tostring(vC or " "):sub(1,1)
  nN = ((nN - sStr:len()) / 2); nN = ((nN > 0) and nN or 0)
  if(nN > 0) then local nL, nH = math.floor(nN), math.ceil(nN)
    if(bS) then return (sC:rep(nL)..sStr..sC:rep(nH))
    else return (sC:rep(nH)..sStr..sC:rep(nL)) end
  end; return sStr
end

function common.stringTrim(sStr, sC)
  local sC = tostring(sC or "%s")
  return (sStr:match("^"..sC.."*(.-)"..sC.."*$") or sStr)
end

function common.stringPadR(sS, nL, sC)
  return sS..tostring(sC or " "):rep(nL - sS:len())
end

function common.stringPadL(sS, nL, sC)
  return tostring(sC or " "):rep(nL - sS:len())..sS
end

function common.stringGetExtension(sSrc)
  return sSrc:match("%.([^%.]+)$")
end

function common.stringStripExtension(sSrc)
  local nP = sSrc:match(".+()%.%w+$")
  if(nP) then return sSrc:sub(1, nP-1) end
  return sSrc
end

function common.stringGetFilePath(sSrc)
  return (sSrc:match("^(.*[/\\])[^/\\]-$") or "")
end

function common.stringGetFileName(sSrc)
  if(not (sSrc:find("\\") or sSrc:find("/"))) then return sSrc end
  return (sSrc:match("[\\/]([^/\\]+)$") or "")
end

function common.stringGetChunkPath()
  local sSrc = debug.getinfo(2).source
  return common.stringGetFilePath(sSrc:gsub("@","",1))
end

local function stringParseTableRec(sRc, fCnv, tInfo, nStg)
  local sIn = common.stringTrim(tostring(sRc or ""))
  if(sIn:sub(1,1)..sIn:sub(-1,-1) ~= "{}") then
    return common.logStatus("common.stringTable: Table format invalid <"..sIn..">", false) end
  local tIn, tOut = fCnv(common.stringExplode(sIn:sub(2,-2),","), ","), {}
  for ID = 1, #tIn do local sVal = common.stringTrim(tIn[ID])
    if(not common.isDryString(sVal)) then
      local tVal = fCnv(common.stringExplode(sVal,"="), "=")
      local kVal, vVal = tVal[1], tVal[2]
      if(not vVal) then -- If no key is provided but just value use default integer keys
        if(not tInfo[nStg]) then tInfo[nStg] = 0 end
        tInfo[nStg] = tInfo[nStg] + 1
        kVal, vVal = tInfo[nStg], kVal
      end; local skVal = tostring(kVal) -- Handle keys
      if(skVal:sub(1,1)..skVal:sub(-1,-1) == "[]") then skVal = skVal:sub(2,-2) end
      if(common.isDryString(kVal)) then
        return common.logStatus("common.stringTable: Table key fail at <"..vVal..">", false) end
      if(skVal:sub(1,1)..skVal:sub(-1,-1) == "\"\"") then kVal = skVal:sub(2,-2)
      elseif(tonumber(skVal)) then kVal = tonumber(skVal)
      else kVal = skVal end -- Handle values
      if(common.isDryString(vVal)) then vVal = nil
      elseif(vVal:sub(1,1)..vVal:sub(-1,-1) == "\"\"") then vVal = vVal:sub(2,-2)
      elseif(vVal:sub(1,1)..vVal:sub(-1,-1) == "{}")   then vVal = stringParseTableRec(vVal, fCnv, tInfo, nStg + 1)
      elseif(vVal == "true" or vVal == "false") then vVal = common.toBool(vVal)
      else vVal = (tonumber(vVal) or 0) end
      -- Write stuff
      tOut[kVal] = vVal
    end
  end; return tOut
end

function common.stringToTable(sRc)
  return stringParseTableRec(sRc,function(tIn, sCh)
    local aAr, aID, aIN = {}, 1, 0
    for ID = 1, #tIn do
      local sVal = common.stringTrim(tIn[ID])
      if(sVal:find("{")) then aIN = aIN + 1 end
      if(sVal:find("}")) then aIN = aIN - 1 end
      if(not aAr[aID]) then aAr[aID] = "" end
      if(aIN == 0) then aAr[aID] = aAr[aID]..sVal; aID = (aID + 1)
      else aAr[aID] = aAr[aID]..sVal..sCh end
    end; return aAr
  end, {}, 1)
end

function common.stringToNyan(sR)
  return sR:gsub("([Nn])(a)", "%1y%2")
end

function common.fileRead(pF, vM, bT)
  if(not pF) then
    return common.logStatus("common.fileGetLine: No file", "", true) end
  local tMd, nM = metaCommon.__rmod, tonumber(vM)
  local vMd = common.getPick(nM, nM, tostring(vM or tMd[2][1]))
  if(common.isNil(tMd[vMd]) and not nM) then
    local sEr = "common.fileRead: Mode missed <"..tostring(vMd)..">"; nM = 1
    while(tMd[nM]) do sEr = sEr .."\n"
      local com, desc = tMd[nM][1], tMd[nM][2]
            com = common.stringPadR("["..com.."]", 9).." > "
            sEr = sEr..("  "..com..desc); nM = nM + 1
    end; sEr = sEr..("\n  "..common.stringPadR("[N]", 9).." > Reads up to N characters")
    return common.logStatus(sEr, "", true)
  end; local sLn, bEf = pF:read(vMd), false
  if(common.isDryString(sLn) and vMd == tMd[1][1]) then return "", true end
  if(common.isNil(sLn)) then return "", true end
  if(bT) then return common.stringTrim(sLn), bEf end
  return sLn, bEf
end

-- https://www.computerhope.com/dirhlp.htm
function common.fileFind(sN, sA)
  local tSet, fFoo = metaCommon.__tfil, metaCommon.__ffil
  local sNam = sN:gsub("/+","/"):gsub("\\+","/")
  local sExt = common.stringGetExtension(sNam)
  local sArg = tostring(sA or "")
  local sTmp, sMch = os.tmpname(), ("%."..sExt.."$")
  os.execute("dir "..sNam:gsub("/","\\").." "..sArg.." >> "..sTmp)
  local fT, tO, iD = io.open(sTmp), {}, 0
  for line in fT:lines() do
    if(line:find(sMch)) then
      iD = iD + 1; tO[iD] = {}
      tO[iD] = line:sub(37, -1)
  end; end; fT:close()
  os.execute("del "..sTmp)
  return tO
end

function common.isEven(nV)
  return ((nV % 2) == 0)
end

function common.isOdd(nV)
  return ((nV % 2) ~= 0)
end

function common.getClamp(nV, nH, nL)
  if(nH and nV > nH) then return nH end
  if(nL and nV < nL) then return nL end
  return nV
end

function common.getRemap(nV, iH, iL, oH, oL, bR)
  if(bR) then
    local nK = ((nV - oL) / (oH - oL))
    return (nK * (iH - iL) + iL)
  else
    local nK = ((nV - iL) / (iH - iL))
    return (nK * (oH - oL) + oL)
  end; return nV
end

function common.getSign(nV)
  return ((nV > 0 and 1) or (nV < 0 and -1) or 0)
end

function common.getSignNon(nV)
  return ((nV >= 0 and 1) or -1)
end

function common.getSignString(nV)
  if(not common.isNumber(nV)) then
    return common.logStatus("common.getSignString: Not number",nil) end
  return (nV < 0 and "-" or "+")
end

function common.convSignString(nV)
  local sS = common.getSignString(nV); if(not sS) then
    return common.logStatus("common.convSignString: Not number",nil) end
  return (sS..tostring(math.abs(nV)))
end

function common.getType(o)
  local mt = getmetatable(o)
  if(mt and mt.__type) then
    return tostring(mt.__type)
  end; return type(o)
end

-- Defines what should return /false/ when converted to a boolean
local __tobool = {
  [0]       = true,
  ["0"]     = true,
  ["false"] = true,
  [false]   = true
}

-- http://lua-users.org/lists/lua-l/2005-11/msg00207.html
function common.toBool(anyVal)
  if(not anyVal) then return false end
  if(__tobool[anyVal]) then return false end
  return true
end

function common.getPick(bC, vT, vF)
  if(bC) then return vT end; return vF
end

function common.getDecode(vC, nM, ...)
  local tV, vO = {...}, nil
  local nV, nD = (tonumber(nM) or #tV), nil
  if(nV % 2 ~= 0) then nD = tV[nV]; nV = (nV-1) end
  for iD = 1, (nV-1), 2 do if(vC == tV[iD]) then
    return tV[iD+1] end; end; return nD
end

function common.getValueKeys(tTab, tKeys, aKey)
  if(aKey) then return tTab[aKey] end
  local out; for ID = 1, #tKeys do
    local key = tKeys[ID]; out = (tTab[key] or out)
    if(out) then return out, key end
  end; return nil
end

function common.getClamp(nN, nL, nH)
  if(nL and nN < nL) then return nL end
  if(nH and nN > nH) then return nH end; return nN
end

function common.getRoll(nN, nL, nH)
  if(nN < nL) then return nH end
  if(nN > nH) then return nL end
  return nN
end

function common.isAmong(nN, nL, nH)
  if(nN < nL) then return false end
  if(nN > nH) then return false end
  return true
end

function common.isAmongEq(nN, nL, nH)
  if(nN <= nL) then return false end
  if(nN >= nH) then return false end
  return true
end

function common.getRound(nE, nF)
  local dF = nF * common.getSign(nE)
  if(dF == 0) then return dF end
  local q, d = math.modf(nE/dF)
  return (dF * (q + (d > 0.5 and 1 or 0)))
end

function common.timeDelay(nD)
  if(nD) then local eT = (os.clock() + nD)
    while(os.clock() < eT) do end
  else while(true) do end end
end

function common.getCall(sNam, ...)
  if(not metaCommon.__func[sNam]) then
    return common.logStatus("common.getCall: Missed <"..tostring(sNam)..">", nil) end
  return pcall(metaCommon.__func[sNam].out, ...)
end

function common.setCall(sNam, fFoo, fOut)
  if(metaCommon.__func[sNam]) then
    common.logStatus("common.setCall: Replaced <"..tostring(sNam)..">") end
  if(not (type(fFoo) == "function")) then
    return common.logStatus("common.setCall: Main <"..tostring(sNam)..">", false) end
  if(not (type(fOut) == "function")) then
    return common.logStatus("common.setCall: Out <"..tostring(sNam)..">", false) end
  metaCommon.__func[sNam] = {}
  metaCommon.__func[sNam].foo = fFoo
  metaCommon.__func[sNam].out = fOut
end

function common.copyItem(obj, ccpy, seen)
  if(type(obj) ~= "table") then return obj end
  if(seen and seen[obj]) then return seen[obj] end
  local c, mt = (ccpy or {}), getmetatable(obj)
  -- Copy-constructor linked to the meta table
  if(mt) then
    if(type(c[mt]) == "function") then
      local suc, out = pcall(c[mt], obj); if(suc) then return out end
      return common.logStatus("common.copyItem("..tostring(mt).."): "..tostring(out), nil)
    elseif(mt.__type) then local mtt = mt.__type
      if(type(mtt) == "string" and type(c[mtt]) == "function") then
        local suc, out = pcall(c[mtt], obj); if(suc) then return out end
        common.logStatus("common.copyItem("..mtt.."): "..tostring(out), nil)
      end
    end
  end
  local s, res = (seen or {}), setmetatable({}, mt)
  local f = common.copyItem; s[obj] = res
  for k, v in pairs(obj) do res[f(k, c, s)] = f(v, c, s) end
  return res
end

local function logTableRec(tT,sS,tP,tD)
  local tY = metaCommon.__type
  local sS, tP = tostring(sS or "Data"), (tP or {})
  local vS, vT, vK = type(sS), type(tT), ""
  if(vT ~= tY[5]) then
    return common.logStatus("{"..vT.."}["..tostring(sS or "Data").."] = <"..tostring(tT)..">",nil) end
  if(next(tT) == nil) then
    return common.logStatus(sS.." = {}") end; common.logStatus(sS.." = {}",nil)
  for k,v in pairs(tT) do
    if(type(k) == tY[3]) then
      vK = sS.."[\""..k.."\"]"
    else sK = tostring(k)
      if(tP[k]) then sK = tostring(tP[k]) end
      vK = sS.."["..sK.."]"
    end
    if(type(v) ~= tY[5]) then
      if(type(v) == tY[3]) then
        common.logStatus(vK.." = \""..v.."\"")
      else sK = tostring(v)
        if(tP[v]) then sK = tostring(tP[v]) end
        common.logStatus(vK.." = "..sK)
      end
    else local cT, mT = common.getType(v), getmetatable(v)
      if(v == tT) then
        common.logStatus(vK.." = "..sS)
      elseif(tP[v]) then
        common.logStatus(vK.." = "..tostring(tP[v]))
      elseif(type(tD) == tY[5] and
        (type(tD[cT]) == tY[4] or type(tD[mT]) == tY[4])) then
          local vF = common.getPick(tD[cT], tD[cT], tD[mT])
          common.logStatus(vK.." = "..vF(v))
      else
        if(not tP[v]) then tP[v] = vK end
        logTableRec(v,vK,tP,tD)
      end
    end
  end
end

--[[ The non-recursive (this) function must be called
  tT -> The table to export/printout/log
  sS -> A meaningful name for the exported log
  tP -> A list of already met values to log correctly pointer to itself
  tD -> A string converter to use when a meta table or type is met
]]--
function common.logTable(tT, sS, tP, tD)
  local lS, lP = tostring(sS or "Data")
  if(tT ~= nil) then lP = {[tT] = lS} end
  if(type(tP) == "table" and lP) then
    for ptr, abr in pairs(tP) do lP[ptr] = abr end end
  logTableRec(tT, lS, lP, tD); return lP
end

function common.addLibrary(sB, ...)
  local bas = tostring(sB or ""); if(common.isDryString(bas)) then
    common.logStatus("common.addPathLibrary: Missing base path") return end
  bas = common.normFolder(bas:sub(-1,-1) == "/" and bas or bas.."/")
  local arg, fmt = {...}, metaCommon.__fmdr; bas = common.stringTrim(bas)
  if(arg[1] and common.isTable(arg[1])) then arg = arg[1] end
  for idx, val in ipairs(arg) do
    local ext = common.stringTrim(val)
    local dir = ext:gsub("/",""):gsub("%s+", "")
    if(not common.isDryString(dir)) then
      ext = common.normFolder(ext)
      local ok = os.execute("cd "..bas..ext)
      if(ok) then ext = (bas..fmt:format(ext))
        package.path = package.path..";"..ext
      else ext = bas..ext
        common.logStatus("common.addLibrary["..tostring(idx).."]: Mismatch: "..ext,nil)
      end
    else
      common.logStatus("common.addLibrary["..tostring(idx).."]: Skipped",nil)
    end
  end
end

function common.tableClear(tT)
  if(not common.isTable(tT)) then
    return common.logStatus("common.tableClear: Missing <"..tostring(tT)..">") end
  for k,v in pairs(tT) do tT[k] = nil end
end

function common.tableArrGetLinearSpace(nBeg, nEnd, nAmt)
  local fAmt = math.floor(tonumber(nAmt) or 0); if(fAmt < 0) then
    return common.logStatus("common.tableArrGetLinearSpace: Samples count invalid <"..tostring(fAmt)..">",nil) end
  local iAmt, dAmt = (fAmt + 1), (nEnd - nBeg)
  local fBeg, fEnd, nAdd = 1, (fAmt+2), (dAmt / iAmt)
  local tO = {[fBeg] = nBeg, [fEnd] = nEnd}
  while(fBeg <= fEnd) do fBeg, fEnd = (fBeg + 1), (fEnd - 1)
    tO[fBeg], tO[fEnd] = (tO[fBeg-1] + nAdd), (tO[fEnd+1] - nAdd)
  end return tO
end

function common.tableArrReverse(tT)
  if(not common.isTable(tT)) then return end
  local nN = #tT -- Store the array length
  for ID = 1, #tT do tT[ID] = tT[nN-ID+1] end
end

function common.tableArrMallocDim(vV, ...)
  local vA, tA = common.getPick(vV,common.copyItem(vV),0), {...}
  local nD, tO = table.remove(tA, 1), {}
  if(common.isNil(nD)) then return vA end
  for iD = 1, nD do
    tO[iD] = common.tableArrMallocDim(vA, unpack(tA))
  end; return tO
end

function common.tableArrMalloc(nL)
  return common.tableArrMallocDim(0, nL)
end

function common.tableArrMalloc2D(nW, nH)
  return common.tableArrMallocDim(0, nH, nW)
end

-- Transfer array data from source to destination
--[[
  tD -> Destination array
  tS -> Source Array
  tC -> Array containing copy methods
]]
function common.tableArrTransfer(tD, tS, tC)
  local iD = 1; while(not common.isNil(tS[iD]))do
    tD[iD] = common.copyItem(tS[iD], tC); iD = iD + 1 end
end

--[[
 * Converts linear array to a 2D array
 * arLin -> Linear array in format {1,2,3,4,w=2,h=2}
 * w,h   -> Custom array size
]]
function common.tableArrConvert2D(arLin, vW, vH)
  if(not arLin) then return false end
  local nW, nH = (vW or arLin.w), (vH or arLin.h)
  if(not (nW and nH)) then return false end
  if(not (nW > 0 and nH > 0)) then return false end
  arRez = common.tableArrMalloc2D(nW, nH)
  for i = 0, (nH-1) do for j = 0, (nW-1) do
      arRez[i+1][j+1] = (tonumber(arLin[i*nW+j+1]) or 0)
  end end; return arRez
end

function common.tableArrRotateR(tArr,sX,sY)
  local ii, jj, tTmp = 1, 1, common.tableArrMalloc2D(sY,sX)
  for j = 1, sX, 1 do for i = sY, 1, -1  do
      if(jj > sY) then ii, jj = (ii + 1), 1 end
      tTmp[ii][jj] = tArr[i][j]
      tArr[i][j]   = nil; jj = (jj + 1)
  end end
  for i = 1, sX do tArr[i] = {}
    for j = 1, sY do tArr[i][j] = tTmp[i][j] end
  end
end

function common.tableArrRotateL(tArr,sX,sY)
  local ii, jj, tTmp = 1, 1, common.tableArrMalloc2D(sY,sX)
  for j = sX, 1, -1 do for i = 1, sY, 1  do
      if(jj > sY) then ii, jj = (ii + 1), 1 end
      tTmp[ii][jj] = tArr[i][j]
      tArr[i][j]   = nil; jj = (jj + 1)
  end end
  for i = 1, sX do tArr[i] = {}
    for j = 1, sY do tArr[i][j] = tTmp[i][j] end
  end
end

-- Getting a start end and delta used in a for loop
function common.getValuesSED(nVal,nMin,nMax)
  local s = (nVal > 0) and nMin or nMax
  local e = (nVal > 0) and nMax or nMin
  local d = getSign(e - s)
  return s, e, d
end

function common.tableArrShift2D(tArr,sX,sY,nX,nY)
  if(not (sX > 0 and sY > 0)) then return end
  local x = math.floor(nX or 0)
  local y = math.floor(nY or 0)
  if(x ~= 0) then local M
    local sx,ex,dx = common.getValuesSED(x,sX,1)
    for i = 1,sY do for j = sx,ex,dx do
        M = (j-x); if(M >= 1 and M <= sX) then
          tArr[i][j] = tArr[i][M]
        else tArr[i][j] = 0 end
    end end
  end
  if(y ~= 0) then local M
    local sy,ey,dy = common.getValuesSED(y,sY,1)
    for i = sy,ey,dy do for j = 1,sX do
        M = (i-y); if(M >= 1 and M <= sY) then
          tArr[i][j] = tArr[M][j]
        else tArr[i][j] = 0 end
    end end
  end
end

function common.tableArrRoll2D(tArr,sX,sY,nX,nY)
  if( not( sX > 0 and sY > 0) ) then return end
  local x, y = math.floor(nX or 0), math.floor(nY or 0)
  if(y ~= 0) then
    local MaxY = (y > 0) and sY or 1
    local MinY = (y > 0) and 1 or sY
    local siY, y, arTmp  = getSign(y), (y * siY), {}
    while(y > 0) do
      for i = 1,sX do arTmp[i] = tArr[MaxY][i] end
      common.tableArrShift2D(tArr,sX,sY,0,siY)
      for i = 1,sX do tArr[MinY][i] = arTmp[i] end
      y = y - 1
    end
  end
  if(x ~= 0) then
    local MaxX = (x > 0) and sX or 1
    local MinX = (x > 0) and 1 or sX
    local siX, x, arTmp  = getSign(x), (x * siX), {}
    while(x > 0) do
      for i = 1,sY do arTmp[i] = tArr[i][MaxX] end
      common.tableArrShift2D(tArr,sX,sY,siX)
      for i = 1,sY do tArr[i][MinX] = arTmp[i] end
      x = x - 1
    end
  end
end

function common.tableArrMirror2D(tArr,sX,sY,fX,fY)
  local tTmp, s = 0, 1
  if(fY) then local e = sY
    while(s < e) do for k = 1,sX do
      tTmp       = tArr[s][k]
      tArr[s][k] = tArr[e][k]
      tArr[e][k] = tTmp end
      s, e = (s + 1), (e - 1)
    end
  end
  if(fX) then local e = sX
    while(s < e) do for k = 1,sY do
      tTmp       = tArr[k][s]
      tArr[k][s] = tArr[k][e]
      tArr[k][e] = tTmp
      end
      s, e = (s + 1), (e - 1)
    end
  end
end

-- Concatenates the array elements (...) and gives new array
function common.tableArrConcat(...)
  local tO, IK = {}, 1
  for J = 1, select("#",...) do
    local ID, tTab = 1, select(J,...)
    if(not common.isNil(tTab)) then
      while(tTab[ID]) do tO[IK] = tTab[ID]
        IK, ID = (IK + 1), (ID + 1) end
    end
  end; return tO
end

function common.tableArrExtract2D(tA,sX,eX,sY,eY)
  local tO, cX, cY = {}
  for iY = sY, eY  do for iX = sX, eX do
    if(not tO[iY-sY+1]) then tO[iY-sY+1] = {} end
    tO[iY-sY+1][iX-sX+1] = tA[iY][iX]
  end end; return tO
end

function common.tableArrReverse(tA)
  local nS, nE = 1, 1
  while(tA[nE]) do nE = nE + 1 end
  nS, nE = 1, (nE - 1)
  while(nE > nS) do
    tA[nE], tA[nS] = tA[nS], tA[nE]
    nS, nE = (nS + 1), (nE - 1)
  end
end

function common.binaryMirror(nN, nB)
  local nO = 0; for iD = 1, nB do local nM = bit.band(nN, 1)
    nN = bit.rshift(nN, 1); nO = bit.lshift(nO, 1); nO = bit.bor(nO, nM)
  end; return nO
end

-- How many bits are needed to represent /nN/
function common.binaryNeededBits(nN, bE)
  local nC = 0; while(nN ~= 0) do
    if(bE) then nN = bit.lshift(nN, 1)
    else nN = bit.rshift(nN, 1) end
    nC = nC + 1
  end; return nC
end

-- Calculate the next binary power based on nN
function common.binaryNextBaseTwo(nN)
  return (2^(math.floor(math.log(nN,2))+1))
end
-- Check if the number is a binary power
function common.binaryIsPower(nN)
  return (bit.band(nN-1,nN) == 0)
end

function common.bytesGetString(tB)
  local tO = {}; for iD = 1, #tB do
    tO[iD] = string.char(tB[iD]) end
  return table.concat(tO)
end

function common.bytesGetNumber(tB)
  local nO = 0; for iD = 1, #tB do
    nO = nO * 256; nO = nO + tB[iD]
  end; return nO
end

function common.getMargin()
  return metaCommon.__marg
end

function common.getDerivative(fF, vX)
  local nX, nM, yL, yH = (tonumber(vX) or 0), metaCommon.__marg
  local xL, xH, bS = (nX-nM), (nX+nM), true -- Derivative margin
  bS, yL = pcall(fF, xL); if(not bS) then
    return common.logStatus("common.getDerivative: "..yL ,0) end
  bS, yH = pcall(fF, xH); if(not bS) then
    return common.logStatus("common.getDerivative: "..yH, 0) end
  return (yH-yL)/(xH-xL)
end

local function sortQuick(tD, iL, iH)
  if(iL and iH and iL > 0 and iL < iH) then
    local iM = common.randomGetNumber(iH-(iL-1))+iL-1
    tD[iL], tD[iM] = tD[iM], tD[iL]
    local vM, iN = tD[iL].__val, (iL + 1); iM = iL
    while(iN <= iH)do
      if(tD[iN].__val < vM) then iM = iM + 1
        tD[iM], tD[iN] = tD[iN], tD[iM]
      end; iN = iN + 1
    end
    tD[iL], tD[iM] = tD[iM], tD[iL]
    sortQuick(tD,iL,iM-1)
    sortQuick(tD,iM+1,iH)
  end
end

local function sortSelect(tD, iL, iH)
  if(iL and iH and iL > 0 and iL < iH) then
    local iN = 1; while(tD[iN]) do
      local iS = iN + 1; while(tD[iS]) do
        if(tD[iS].__val <= tD[iN].__val) then
          tD[iN], tD[iS] = tD[iS], tD[iN]
        end; iS = iS + 1
      end; iN = iN + 1
    end
  end
end

local function sortBubble(tD, iL, iH)
  local bE, iN = false
  while(not bE) do bE = true
    for iN = iL, (iH-1), 1 do
      if(tD[iN].__val > tD[iN+1].__val) then bE = false
        tD[iN], tD[iN+1] = tD[iN+1], tD[iN]
      end
    end
  end
end

metaCommon.__sort.__ID = 1
metaCommon.__sort.__DV = "#"
metaCommon.__sort[1] = {sortQuick , "Quick sort"}
metaCommon.__sort[2] = {sortSelect, "Selection sort"}
metaCommon.__sort[3] = {sortBubble, "Bubble sort"}
metaCommon.__sort.__top = #metaCommon.__sort

function common.sortList()
  local tS = metaCommon.__sort
  local iL = tostring(tS.__top):len()
  common.logStatus("common.sortList: Sorting algorithms available:")
  for iN = 1, tS.__top do local inf = tS[iN][2]
    local sS = "] "..((iN == tS.__ID) and "@" or ">").." "..inf
    common.logStatus("  ["..common.stringPadL(tostring(iN), iL, " ")..sS)
  end
end

function common.normFolder(sD)
  local sS = tostring(sD):gsub("\\","/"):gsub("/+","/")
  return ((sS:sub(-1,-1) == "/") and sS or (sS.."/"))
end

function common.sortRem(vN)
  local tS, iN = metaCommon.__sort, (tonumber(vN) or 0)
  if(tS[iN]) then table.remove(tS, iN) end
  if(tS.__ID == iN) then tS.__ID = 1 end
  if(tS.__top > 0) then tS.__top = (tS.__top - 1) end; return true
end

function common.sortSet(vN)
  local tS, iN = metaCommon.__sort, (tonumber(vN) or 0)
  if(tS[iN]) then tS.__ID = iN; return true end; tS.__ID = 1
  return common.logStatus("common.sortSet: Invalid <"..tostring(iN)..">", 0)
end

function common.sortAdd(fF, sI)
  local tS = metaCommon.__sort; if(metaCommon.__type[4] ~= type(fF)) then
    return common.logStatus("common.sortAdd: Invalid <"..tostring(fF).."><"..tostring(sI)..">", 0) end
  tS.__top = (tS.__top + 1); tS[tS.__top] = {fF, tostring(sI)}; return (tS.__top)
end

function common.sortTable(tT, tC, bR)
  local tF = metaCommon.__sort; if(not tF[tF.__ID]) then
    return common.logStatus("common.sortTable: Sorter missing <"..tostring(tF.__ID)..">", 0) end
  local fS = tF[tF.__ID][1]; if(not type(fS) == metaCommon.__type[4]) then
    return common.logStatus("common.sortTable: Sorter mismatch <"..tostring(fS)..">", 0) end
  local tS, tK = {__top = 0}
  if(not common.isNil(tC)) then
    if(not common.isTable(tC)) then
      tK = {tC}; tK.__top = #tK
    else tK = tC; tK.__top = #tK end
  end
  for key, val in pairs(tT) do tS.__top = (tS.__top + 1)
    if(tK) then tS[tS.__top] = {__key = key, __val = ""}
      for iK = 1, tK.__top do
        local cC = metaCommon.__sort.__DV..tostring(val[tK[iK]])
        tS[tS.__top].__val = tS[tS.__top].__val..cC
      end
    else tS[tS.__top] = {__key = key, __val = val} end
  end
  local bS, sE = pcall(fS, tS, 1, tS.__top); if(not bS) then
    return common.logStatus("common.sortTable: "..tostring(sE)) end
  if(not bR) then
    for iN = 1, tS.__top do
      local rec = tS[iN]
      local key, val = rec.__key, rec.__val
      tS[iN] = tT[key]
    end
  end; return tS
end

function common.getFibonacci(vN)
  local nN, nA, nB = math.floor(tonumber(vN) or 0), 0, 1
  for iD = 1, nN do nA, nB = nB, (nA + nB) end
  return nA
end

function common.setTic()
  metaCommon.__clok = os.clock()
end

function common.getToc()
  return (os.clock() - metaCommon.__clok)
end

function common.getPermute(...)
  local tV = {...} -- Read input arguments
  local nV = #tV; if(nV <= 1) then return tV end
  if(nV == 2) then return {{tV[1], tV[2]}, {tV[2], tV[1]}} end
  local tO, iO = {}, 1
  for iD = 1, nV do
    local cP = common.copyItem(tV)
    local vP = table.remove(cP, iD) -- Pop element
    local tP = common.getPermute(unpack(cP))
    for iP = 1, #tP do table.insert(tP[iP], vP)
      tO[iO] = tP[iP]; iO = (iO + 1)
    end
  end; return tO
end

function common.getAngNorm(nA)
  local nA = (tonumber(nA) or 0)
  return ((nA + 180) % 360 - 180)
end

function common.getApprox(a,b,n)
  local so, nn = "", (tonumber(n) or 0)
  local na, nb = tonumber(a), tonumber(b)
  if(na and nb and nn > 0) then local bt = false
    local sa, sb = tostring(na), tostring(nb)
    local id = 0  -- Drop digit index
    local sd = "" -- The dropped digit
    local st = "" -- The result dring selected for division
    local nt = 0  -- Subtraction base number holder
    local nd = 0  -- Subtraction argument holder
    local ns = 0  -- Subtraction result
    local nk = 0  -- Koef for reverse multiplication
    while(nt < nb) do
      id = (id + 1)
      sd = sa:sub(id, id)
      if(sd == "") then sd = "0"; nn = (nn - 1)
        if(so == "") then so, bt = sd..".", true
        else so = so..sd end
      end
      st = st..sd; nt = tonumber(st)
    end
    nk = math.floor(nt/nb)
    so = so..tostring(nk)
    nd = nk * nb; ns = nt - nd
    st, nt = tostring(ns), ns
    if(ns ~= 0) then
      if(not bt) then so = so.."." end
      while(nn >= 0) do
        while(nt < nb and ns ~= 0) do
          id, nn = (id + 1), (nn - 1)
          sd = sa:sub(id, id)
          if(sd == "") then sd = "0" end
          st = st..sd
          nt = tonumber(st)
        end
        nk = math.floor(nt/nb)
        so = so..tostring(nk)
        nd = nk * nb; ns = nt - nd
        st, nt = tostring(ns), ns
      end
    end
  end
  return so
end

common.randomSetSeed()

return common
