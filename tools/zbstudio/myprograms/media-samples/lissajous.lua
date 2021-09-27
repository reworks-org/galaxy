require("turtle")
local common   = require("common")
local signals  = require("signals")
local colormap = require("colormap")
local chartmap = require("chartmap")

local dd, d = 1, 0.1
local ff    = 50
local fs    = 8000
local W, H  = 800, 800
local intX  = chartmap.New("interval","WinX", -dd, dd, 0, W)
local intY  = chartmap.New("interval","WinY", -dd, dd, H, 0)
local scOpe = chartmap.New("scope"):setInterval(intX, intY)
      scOpe:Update():setColor():setDelta(d, d)
local s, g = {}, {}
local tm = signals.getRamp(0,0.1,1/fs)
local w1 = signals.convLineToCircleFrq(ff*2)
local w2 = signals.convLineToCircleFrq(ff*5)
signals.setWave(s, math.sin, w1, tm)
signals.setWave(g, math.sin, w2, tm, math.pi/2)

open("Lissajous curve")
size(W, H); zero(0, 0)
updt(false) -- disable auto updates

scOpe:Draw(true, true, true):setSizeVtx(0)
scOpe:setColorDir(colr(colormap.getColorBlueRGB())):drawGraph(g, s)
updt()

wait()
