-- Copyright (C) 2017 Deyan Dobromirov
-- Conway's game of life core library

if not debug.getinfo(3) then
  print("This is a module to load with `local lifelib = require('lifelib')`.")
  os.exit(1)
end

local common    = require("common")
local lifelib   = {}
local pairs     = pairs
local tonumber  = tonumber
local tostring  = tostring
local type      = type
local io        = io
local metaStamp = {}
local metaField = {}
local metaData  = {}

metaStamp.__type = "lifelib.shape"
metaField.__type = "lifelib.field"

metaData.__aliv  = "O"
metaData.__dead  = "-"
metaData.__path  = "game-of-life/shapes/"

local isNil        = common.isNil
local isString     = common.isString
local isTable      = common.isTable
local isAmongEq    = common.isAmongEq
local logStatus    = common.logStatus
local getSign      = common.getSign
local arMalloc2D   = common.tableArrMalloc2D
local arRotateR    = common.tableArrRotateR
local arRotateL    = common.tableArrRotateL
local getValuesSED = common.getValuesSED
local arShift2D    = common.tableArrShift2D
local arMirror2D   = common.tableArrMirror2D
local strExplode   = common.stringExplode
local strImplode   = common.stringImplode
local stringTrim   = common.stringTrim
local copyItem     = common.copyItem
local arConvert2D  = common.tableArrConvert2D
local getClamp     = common.getClamp
local getPick      = common.getPick

metaData.__init = {
  ["heart"]       = { 1,0,1,
                      1,0,1,
                      1,1,1;
                      w = 3, h = 3 },
  ["glider"]      = { 0,0,1,
                      1,0,1,
                      0,1,1;
                      w = 3, h = 3 },
  ["explode"]     = { 0,1,0,
                      1,1,1,
                      1,0,1,
                      0,1,0;
                      w = 3, h = 4 },
  ["fish"]        = { 0,1,1,1,1,
                      1,0,0,0,1,
                      0,0,0,0,1,
                      1,0,0,1,0;
                      w = 5, h = 4 },
  ["butterfly"]   = { 1,0,0,0,1,
                      0,1,1,1,0,
                      1,0,0,0,1,
                      1,0,1,0,1,
                      1,0,0,0,1;
                      w = 5, h = 5 },
  ["glidergun"]   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
                     0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
                     1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0;
                     w = 36,h = 9},
  ["block"]       = {1,1,1,1;
                     w = 2, h = 2},
  ["blinker"]     = {1,1,1;
                     w = 3, h = 1},
  ["r_pentomino"] = {0,1,1,
                     1,1,0,
                     0,1,0;
                     w = 3, h = 3},
  ["pulsar"]      ={0,0,1,1,1,0,0,0,1,1,1,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,
                    1,0,0,0,0,1,0,1,0,0,0,0,1,
                    1,0,0,0,0,1,0,1,0,0,0,0,1,
                    1,0,0,0,0,1,0,1,0,0,0,0,1,
                    0,0,1,1,1,0,0,0,1,1,1,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,1,1,1,0,0,0,1,1,1,0,0,
                    1,0,0,0,0,1,0,1,0,0,0,0,1,
                    1,0,0,0,0,1,0,1,0,0,0,0,1,
                    1,0,0,0,0,1,0,1,0,0,0,0,1,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,1,1,1,0,0,0,1,1,1,0,0;
                    w = 13, h = 13}
}



--------------------------- ALIVE / DEAD / PATH -------------------------------

function lifelib.charAliv(sA)
  if(not sA) then return metaData.__aliv end
  local sA = tostring(sA):sub(1,1)
  if(sA ~= "" and sA ~= metaData.__dead) then
    metaData.__aliv = sA; return true end
  return false
end

function lifelib.charDead(sD)
  if(not sD) then return metaData.__dead end
  local sD = tostring(sD):sub(1,1)
  if(sD ~= "" and sD ~= metaData.__aliv) then
    metaData.__dead = sD; return true end
  return false
end

function lifelib.shapesPath(sData)
  if(not sData) then return metaData.__path end
  local Typ = type(sData)
  if(Typ == "string" and sData ~= "") then
    metaData.__path = stringTrim(sData:gsub("\\","/"),"/")
    return logStatus("lifelib.shapesPath: "..metaData.__path, true)
  end; return false
end

--------------------------- RULES -------------------------------

function lifelib.getDefaultRule() -- Conway
  return {Name = "B3/S23", Data = {B = {3}, S = {2,3}}}
end

function lifelib.getRuleBS(sStr)
  local BS = {Name = tostring(sStr or ""), Data = {}}; if(BS.Name == "") then
    return logStatus("lifelib.getRuleBS: Empty rule", nil) end
  local expBS = strExplode(BS.Name, "/"); if(not (expBS[1] and expBS[2])) then
    return logStatus("lifelib.getRuleBS: Rule invalid <"..BS.Name..">", nil) end
  local kB = expBS[1]:sub(1,1); if(kB ~= "B") then
    return logStatus("lifelib.getRuleBS: Born invalid <"..BS.Name..">", nil) end
  local kS = expBS[2]:sub(1,1); if(kS ~= "S") then
    return logStatus("lifelib.getRuleBS: Surv invalid <"..BS.Name..">", nil) end
  local bI, sI = 2, 2; BS.Data[kB], BS.Data[kS] = {}, {}
  local cB, cS, tD = expBS[1]:sub(bI,bI), expBS[2]:sub(sI,sI), BS.Data
  while(cB ~= "" or cS ~= "") do
    local nB, nS = tonumber(cB), tonumber(cS)
    if(nB) then tD[kB][#tD.B + 1] = nB end
    if(nS) then tD[kS][#tD.S + 1] = nS end
    bI, sI = (bI + 1), (sI + 1)
    cB, cS = expBS[1]:sub(bI,bI), expBS[2]:sub(sI,sI)
  end; if(tD[kB][1] and tD[kS][1]) then return BS end
  return logStatus("lifelib.getRuleBS: Population fail <"..BS.Name..">", nil)
end

function lifelib.getRuleName(tRule, bSoc)
  local tDat = (tRule.Data or tRule); if(isNil(tDat)) then
    return logStatus("lifelib.expRuleName: Rule invalid", nil) end
  local tB = tDat["B"]; if(isNil(tB)) then
    return logStatus("lifelib.expRuleName: No born", nil) end
  local tS = tDat["S"]; if(isNil(tS)) then
    return logStatus("lifelib.expRuleName: No surv", nil) end
  local sNam, fNam = "", function(nN)
    local num = tonumber(nN)
    local flg = getPick(num, true, false)
    if(flg) then flg =  isAmongEq(num, 0, 8)
      local flr, cel = math.floor(num), math.ceil(num)
      if(flr ~= cel) then logStatus("lifelib.getRuleName: Float detected") end
      return getPick(flg, tostring(getPick(bSoc, cel, flr)), "")
    end; return ""
  end
  for ID = 1, #tB do sNam = sNam..fNam(tB[ID]) end; sNam = "B"..sNam.."/S"
  for ID = 1, #tS do sNam = sNam..fNam(tS[ID]) end; return sNam
end

function lifelib.getRleSettings(sStr)
  local Cpy = sStr..","
  local Len, Key = Cpy:len(), nil
  local Che, Exp, S, E = "", {}, 1, 1
  while(E <= Len) do
    Che = Cpy:sub(E,E)
    if(Che == "=") then
      Key = stringTrim(Cpy:sub(S,E-1))
      S = E + 1; E = E + 1
    elseif(Che == ",") then
      Exp[Key] = stringTrim(Cpy:sub(S,E-1))
      S = E + 1; E = E + 1
    end
    E = E + 1
  end
  return Exp
end

function lifelib.convRule(vRule) local tTmp
  if(isString(vRule)) then
    tTmp = lifelib.getRuleBS(vRule)
  elseif(isTable(vRule)) then
    if(vRule.Name and not vRule.Data) then
      tTmp = lifelib.getRuleBS(vRule.Name)
    elseif(vRule.Data and not vRule.Name) then
      tTmp = {Name = lifelib.getRuleName(vRule.Data), Data = vRule.Data}
    elseif(vRule.Name and vRule.Data) then
      if(lifelib.getRuleName(vRule.Data) == vRule.Name) then
        tTmp = {Name = vRule.Name, Data = vRule.Data}
      end
    end
  else tTmp = lifelib.getDefaultRule() end
  if(tTmp == nil) then
    return logStatus("lifelib.convRule: Incorrect life rule <"..tostring(vRule).."> !",nil) end
  return tTmp.Name, tTmp.Data;
end
------------------- SHAPE INIT --------------------

function lifelib.addStamp(sKey,tInit)
  if(not isString(sKey)) then
    return logStatus("lifelib.addStamp: Key <"..tostring(sKey).."> is "..type(sKey), false) end
  if(not isTable(tInit)) then
    return logStatus("lifelib.addStamp: Table missing <"..type(tInit)..">", false) end
  if(not isNil(metaData.__init[sKey])) then
    return logStatus("lifelib.addStamp: Key <"..sKey.."> populated !", false) end
  local ID = 1; while(tInit[ID]) do tInit[ID] = math.floor(getClamp(tInit[ID],0,1)) ID = ID + 1 end
  metaData.__init[sKey] = tInit; return true
end

local function initTable(sName)
  return metaData.__init[tostring(sName or ""):lower()]
end

local function initStringText(sStr,sDel)
  local sAlv = metaData.__aliv
  local sStr = tostring(sStr or "")
  local sDel = tostring(sDel or "\n"):sub(1,1)
  local Rows = strExplode(sStr,sDel)
  local Rall = StrImplode(Rows)
  local Shape = {w = Rows[1]:len(), h = #Rows}
  for k = 1,(Shape.w * Shape.h) do
    Shape[k] = (Rall:sub(k,k) == sAlv) and 1 or 0
  end; return Shape
end

local function initStringRle(sStr, sDel, sEnd)
  local nS, nE, Ch
  local Cnt, Ind, Lin = 1, 1, true
  local Len, sAlv = sStr:len(), metaData.__aliv
  local Num, toNum, isNum = 0, 0, false
  local Shape = {w = 0, h = 0}
  local sDel = tostring(sDel or "$"):sub(1,1)
  local sEnd = tostring(sEnd or "!"):sub(1,1)
  while(Cnt <= Len) do
    Ch = sStr:sub(Cnt,Cnt)
    if(Ch == sEnd) then Shape.h = Shape.h + 1; break end
    toNum = tonumber(Ch)
    if(not isNum and toNum) then
      -- Start of a number
      isNum = true; nS = Cnt
    elseif(not toNum and isNum) then
      -- End of a number
      isNum = false; nE = Cnt - 1
      Num   = tonumber(sStr:sub(nS,nE)) or 0
    end
    if(Num > 0) then
      if(Lin) then Shape.w = Shape.w + Num end
      while(Num > 0) do
        Shape[Ind] = (((Ch == sAlv) and 1) or 0)
        Ind = Ind + 1
        Num = Num - 1
      end;
    elseif(Ch ~= sDel and Ch ~= sEnd and not isNum) then
      if(Lin) then Shape.w = Shape.w + 1 end
      Shape[Ind] = (((Ch == sAlv) and 1) or 0)
      Ind = Ind + 1
    elseif(Ch == sDel) then Shape.h = Shape.h + 1; Lin = false end
    Cnt = Cnt + 1
  end; return Shape
end

local function initFileLif105(sName)
  local N = metaData.__path.."/lif/"..sName:lower().."_105.lif"
  local F = io.open(N,"rb"); if(not F) then
    return logStatus("initFileLif105: Invalid file: <"..N..">",nil) end
  local Line, ID, CH, Data, Alv = "", 1, 1, {}, metaData.__aliv
  local Shape = {w = 0, h = 0, Header = {}, Offset = {Cent = {}}}
  while(Line) do
    Line = F:read()
    if(not Line) then break end
    Line = stringTrim(Line)
    cFirst = Line:sub(1,1)
    leLine = Line:len()
    if(cFirst == "#") then
      local lnData  = stringTrim(Line:sub(2,-1))
      local cSecond = lnData:sub(1,1)
      if(cSecond == "P") then
        local sCoord = stringTrim(lnData:sub(2,leLine))
        local Center = sCoord:find(" ")
        Shape.Offset.Cent[1] = -tonumber(sCoord:sub(1,Center-1))
        Shape.Offset.Cent[2] = -tonumber(sCoord:sub(Center+1,sCoord:len()))
      else Shape.Header[ID] = Line:sub(2,leLine); ID = ID + 1 end
    else
      Shape.h = Shape.h + 1; Data[Shape.h] = {}
      if(leLine >= Shape.w) then Shape.w = leLine end
      for CH = 1, leLine do
        Data[Shape.h][CH] = ((Line:sub(CH,CH) == Alv) and 1 or 0) end
    end
  end; F:close()
  for ID = 1, Shape.h do
    for CH = 1, Shape.w do
      Shape[(ID-1)*Shape.w+CH] = (Data[ID][CH] or 0)
    end
  end; return Shape
end

local function initFileLif106(sName)
  local N = metaData.__path.."/lif/"..sName:lower().."_106.lif"
  local F = io.open(N,"rb"); if(not F) then
    return logStatus("initFileLif106: Invalid file: <"..N..">",nil) end
  local Line, ID, CH, Data, Offset = "", 1, 1, {}, {}
  local MinX, MaxX, MinY, MaxY, x, y
  local Shape = {w = 0, h = 0, Header = {}}
  while(Line) do
    Line = F:read()
    if(not Line) then break end
    Line = stringTrim(Line)
    cFirst, leLine = Line:sub(1,1), Line:len()
    if(not (tonumber(cFirst) or cFirst == "+" or cFirst == "-" )) then
      Shape.Header[ID] = Line:sub(2,leLine) or ""; ID = ID + 1
    else
      ID = Line:find("%s")
      if(MinX and MaxX and MinY and MaxY and x and y) then
        x = tonumber(Line:sub(1,ID-1))
        y = tonumber(Line:sub(ID+1,leLine))
        if(x and y) then
          if(x > MaxX) then MaxX = x end
          if(x < MinX) then MinX = x end
          if(y > MaxY) then MaxY = y end
          if(y < MinY) then MinY = y end
        else return logStatus("initFileLif106: Coordinates conversion failed !", nil) end
      else
        x = tonumber(Line:sub(1,ID-1)) or 0
        y = tonumber(Line:sub(ID+1,leLine)) or 0
        MaxX, MinX = x, x
        MaxY, MinY = y, y
      end
      Data[CH] = {x=x,y=y}; CH = CH + 1
    end
  end
  Shape.w = MaxX - MinX + 1
  Shape.h = MaxY - MinY + 1
  Offset.TopL = { MinX, MinY }
  Offset.TopR = { MaxX, MinY }
  Offset.BotL = { MinX, MaxY }
  Offset.BotR = { MaxX, MaxY }
  Offset.Cent = {x=math.floor(Shape.w/2), y=math.floor(Shape.h/2)}
  for ID = 1, Shape.w*Shape.h do Shape[ID] = 0 end
  CH = 1; while(Data[CH]) do
    local xyAlv = Data[CH]
    local xAlv  = Offset.Cent.x + Data[CH].x
    local yAlv  = Offset.Cent.y + Data[CH].y
    Shape[yAlv*Shape.w+xAlv+1], CH = 1, (CH + 1)
  end; F:close(); return Shape
end

local function initFileRle(sName)
  local N = metaData.__path.."/rle/"..sName:lower()..".rle"
  local F = io.open(N,"rb"); if(not F) then
    return logStatus("initFileRle: Invalid file: <"..N..">",nil) end
  local FilePos, ChCnt, leLine
  local Line, cFirst, sAlv =  "",  "", metaData.__aliv
  local nS, nE, Ind, Cel = 1, 1, 1, 1
  local Num, isNum, toNum = 0, false, nil
  local Shape = {w = 0, h = 0, Rule = {Name = "", Data = {}}, Header = {}}
  while(Line) do
    Line = F:read()
    if(not Line) then break end
    Line = stringTrim(Line)
    cFirst = Line:sub(1,1)
    leLine = Line:len()
    if(cFirst == "#") then
      Shape.Header[Ind] = Line:sub(2,leLine)
      Ind = Ind + 1
    elseif(cFirst == "x") then
      local tSet = lifelib.getRleSettings(Line)
      Shape.w = tonumber(tSet["x"])
      Shape.h = tonumber(tSet["y"])
      Shape.Rule.Name = tSet["rule"]
      Shape.Rule.Data = lifelib.getRuleBS(Shape.Rule.Name)
    else
      nS, nE, ChCnt, leLine = 1, 1, 1, Line:len()
      while(ChCnt <= leLine) do
        cFirst = Line:sub(ChCnt,ChCnt)
        if(cFirst == "!") then break end
        toNum = tonumber(cFirst)
        if    (not isNum and toNum) then isNum = true ; nS = ChCnt -- Start of a number
        elseif(not toNum and isNum) then isNum = false; nE = ChCnt - 1 -- End of a number
          Num = tonumber(Line:sub(nS,nE)) or 0 end
        if(Num > 0) then
          while(Num > 0) do
            Shape[Cel] = (((cFirst == sAlv) and 1) or 0)
            Cel = Cel + 1; Num = Num - 1
          end
        elseif(cFirst ~= "$" and cFirst ~= "!" and not isNum ) then
          Shape[Cel] = (((cFirst == sAlv) and 1) or 0); Cel = Cel + 1
        end; ChCnt = ChCnt + 1
      end
    end
  end; F:close(); return Shape
end

local function initFileCells(sName)
  local N = metaData.__path.."/cells/"..sName:lower()..".cells"
  local F = io.open(N,"rb"); if(not F) then
    return logStatus("initFileCells: Invalid file: <"..N..">",nil) end
  local x, y, Lenw, Alv = 0, 0, 0, metaData.__aliv
  local Line, ID, CH, Data = "", 1, 1, {}
  local Shape = {w = 0, h = 0, Header = {}}
  while(Line) do
    Line = F:read()
    if(not Line) then break end
    Line = stringTrim(Line)
    Firs = Line:sub(1,1)
    Lenw = Line:len()
    if(Firs ~= "!") then
      Shape.h = Shape.h + 1; Data[Shape.h] = {}
      if(Lenw >= Shape.w) then Shape.w = Lenw end
      for CH = 1, Lenw do
        Data[Shape.h][CH] = ((Line:sub(CH,CH) == Alv) and 1 or 0) end
    else
      Shape.Header[ID] = Line:sub(2,Lenw)
      ID = ID + 1
    end
  end; F:close()
  for ID = 1, Shape.h do
    for CH = 1, Shape.w do
      Shape[(ID-1)*Shape.w+CH] = (Data[ID][CH] or 0)
    end
  end; return Shape
end

local function drawConsole(F)
  local tArr, fx, fy = F:getArray(), F:getW(), F:getH()
  logStatus("Generation: "..(F:getGenerations() or "N/A"))
  local Line, Alv, Ded = "", metaData.__aliv, metaData.__dead
  for y = 1, fy do for x = 1, fx do
      Line = Line..(((tArr[y][x]~=0) and Alv) or Ded)
  end; logStatus(Line); Line = "" end
end

function lifelib.getSumStatus(nPrev,nSum,tRule)
  local sTyp = (((nPrev == 0) and "B") or ((nPrev == 1) and "S") or nil)
  if(isNil(sTyp)) then -- Check either the previous will be born and it will survive
    return logStatus("life.getSumStatus: Undefined value <"..tostring(nPrev)..">", nil) end
  for _, v in ipairs(tRule.Data[sTyp]) do
    if(v == nSum) then return 1 end
  end; return 0
end

--[[
 * Creates a field object used for living environment for the shapes ( organisms )
]]--
function lifelib.newField(nW,nH,sRule)
  local self = setmetatable({}, metaField)
  local mnW = getClamp(tonumber(nW) or 0, 1)
  local mnH = getClamp(tonumber(nH) or 0, 1)
  local miGen, mtRule = 0, {}
  local mtOld = arMalloc2D(mnW,mnH)
  local mtNew = arMalloc2D(mnW,mnH)
  local mtDraw = {["text"] = drawConsole}

  --[[
   * Internal data primitives
  ]]--
  function self:getW() return mnW end
  function self:getH() return mnH end
  function self:getSellCount() return (mnW * mnH) end
  function self:getRuleName() return mtRule.Name end
  function self:getRuleData() return mtRule.Data end
  function self:shiftXY (nX,nY) arShift2D (mtOld,mnW,mnH,(tonumber(nX) or 0),(tonumber(nY) or 0)); return self end
  function self:rollXY  (nX,nY) arRoll2D  (mtOld,mnW,mnH,(tonumber(nX) or 0),(tonumber(nY) or 0)); return self end
  function self:mirrorXY(bX,bY) arMirror2D(mtOld,mnW,mnH,bX,bY); return self end
  function self:getArray()       return mtOld end
  function self:getGenerations() return miGen end
  function self:rotR() arRotateR(mtOld,mnW,mnH); mnH,mnW = mnW,mnH; return self end
  function self:rotL() arRotateL(mtOld,mnW,mnH); mnH,mnW = mnW,mnH; return self end

  --[[
   * Apply desired rule for the stamp by using a string
   * the one provided with the initialization table.
   * If no rule can be processed the default one is used
  ]]--
  function self:setRule(vRule)
    local nam, dat = lifelib.convRule(vRule)
    if(not (nam and dat)) then return nil end
    mtRule.Name, mtRule.Data = nam, dat; return self
  end

  function self:Reset() miGen = 0
    for y = 1, mnH do
      for x = 1, mnW do
        mtNew[y][x], mtOld[y][x] = 0, 0
      end
    end; return self
  end

  --[[
   * Stamp a shape inside the field array
  ]]--
  function self:setShape(oStm,nPx,nPy)
    local px, py = ((tonumber(nPx) or 1) % mnW), ((tonumber(nPy) or 1) % mnH)
    if(oStm == nil) then
      return logStatus("lifelib.newField.setShape(Stamp,PosX,PosY): Stamp: Not present !",nil) end
    if(getmetatable(oStm) ~= metaStamp) then
      return logStatus("lifelib.newField.setShape(Stamp,PosX,PosY): Stamp: Object invalid !",nil) end
    if(mtRule.Name ~= oStm:getRuleName()) then
      return logStatus("lifelib.newField.setShape(Stamp,PosX,PosY): Stamp: Different kind of life !",nil) end
    local sw, sh, ar = oStm:getW(), oStm:getH(), oStm:getArray()
    for i = 1,sh do for j = 1,sw do
      local x, y = (px + j - 1), (py + i - 1)
      if(x > mnW) then x = x - mnW end; if(x < 1) then x = x + mnW end
      if(y > mnH) then y = y - mnH end; if(y < 1) then y = y + mnH end
      mtOld[y][x] = ar[i][j]
    end end; return self
  end
  --[[
   * Calculates the next generation
  ]]--
  function self:evoNext()
    local ym1, y, yp1, yi = (mnH - 1), mnH, 1, mnH
    while(yi > 0) do
      local xm1, x, xp1, xi = (mnW - 1), mnW, 1, mnW
      while(xi > 0) do
        local sum = mtOld[ym1][xm1] + mtOld[ym1][x] + mtOld[ym1][xp1] +
                    mtOld[ y ][xm1]                 + mtOld[ y ][xp1] +
                    mtOld[yp1][xm1] + mtOld[yp1][x] + mtOld[yp1][xp1]
        mtNew[y][x] = lifelib.getSumStatus(mtOld[y][x],sum,mtRule)
        xm1, x, xp1, xi = x, xp1, (xp1 + 1), (xi - 1)
      end; ym1, y, yp1, yi = y, yp1, (yp1 + 1), (yi - 1)
    end; mtOld, mtNew, miGen = mtNew, mtOld, (miGen + 1); return self
  end

  --[[
   * Registers a draw method under a particular key
  ]]--
  function self:regDraw(sKey,fFoo)
    if(type(sKey) == "string" and type(fFoo) == "function") then mtDraw[sKey] = fFoo
    else logStatus("lifelib.newField.regDraw(sKey,fFoo): Drawing method @"..tostring(sKey).." registration skipped !")
    end; return self
  end

  --[[
   * Visualizes the field on the screen using the draw method given
  ]]--
  function self:drwLife(sMode,...)
    local Mode = tostring(sMode or "text")
    if(mtDraw[Mode]) then mtDraw[Mode](self,...)
    else logStatus("lifelib.newField.drwLife(sMode,...): Drawing mode <"..Mode.."> not found !")
    end; return self
  end

  --[[
   * Converts the field to a number, beware they are big
  ]]--
  function self:toNumber()
    local Pow, Num, Flg = 0, 0, 0
    for i = mnH,1,-1 do for j = mnW,1,-1 do
      Flg = (mtOld[i][j] ~= 0) and 1 or 0
      Num = Num + Flg * 2 ^ Pow; Pow  = Pow + 1
    end end; return Num
  end

  --[[
   * Exports a field to a non-delimited string format
  ]]--
  function self:toString()
    local Line, Alv, Ded = "", metaData.__aliv, metaData.__dead
    for i = 1,mnH do for j = 1,mnW do
        Line = Line .. tostring((mtOld[i][j] ~= 0) and Alv or Ded)
    end end; return Line
  end

  return self:setRule(sRule)
end

--[[
 * Crates a shape ( life form ) object
]]--
function lifelib.newStamp(sName, sSrc, sExt, ...)
  local sName = tostring(sName or "")
  local sSrc, sExt = tostring(sSrc  or ""), tostring(sExt  or "")
  local tArg, isEmpty, iCnt, mtInit = {...}, true, 1, nil
  if(sSrc == "file") then
    if    (sExt == "rle"   ) then mtInit = initFileRle(sName)
    elseif(sExt == "cells" ) then mtInit = initFileCells(sName)
    elseif(sExt == "lif105") then mtInit = initFileLif105(sName)
    elseif(sExt == "lif106") then mtInit = initFileLif106(sName)
    else return logStatus("lifelib.newStamp(sName, sSrc, sExt, ...): Extension <"..
      sExt.."> not supported on the source <"..sSrc.."> for <"..sName..">",nil) end
  elseif(sSrc == "string") then
    if    (sExt == "rle" ) then mtInit = initStringRle(sName,tArg[1],tArg[2])
    elseif(sExt == "txt" ) then mtInit = initStringText(sName,tArg[1])
    else return logStatus("lifelib.newStamp(sName, sSrc, sExt, ...): Extension <"..
      sExt.."> not supported on the source <"..sSrc.."> for <"..sName">",nil) end
  elseif(sSrc == "table") then mtInit = initTable(sName)
  else return logStatus("lifelib.newStamp(sName, sSrc, sExt, ...): Source <"..
    sSrc.."> not supported for <"..sName..">",nil)
  end

  if(not mtInit) then
    return logStatus("lifelib.newStamp(sName, sSrc, sExt, ...): No initialization table",nil) end
  if(not (mtInit.w and mtInit.h)) then
    return logStatus("lifelib.newStamp(sName, sSrc, sExt, ...): Initialization table bad dimensions\n",nil) end
  if(not (mtInit.w > 0 and mtInit.h > 0)) then
    return logStatus("lifelib.newStamp(sName, sSrc, sExt, ...): Check Shape unit structure !\n",nil) end

  while(mtInit[iCnt]) do
    if(mtInit[iCnt] == 1) then isEmpty = false end; iCnt = iCnt + 1 end
  if(isEmpty) then
    return logStatus("lifelib.newStamp(sName, sSrc, sExt, ...): Shape <"..
      sName.."> empty for <"..sExt.."> <"..sSrc..">",nil) end
  local self = setmetatable({}, metaStamp)
  local mnW, mnH  = mtInit.w, mtInit.h
  local mtData = arConvert2D(mtInit,mnW,mnH)
  local mtDraw = {["text"] = drawConsole}
  local mtRule = {}

  --[[
   * Internal data primitives
  ]]--
  function self:getW() return mnW end
  function self:getH() return mnH end
  function self:rotR() arRotateR(mtData,mnW,mnH); mnH,mnW = mnW,mnH; return self end
  function self:rotL() arRotateL(mtData,mnW,mnH); mnH,mnW = mnW,mnH; return self end
  function self:getArray() return mtData end
  function self:getInit() return mtInit end
  function self:getRuleName() return mtRule.Name end
  function self:getRuleData() return mtRule.Data end
  function self:getCellCount() return (mnW * mnH) end
  function self:getGenerations() return nil end
  function self:mirrorXY(bX,bY) arMirror2D(mtData,mnW,mnH,bX,bY); return self end
  function self:rollXY(nX,nY) arRoll2D(mtData,mnW,mnH,tonumber(nX) or 0,tonumber(nY) or 0); return self end

  --[[
   * Apply desired rule for the stamp by using a string
   * the one provided with the initialization table.
   * If no rule can be processed the default one is used
  ]]--
  function self:setRule(vRule)
    local nam, dat = lifelib.convRule(vRule)
    if(not (nam and dat)) then return nil end
    mtRule.Name, mtRule.Data = nam, dat; return self
  end

  function self:Reset()
    mnW, mnH  = mtInit.w, mtInit.h
    mtData = arConvert2D(mtInit,mnW,mnH); return self
  end

  --[[
   * Registers a draw method under a particular key
  ]]--
  function self:regDraw(sKey,fFoo)
    if(type(sKey) == "string" and type(fFoo) == "function") then mtDraw[sKey] = fFoo
    else logStatus("lifelib.newStamp.regDraw(sKey,fFoo): Drawing method @"..tostring(sKey).." registration skipped !")
    end; return self
  end
  --[[
   * Visualizes the shape on the screen using the draw method given
  ]]--
  function self:drwLife(sMode,...)
    local Mode = sMode or "text"
    if(mtDraw[Mode]) then mtDraw[Mode](self, ...)
    else logStatus("lifelib.newStamp.drwLife(sMode,...): Drawing mode not found !\n")
    end; return self
  end
  --[[
   * Converts the shape to a number, beware they are big
  ]]--
  function self:toNumber()
    local Pow, Num = 0, 0
    for i = mnH,1,-1 do for j = mnW,1,-1 do
      Flg = (mtData[i][j] ~= 0) and 1 or 0
      Num = Num + Flg * 2 ^ Pow; Pow = Pow + 1
    end end; return Num
  end
  --[[
   * Exports the shape in non-delimited string format
  ]]--
  function self:toString()
    local Line, Alv, Ded = "", metaData.__aliv, metaData.__dead
    for i = 1,mnH do for j = 1,mnW do
        Line = Line .. tostring((mtData[i][j] ~= 0) and Alv or Ded)
    end end; return Line
  end
  --[[
   * Exports the shape in RLE format
  ]]--
  function self:toStringRle(sD, sE)
    local BaseCh, CurCh, Line, Cnt  = "", "", "", 0
    local sAlv, sDed = metaData.__aliv, metaData.__dead
    local sD, sE = tostring(sD):sub(1,1), tostring(sE):sub(1,1)
    for i = 1,mnH do
      BaseCh = tostring(((mtData[i][1] ~= 0) and sAlv) or sDed); Cnt = 0
      for j = 1,mnW do
        CurCh = tostring(((mtData[i][j] ~= 0) and sAlv) or sDed)
        if(CurCh == BaseCh) then Cnt = Cnt + 1
        else
          if(Cnt > 1) then Line  = Line..Cnt..BaseCh
          else Line  = Line..BaseCh end
          BaseCh, Cnt = CurCh, 1
        end
      end
      if(Cnt > 1) then Line  = Line..Cnt..BaseCh
      else Line  = Line .. BaseCh end
      if(i ~= mnH) then Line = Line..sD end
    end; return Line..sE
  end
  --[[
   * Exports the shape in text format
   * sDel the delimiter for the lines
   * bAll Draw the shape to the end of the line
  ]]--
  function self:toStringText(sDel,bTrim)
    local sAlv, sDed = metaData.__aliv, metaData.__dead
    if(sDel == sAlv) then
      return logStatus("lifelib.newStamp.toStringText(sMode,tArgs) Delimiter <"..sDel.."> matches alive","") end
    if(sDel == sDed) then
      return logStatus("lifelib.newStamp.toStringText(sMode,tArgs) Delimiter <"..sDel.."> matches dead","")  end
    local Line, Len = ""
    for i = 1,mnH do Len = mnW
      if(bTrim) then while(mtData[i][Len] == 0) do Len = Len - 1 end end
      for j = 1,Len do
        Line = Line..tostring(((mtData[i][j] ~= 0) and sAlv) or sDed)
      end; Line = Line..sDel
    end; return Line
  end

  return self:setRule(mtInit.Rule)
end

return lifelib
