-- Copyright (C) 2017 Deyan Dobromirov
-- A color mapping functionalities library

if not debug.getinfo(3) then
  print("This is a module to load with `local colormap = require('colormap')`.")
  os.exit(1)
end

local common    = require("common")
local math      = math
local colormap  = {}
local clMapping = {}
local clClamp   = {0, 255}
local clHash    = {
  R = {1, "r", "R", "red"  , "Red"  , "RED"  },
  G = {2, "g", "G", "green", "Green", "GREEN"},
  B = {3, "b", "B", "blue" , "Blue" , "BLUE" }
}

local logStatus       = common.logStatus
local getValueKeys    = common.getValueKeys
local stringExplode   = common.stringExplode
local getClamp        = common.getClamp
local getRound        = common.getRound
local isNil           = common.isNil

--[[ https://en.wikipedia.org/wiki/HSL_and_HSV ]]
local function projectColorHC(h, c)
  local hp, hc = (h / 60), (h % 360)
  local x  = c * (1 - math.abs((hp % 2) - 1))
  if(hc >=   0 and hc <  60) then return c, x , 0 end
  if(hc >=  60 and hc < 120) then return x, c , 0 end
  if(hc >= 120 and hc < 180) then return 0, c , x end
  if(hc >= 180 and hc < 240) then return 0, x , c end
  if(hc >= 240 and hc < 300) then return x, 0 , c end
  if(hc >= 300 and hc < 360) then return c, 0 , x end
  return 0, 0, 0
end

function colormap.getColorBlackRGB () return 0  ,  0,  0 end
function colormap.getColorRedRGB   () return 255,  0,  0 end
function colormap.getColorGreenRGB () return 0  ,255,  0 end
function colormap.getColorBlueRGB  () return 0  ,  0,255 end
function colormap.getColorYellowRGB() return 255,255,  0 end
function colormap.getColorCyanRGB  () return 0  ,255,255 end
function colormap.getColorMagenRGB () return 255,  0,255 end
function colormap.getColorWhiteRGB () return 255,255,255 end
function colormap.getColorPadRGB(pad) return pad,pad,pad end
function colormap.getColorNewRGB(r,g,b) return r, g, b end

function colormap.getColorRotateLeft(r, g, b) return g, b, r end
function colormap.getColorRotateRigh(r, g, b) return b, r, g end

function colormap.getClamp(vN)
  local nN = tonumber(vN); if(not nN) then
    return logStatus("colormap.getClamp: NAN {"..type(nN).."}<"..tostring(nN)..">") end
  return getClamp(getRound(nN, 1), clClamp[1], clClamp[2])
end

-- H [0,360], S [0,1], V [0,1]
function colormap.getColorHSV(h,s,v)
  local c = v * s
  local m = v - c
  local r, g, b = projectColorHC(h,c)
  return colormap.getClamp(clClamp[2] * (r + m)),
         colormap.getClamp(clClamp[2] * (g + m)),
         colormap.getClamp(clClamp[2] * (b + m))
end

-- H [0,360], S [0,1], L [0,1]
function colormap.getColorHSL(h,s,l)
  local c = (1 - math.abs(2*l - 1)) * s
  local m = l - 0.5*c
  local r, g, b = projectColorHC(h,c)
  return colormap.getClamp(clClamp[2] * (r + m)),
         colormap.getClamp(clClamp[2] * (g + m)),
         colormap.getClamp(clClamp[2] * (b + m))
end

-- H [0,360], C [0,1], L [0,1]
function colormap.getColorHCL(h,c,l)
  local r, g, b = projectColorHC(h,c)
  local m = l - (0.30*r + 0.59*g + 0.11*b)
  return colormap.getClamp(clClamp[2] * (r + m)),
         colormap.getClamp(clClamp[2] * (g + m)),
         colormap.getClamp(clClamp[2] * (b + m))
end

function colormap.stringColorRGB(r, g, b)
  return ("{"..tostring(r)..","..tostring(g)..","..tostring(b).."}")
end

function colormap.printColorRGB(...)
  logStatus(colormap.stringColorRGB(...))
end

function colormap.printColorMap(vKey, ...)
  if(isNil(vKey)) then
    return logStatus("colormap.printColorMap: Key missing") end
  local tRgb = clMapping[vKey]; if(not tRgb) then
    return logStatus("colormap.printColorMap: Mapping missing ["..tostring(vKey).."]") end
  local tyRgb = type(tRgb); if(tyRgb ~= "table") then
    return logStatus("colormap.printColorMap: Internal structure is ["..tyRgb.."]<"..tostring(tRgb)..">") end
  local nRgb = #tRgb; if(nRgb == 0) then
    return logStatus("colormap.printColorMap: Mapping empty ["..tostring(tRgb).."]") end
  local fRgb, nSiz, vMis = "%"..tostring(nRgb):len().."d", tRgb.Size, tRgb.Miss
  vMis = ((vMis and type(vMis) == "table") and colormap.stringColorRGB(unpack(vMis)) or "N/A")
  logStatus("Colormap ["..tostring(vKey).."]["..tostring(nSiz).."]: "..tostring(vMis))
  for ID = 1, nRgb do local tRow = tRgb[ID]
    local tyRow = type(tRow); if(tyRow == "table") then
      logStatus(fRgb:format(ID)..": "..colormap.stringColorRGB(unpack(tRow)))
    else logStatus(fRgb:format(ID)..": ["..tyRow.."]<"..tostring(tRow)..">") end
  end
end

function colormap.setColorMap(vKey,tTable,bReplace)
  if(isNil(vKey)) then
    return logStatus("colormap.setColorMaps: Key missing") end
  local tyTable = type(tTable); if(tyTable ~= "table") then
    return logStatus("colormap.setColorMap: Missing table argument",nil) end
  local tRgb = clMapping[vKey]; if(tRgb and not bReplace) then
    return logStatus("colormap.setColorMap: Exists mapping for <"..tostring(vKey)..">",nil) end
  clMapping[vKey] = tTable; if(not tTable.Size) then tTable.Size = #tTable end
  return clMapping[vKey]
end

function colormap.getColorMap(vKey, iNdex)
  if(isNil(vKey)) then
    return logStatus("colormap.getColorMap: Key missing") end
  if(isNil(iNdex)) then return clMapping[vKey] end
  local iNdex, tCl = (tonumber(iNdex) or 0)
  local tRgb = clMapping[vKey]; if(not tRgb) then
    logStatus("colormap.getColorMap: Missing mapping for <"..tostring(vKey)..">")
    return colormap.getColorBlackRGB() -- Not mapped then return black
  end; local cID = (iNdex % tRgb.Size + 1); tCl = tRgb[cID]
  if(not tCl) then tCl = tRgb.Miss end
  if(not tCl) then return colormap.getColorBlackRGB() end
  return colormap.getClamp(tCl[1]), colormap.getClamp(tCl[2]), colormap.getClamp(tCl[3])
end

function colormap.getColorMapGradient(tMap, nStp)
  local tPal, nS, iP, nT = {}, (tonumber(nStp) or 0), 0, #tMap; if(nS <= 0) then
    return logStatus("colormap.getColorMapGradient: Mismatch <"..tostring(nStp)..">", nil) end
  for iD = 1, (#tMap-1) do iP = iP + 1; tPal[iP] = {}
    local dr = (tMap[iD+1][1]-tMap[iD][1]) / (nS + 1)
    local dg = (tMap[iD+1][2]-tMap[iD][2]) / (nS + 1)
    local db = (tMap[iD+1][3]-tMap[iD][3]) / (nS + 1)
    tPal[iP][1] = colormap.getClamp(tMap[iD][1])
    tPal[iP][2] = colormap.getClamp(tMap[iD][2])
    tPal[iP][3] = colormap.getClamp(tMap[iD][3])
    for iK = 1, nS do iP = iP + 1; tPal[iP] = {}
      tPal[iP][1] = colormap.getClamp(tPal[iP-1][1]+dr)
      tPal[iP][2] = colormap.getClamp(tPal[iP-1][2]+dg)
      tPal[iP][3] = colormap.getClamp(tPal[iP-1][3]+db) end
  end; iP = iP + 1; tPal[iP] = {}
  tPal[iP][1] = colormap.getClamp(tMap[nT][1])
  tPal[iP][2] = colormap.getClamp(tMap[nT][2])
  tPal[iP][3] = colormap.getClamp(tMap[nT][3])
  return tPal
end

--[[
  Colormap for fiery-red-yellow
  https://a4.pbase.com/o6/09/60809/1/79579853.u4uTlB2w.Elephantvalleyhistogramcolors.JPG
]]--
function colormap.getColorRegion(iDepth, maxDepth, iRegions)
  local sKey, iDepth = "getColorRegion", (tonumber(iDepth) or 0); if(iDepth <= 0) then
    logStatus("colormap.getColorRegion: Missing Region depth #"..iDepth,colormap.getColorBlackRGB()) end
  local maxDepth = (tonumber(maxDepth) or 0); if(maxDepth <= 0) then
    logStatus("colormap.getColorRegion: Missing Region max depth #"..maxDepth,colormap.getColorBlackRGB()) end
  local iRegions = (tonumber(iRegions) or 0); if(iRegions <= 0) then
    logStatus("colormap.getColorRegion: Missing Regions count #"..iRegions,colormap.getColorBlackRGB()) end
  if (iDepth == maxDepth) then return colormap.getColorBlackRGB() end
  -- Cache the damn thing as it is too heavy
  if(not clMapping[sKey]) then clMapping[sKey] = {} end
  if(not clMapping[sKey][iRegions]) then clMapping[sKey][iRegions] = {} end
  local arRegions = clMapping[sKey][iRegions][maxDepth]
  if(not clMapping[sKey][iRegions][maxDepth]) then
    clMapping[sKey][iRegions][maxDepth] = {{brd = (maxDepth / iRegions), foo = function(iTer) return iTer * 2, 0, 0 end}}
    arRegions = clMapping[sKey][iRegions][maxDepth]
    local oneThird = math.ceil(0.33 * iRegions)
    for regid = 2,iRegions do
      arRegions[regid] = {}
      arRegions[regid].brd = arRegions[regid - 1].brd + arRegions[1].brd
      if(regid <= oneThird and regid > 1) then
        arRegions[regid].foo = function(iTer)
          return colormap.getClamp((((iTer - arRegions[regid-1].brd) * arRegions[oneThird-regid+1].brd)
                 * arRegions[2].brd) + arRegions[2].brd), 0, 0
        end
      else
        arRegions[regid].foo = function(iTer)
          return clClamp[2], colormap.getClamp((((iTer - arRegions[regid-1].brd) * arRegions[1].brd)
                 / arRegions[regid-2].brd) + arRegions[regid-3].brd), clClamp[1]
        end
      end
    end
  end
  local lowBorder = 1
  for regid = 1, iRegions do
    local uppBorder = arRegions[regid].brd
    if(iDepth >= lowBorder and iDepth < uppBorder) then return arRegions[regid].foo(iDepth) end
    lowBorder = arRegions[regid].brd
  end
end

local function tableToColorRGB(tTab, kR, kG, kB)
  if(not tTab) then return nil end
  local cR = colormap.getClamp(tonumber(getValueKeys(tTab, clHash.R, kR)) or clClamp[1])
  local cG = colormap.getClamp(tonumber(getValueKeys(tTab, clHash.G, kG)) or clClamp[1])
  local cB = colormap.getClamp(tonumber(getValueKeys(tTab, clHash.B, kB)) or clClamp[1])
  return cR, cG, cB
end

function colormap.convColorRGB(aIn, ...)
  local tArg, tyIn, cR, cG, cB = {...}, type(aIn)
  if(tyIn == "boolean") then
    cR = (aIn     and clClamp[2] or clClamp[1])
    cG = (tArg[1] and clClamp[2] or clClamp[1])
    cB = (tArg[2] and clClamp[2] or clClamp[1]); return cR, cG, cB
  elseif(tyIn == "string") then
    local sDe = (tArg[1] and tostring(tArg[1]) or ",")
    local tCol = stringExplode(aIn,sDe)
    cR = colormap.getClamp(tonumber(tCol[1]) or clClamp[1])
    cG = colormap.getClamp(tonumber(tCol[2]) or clClamp[1])
    cB = colormap.getClamp(tonumber(tCol[3]) or clClamp[1]); return cR, cG, cB
  elseif(tyIn == "number") then
    cR = colormap.getClamp(tonumber(aIn    ) or clClamp[1])
    cG = colormap.getClamp(tonumber(tArg[1]) or clClamp[1])
    cB = colormap.getClamp(tonumber(tArg[2]) or clClamp[1]); return cR, cG, cB
  elseif(tyIn == "table") then return tableToColorRGB(aIn, tArg[1], tArg[2], tArg[3]) end
  return logStatus("colormap.convColorRGB: Type <"..tyIn.."> not supported",nil)
end

return colormap
