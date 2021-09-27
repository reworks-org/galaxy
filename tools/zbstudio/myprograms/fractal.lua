-- Copyright (C) 2017 Deyan Dobromirov
-- A general fractals calculation library

if not debug.getinfo(3) then
  print("This is a module to load with `local fractal = require('fractal')`.")
  os.exit(1)
end

local complex      = require("complex")
local common       = require("common")
local type         = type
local math         = math
local string       = string
local tonumber     = tonumber
local tostring     = tostring
local setmetatable = setmetatable
local isNil        = common.isNil
local logStatus    = common.logStatus
local isFunction   = common.isFunction
local fractal      = {}

local mtPlaneZ     = {}
mtPlaneZ.__type    = "fractal.z-plane"
mtPlaneZ.__index   = mtPlaneZ
mtPlaneZ.__regkey  = {
  ["FUNCTION"] = {ID = 1, Dsc = "Registers a main loop function calculator"},
  ["PALETTE"]  = {ID = 2, Dsc = "Registers a display color genaration set" }
}
mtPlaneZ.__metatable = mtPlaneZ.__type
local function newPlaneZ(w,h,minw,maxw,minh,maxh)
  local imgW , imgH  = w   , h
  local minRe, maxRe = minw, maxw
  local minIm, maxIm = minh, maxh
  local imgSz, miUpdt = imgW * imgH, nil
  local imgCx, imgCy = (imgW / 2), (imgH / 2)
  local reFac = (maxRe-minRe)/(imgW) -- Re units per pixel
  local imFac = (maxIm-minIm)/(imgH) -- Im units per pixel
  local self, frcPalet, frcNames, conKeys, uZoom = {}, {}, {}, {}, 1
  local uniCr, uniCi = minRe + ((maxRe - minRe) / 2), minIm + ((maxIm - minIm) / 2)
  setmetatable(self,mtPlaneZ)
  function self:GetKey(sKey) return conKeys[tostring(sKey)] end
  function self:SetControlWX(wx)
    conKeys.dirU, conKeys.dirD = (wx["WXK_UP"]   or -1), (wx["WXK_DOWN"]  or -1)
    conKeys.dirL, conKeys.dirR = (wx["WXK_LEFT"] or -1), (wx["WXK_RIGHT"] or -1)
    conKeys.zooP, conKeys.zooM = (wx["wxEVT_LEFT_DOWN"] or -1), (wx["wxEVT_RIGHT_DOWN"] or -1)
    conKeys.resS, conKeys.savE = (wx["WXK_ESCAPE"] or -1), (wx["WXK_TAB"] or -1); return self
  end
  function self:SetArea(vminRe, vmaxRe, vminIm, vmaxIm)
    minRe, maxRe = (tonumber(vminRe) or 0), (tonumber(vmaxRe) or 0)
    minIm, maxIm = (tonumber(vminIm) or 0), (tonumber(vmaxIm) or 0)
    uniCr, uniCi = (minRe + (maxRe - minRe) / 2), (minIm + (maxIm - minIm) / 2)
    reFac = (maxRe - minRe) / (imgW) -- Re units per pixel
    imFac = (maxIm - minIm) / (imgH) -- Im units per pixel
    return self
  end
  function self:SetCenter(xCen,yCen,sMode)
    local xCen, yCen = tonumber(xCen), tonumber(yCen)
    if(not xCen) then logStatus("PlaneZ.SetCenter: X nan"); return end
    if(not yCen) then logStatus("PlaneZ.SetCenter: Y nan"); return end
    local sMode = tostring(sMode or "IMG")
    logStatus("PlaneZ.Center("..sMode.."): {"..xCen..","..yCen.."}")
    if(sMode == "IMG") then -- Use the win center in pixels
      if(xCen < 0 or xCen > imgW) then logStatus("PlaneZ.SetCenter: X outbound"); return end
      if(yCen < 0 or yCen > imgH) then logStatus("PlaneZ.SetCenter: Y outbound"); return end
      local dxP, dyP = (xCen - imgCx), (yCen - imgCy)
      local dxU, dyU = (reFac  * dxP), (imFac *  dyP)
      logStatus("PlaneZ.Center: DX = "..dxP.." >> "..dxU)
      logStatus("PlaneZ.Center: DY = "..dyP.." >> "..dyU)
      self:SetArea((minRe + dxU), (maxRe + dxU), (minIm + dyU), (maxIm + dyU))
    elseif(sMode == "POS") then -- Use the fractal center
      local disRe = (maxRe - minRe) / 2
      local disIm = (maxIm - minIm) / 2
      self:SetArea((xCen - disRe), (xCen + disRe), (yCen - disIm), (yCen + disIm))
    else logStatus("PlaneZ.SetCenter: Mode <"..sMode.."> missing") end
    return self
  end
  function self:MoveCenter(dX, dY)
    logStatus("PlaneZ.MoveCenter: {"..dX..","..dY.."}")
    return self:SetCenter(imgCx + (tonumber(dX) or 0), imgCy + (tonumber(dY) or 0))
  end
  function self:Zoom(nZ)
    local nZoom = (tonumber(nZ) or 0)
    if(nZoom == 0) then logStatus("PlaneZ.Zoom("..tostring(nZoom).."): Skipped") return end
    local disRe, disIm = ((maxRe - minRe) / 2), ((maxIm - minIm) / 2)
    local midRe, midIm = (minRe + disRe), (minIm + disIm)
    if(nZoom > 0) then uZoom = uZoom * math.abs(nZoom)
      self:SetArea(midRe - disRe / nZoom, midRe + disRe / nZoom,
                   midIm - disIm / nZoom, midIm + disIm / nZoom)
    elseif(nZoom < 0) then
      self:SetArea(midRe + disRe * nZoom, midRe - disRe * nZoom,
                   midIm + disIm * nZoom, midIm - disIm * nZoom)
      uZoom = uZoom / math.abs(nZoom)
    end; return self
  end
  function self:Register(...) local tArgs = {...}
    local sMode = tostring(tArgs[1] or "N/A")
    local tRKey = mtPlaneZ.__regkey
    for iNdex = 2, #tArgs, 2 do
      local key = tArgs[iNdex]; if(not key) then
        logStatus("PlaneZ.Register: Key missing <"..iNdex..">"); return end
      local foo = tArgs[iNdex + 1]; if(not isFunction(foo)) then
        logStatus("PlaneZ.Register: Non-function under ["..iNdex.."]<"..key..">"); return end
      local tMode = tRKey[sMode:upper()] -- Read given registration mode
      if(not isNil(tMode)) then local iD = (tonumber(tMode.ID) or 0)
        if    (iD == 1) then frcNames[key] = foo
        elseif(iD == 2) then frcPalet[key] = foo
        else logStatus("PlaneZ.Register: Skip <"..sMode.."> mode under ID ["..iD.."] !") end
      else logStatus("PlaneZ.Register: Mode mismatch for <"..sMode.."> !")
        for k, v in pairs(tRKey) do
          logStatus("PlaneZ.Register: Available: "..("%-10s"):format("["..k.."]")..": "..tostring(v.Dsc or "N/A"))
        end; return
      end
    end; return self
  end
  function self:Update(fUpdt,clbrd,bBrdP)
    brdCl, bbrdP = clbrd, bBrdP
    if(fUpdt) then local nUpd = (tonumber(fUpdt) or 0)
      local nw, nf = math.modf(nUpd) -- Extract fraction
      if    (nw ~= 0 and nf == 0) then miUpdt = math.abs(nw)
      elseif(nw == 0 and nf ~= 0) then miUpdt = math.abs(math.ceil(nf * imgSz))
      elseif(nw ~= 0 and nf ~= 0) then miUpdt = math.abs(math.ceil(fUpdt))
      elseif(nw == 0 and nf == 0) then miUpdt = nil end
      logStatus("PlaneZ.Updt: {"..nUpd.."}")
    end; return self
  end

  function self:Draw(sName,sPalet,maxItr)
    local maxItr = (tonumber(maxItr) or 0); if(maxItr < 1) then
      logStatus("PlaneZ.Draw: Iteration depth #"..tostring(maxItr).." invalid"); return end
    local sName, r, g, b, iDepth, isInside, nrmZ = tostring(sName), 0, 0, 0, 0, true
    local C, Z, R, P = complex.getNew(), complex.getNew(), {}, 0
    logStatus("PlaneZ.Zoom: {"..uZoom.."}")
    logStatus("PlaneZ.Cent: {"..uniCr..","..uniCi.."}")
    logStatus("PlaneZ.Area: {"..minRe..","..maxRe..","..minIm..","..maxIm.."}")
    for y = 0, imgH do -- Row
      if(brdCl and not miUpdt) then pncl(brdCl); line(0,y,imgW,y); updt() end
      C:setImag(minIm + y*imFac)
      for x = 0, imgW do -- Col
        if(brdCl and bbrdP and not miUpdt) then updt() end
        C:setReal(minRe + x*reFac); Z:Set(C); isInside = true
        for n = 1, maxItr do
          nrmZ = Z:getNorm2()
          if(nrmZ > 4) then iDepth, isInside = n, false; break end
          if(not frcNames[sName]) then
            logStatus("PlaneZ.Draw: Invalid fractal name <"..sName.."> given"); return end
          frcNames[sName](Z, C, R) -- Call the fractal formula
        end; r, g, b = 0, 0, 0
        if(not isInside) then
          if(not frcPalet[sPalet]) then
            logStatus("PlaneZ.Draw: Invalid pallet <"..tostring(sPalet).."> given"); return end
          r, g, b = frcPalet[sPalet](Z, C, iDepth, x, y, R) -- Call the fractal coloring
        end
        pncl(colr(r, g, b)); pixl(x,y); P = P + 1
      end
      if(miUpdt) then
        if(P > miUpdt) then updt(); P = 0 end
      else updt() end
    end; updt(); return self
  end; return self
end

local mtTreeY = {}
      mtTreeY.__type  = "fractal.ytree"
      mtTreeY.__index = mtTreeY
      mtTreeY.__metatable = mtTreeY.__type
local function newTreeY(iMax, clDraw)
  local mnLev, mnMax, mcDrw = 1, (tonumber(iMax) or 0), clDraw
  local self = setmetatable({__data = {}}, mtTreeY)
  if(mnMax <= 0) then return logStatus("YTree depth invalid <"..tostring(mnMax)..">") end
  function self:getMax() return mnMax end
  function self:getLev() return mnLev end
  function self:setColor(clDraw) mcDrw = clDraw; return self end
  function self:Allocate(tBrn)
    local tBranch = (tBrn or self.__data)
    if(not tBranch.__lev) then tBranch.__lev = self:getLev() end
    if(tBranch.__lev == 0) then tBranch.__max = self:getMax() end
    if(tBranch.__lev <= self:getMax()) then
      tBranch["<"] = {__lev = (tBranch.__lev + 1)}
      tBranch[">"] = {__lev = (tBranch.__lev + 1)}
      self:Allocate(tBranch["<"]):Allocate(tBranch[">"])
    end; return self
  end
  function self:Draw(oX, oY, dX, dY, fW, nW, tBrn)
    if(not mcDrw) then return end
    if(fW and nW) then fW(nW) end
    local tBranch = (tBrn or self.__data)
    if(tBranch.__lev <= self:getMax()) then
      local o2Y, dX2, dY2 = (oY+dY+dY), dX/2, dY/2
      local odY, opX, omX = (oY+dY), (oX+dX), (oX-dX)
      pncl(mcDrw); line(oX, oY, oX, odY)
      line(oX, odY, omX, o2Y); self:Draw(omX, o2Y, dX2, dY2, fW, nW, tBranch["<"])
      line(oX, odY, opX, o2Y); self:Draw(opX, o2Y, dX2, dY2, fW, nW, tBranch[">"])
    end
  end; return self
end

function fractal.New(sType, ...)
  local sType = "fractal."..tostring(sType or "")
  if(sType == mtPlaneZ.__type) then return newPlaneZ(...) end
  if(sType == mtTreeY.__type) then return newTreeY(...) end
end

return fractal
