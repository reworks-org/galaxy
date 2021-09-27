require("turtle")
local common   = require("common")
local complex  = require("complex")
local chartmap = require("chartmap")
local signals  = require("signals")
local colormap = require("colormap")

local ws = 125               -- Signal frequency [Hz]
local rt = 100               -- Te amount of times to call DFT []
local fs = 2000              -- Sampling rate [Hz]
local et = 1/10              -- End time [s]
local pr = 1 / fs            -- Time per sample [s]
local s, g, i = {}, {}, 1    -- Arry containing samples and time
local W, H = 1000, 600
local intX  = chartmap.New("interval","WinX", 0, et, 0, W)
local intY  = chartmap.New("interval","WinY", -1, 1, H, 0)
local scOpe = chartmap.New("scope"):setInterval(intX, intY)
      scOpe:Update():setColor():setDelta(et / 10, 0.1)

local t  = signals.getRamp(0, et, pr)
signals.setWave(s, math.sin, signals.convLineToCircleFrq(ws), t)

-- Remove the comment from the line below to see the change in the spectrum
-- It simply adds one sinewave with double the frequency to the output signal
-- signals.setWave(s, math.sin, signals.convLineToCircleFrq(2*ws), t, 0, s, 0.7, 0.5)

signals.setWeight(g,s,signals.winNuttall(#s))

-- Try commenting this line to remove the phase factor cashe
signals.setPhaseFactorDFT(common.binaryNextBaseTwo(#s))

local tim, dft = os.clock()
for i = 1, rt do
  dft = signals.getForwardDFT(g)
  common.logString(".")
end; common.logString("\n")
tim = ((os.clock()-tim) * 1000)

open("Discrete Fourier Transform (DFT) graph (red), sampled signal (blue) and weighted (green)")
size(W, H); zero(0, 0)
updt(false) -- disable auto updates

scOpe:Draw(true, false, true):setSizeVtx(0)
scOpe:setColorDir(colr(colormap.getColorGreenRGB())):drawGraph(g, t); updt()
scOpe:setColorDir(colr(colormap.getColorBlueRGB())):drawGraph(s, t); updt()

local xft, mft, tft, aft = {}, 0, 0, #dft
for i = 1, aft/2 do
  local nrm = dft[i]:getNorm()
  if(nrm > mft) then mft, tft = nrm, i end
end

for i = 1, aft do
  local nrm = dft[i]:getNorm()
  xft[i] = (nrm / mft)
end

local dhz = (fs/(#xft-1))

intX:setBorderIn(1, #dft); intY:setBorderIn(0, 1)
scOpe:setInterval(intX, intY):Update():setSizeVtx(2)
scOpe:setColorDir(colr(colormap.getColorRedRGB())):drawStem(xft); updt()

common.logStatus("DFT Input signal sample array size is "..#s)
common.logStatus("DFT Main frequency is at "..(tft-1)*dhz.. " of "..ws)
common.logStatus("DFT scale uses "..dhz.." Hz per division.")
common.logStatus("DFT was calculated "..rt.." times for "..tim.." milliseconds and "..#xft.." points")

wait()
