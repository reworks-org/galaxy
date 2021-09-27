require("wx")
require("turtle")

local compl = require("complex")
local fract = require("fractal")
local clmap = require("colormap")
local commn = require("common")

 io.stdout:setvbuf("no")

-- z(0) = z,    z(n+1) = z(n)*z(n) + z,    n=0,1,2, ...    (1)

local logStatus   = commn.logStatus
local getClamp    = clmap.getClamp
local getColorMap = clmap.getColorMap
local getColorHSL = clmap.getColorHSL
local getColorHSV = clmap.getColorHSV
local getColorHCL = clmap.getColorHCL
local getChunkLoc = commn.stringGetChunkPath

-- https://upload.wikimedia.org/wikipedia/commons/b/b3/Mandel_zoom_07_satellite.jpg
clmap.setColorMap("wikipedia",{
  -- Size = 30          -- You can  override the size ( set automatically on creation as #Table )
  Miss = {255, 0, 255}, -- Color to use when "hole" in the array is located (ex. arr[5])
  { 66,  30,  15}, -- brown 3
  { 25,   7,  26}, -- dark violett
  {  9,   1,  47}, -- darkest blue
  {  4,   4,  73}, -- blue 5
  {  0,   7, 100}, -- blue 4
  { 12,  44, 138}, -- blue 3
  { 24,  82, 177}, -- blue 2
  { 57, 125, 209}, -- blue 1
  {134, 181, 229}, -- blue 0
  {211, 236, 248}, -- lightest blue
  {241, 233, 191}, -- lightest yellow
  {248, 201,  95}, -- light yellow
  {255, 170,   0}, -- dirty yellow
  {204, 128,   0}, -- brown 0
  {153,  87,   0}, -- brown 1
  {106,  52,   3}  -- brown 2
})

-- Changable parameters
local maxCl = 255
local W     = 400
local H     = 400
local szRe  = 2
local szIm  = 2
local nStep = 35
local nZoom = 15
local iTer  = 1000
local sfrac = "mandelbrot"
local spale = "wikipedia"
local brdcl = colr(0, 250, 100) -- Dorder color between calc lines
local brdup = nil -- true -- Update after every pixel calculation
local sTitl = "Fractal plot 2D"

--- Dinamic parameters and constants
local cexp   = compl.getNew(math.exp(1))
local w2, h2 = W/2, H/2

open(sTitl); size(W,H)
zero(0, 0); updt(false) -- disable auto updates

--[[
Some interesing places ( Mandelbrot )
S:SetArea(-0.10109910300926,-0.10109447337963,-0.95628833912037,-0.95628370949074)
S:SetArea(-0.75004543209877,-0.74996641975309,0.0031012345679011,0.0031802469135801)
S:SetArea(-1.4576971634815,-1.4576968123155,-0.0014340916323731,-0.0014337404663923)
]]

local S = fract.New("z-plane",W,H,-szRe,szRe,-szIm,szIm):SetControlWX(wx)
      S:Register("function",
        "mandelbrot", function (Z, C, R) Z:Pow(2); Z:Add(C); R[1] = Z:getAngRad(); end,
        "mandelbar", function (Z, C, R) Z:Pow(2); Z:NegIm(); Z:Add(C) end,
        "julia1", function (Z, C, R) Z:Pow(2); Z:Add(compl.convNew("-0.8+0.156i")) end,
        "julia2", function (Z, C, R) Z:Set(cexp^(Z^3) - 0.621) end,
        "julia3", function (Z, C, R) Z:Set(cexp^Z) Z:Sub(0.65) end,
        "julia4", function (Z, C, R) Z:Pow(3) Z:Add(0.4)  end,
        "julia5", function (Z, C, R) Z:Set((Z^4) * cexp^Z + 0.41 ) end,
        "julia6", function (Z, C, R) Z:Set((Z^3) * cexp^Z + 0.33 ) end)
      S:Register("palette",
        "default", function (Z, C, i)
          return (getClamp((64  * i) % maxCl)), (getClamp((128 * i) % maxCl)), (getClamp((192 * i) % maxCl)) end,
        "rediter", function (Z, C, i) return getClamp((1-(i / iTer)) * maxCl), 0, 0 end,
        "greenbl", function (Z, C, i, x, y)
          local it = i / iTer; return 0, getClamp((1 - it) * maxCl), getClamp(it * maxCl) end,
        "wikipedia", function (Z, C, i, x, y, R) return getColorMap("wikipedia",i) end,
        "region", function (Z, C, i, x, y) return clmap.getColorRegion(i,iTer,10) end,
        "hsl", function (Z, C, i, x, y) local it = i / iTer; return getColorHSL(it*360,it,it) end,
        "hsv", function (Z, C, i, x, y) local it = i / iTer; return getColorHSV(it*360,1,1) end,
        "hcl", function (Z, C, i, x, y) local it = i / iTer; return getColorHCL(it*360,it,it) end,
        "wikipedia_r", function (Z, C, i, x, y, R)
          return getColorMap("wikipedia",i * (R[1] and 1+math.floor(math.abs(R[1])) or 1)) end)

S:Update(0.1,brdcl,brdup):Draw(sfrac,spale,iTer)

while true do local key = char(); wait(0.1)
  local lx, ly = clck('ld')
  local rx, ry = clck('rd')
  if(key or (lx and ly) or (rx and ry)) then
    logStatus("KEY: {"..tostring(key).."}")
    logStatus("LFT: {"..tostring(lx)..","..tostring(ly).."}")
    logStatus("RGH: {"..tostring(rx)..","..tostring(ry).."}")
    if    (lx and ly) then S:SetCenter(lx,ly); S:Zoom( nZoom)
    elseif(rx and ry) then S:SetCenter(rx,ry); S:Zoom(-nZoom) end
    if    (key == S:GetKey("dirU")) then S:MoveCenter(0,-nStep)
    elseif(key == S:GetKey("dirD")) then S:MoveCenter(0, nStep)
    elseif(key == S:GetKey("dirL")) then S:MoveCenter(-nStep,0)
    elseif(key == S:GetKey("dirR")) then S:MoveCenter( nStep,0)
    elseif(key == S:GetKey("savE")) then save(getChunkLoc().."snapshot")
    elseif(key == S:GetKey("resS")) then 
      S:SetArea(-szRe,szRe,-szIm,szIm):SetCenter(0,0,"POS"):Zoom(1)
    end; S:Draw(sfrac,spale,iTer)
  end
end

wait()
