require("turtle")
local com = require("common")
local col = require("colormap")
local crt = require("chartmap")
local sig = require("signals")

-- https://blogs.msdn.microsoft.com/dawate/2009/06/23/intro-to-audio-programming-part-2-demystifying-the-wav-format/

local wData, smpData = sig.readWave("media-samples/crickets.wav")

if(wData) then
  com.logTable(wData)
  com.logStatus("Array  samples: <"..(smpData[1].__top-1)..">")
  com.logStatus("Record samples: <"..wData["DATA"]["dwSamplesPerChan"]..">")
  local smpFrac = math.floor(wData["DATA"]["dwSamplesPerChan"]/2)
  local tData, iTop = smpData[1], smpFrac
  local dX,dY = smpFrac/40, 0.1
  local W , H = 800, 500
  local minX, maxX = 1, iTop
  local minY, maxY = -1.2, 1.2
  local greyLevel  = 200
  local intX  = crt.New("interval","WinX", minX, maxX, 0, W)
  local intY  = crt.New("interval","WinY", minY, maxY, H, 0)
  local trWAV = crt.New("tracer","WAV"):setInterval(intX, intY)
  local clGry = colr(greyLevel,greyLevel,greyLevel)
  local clBlu = colr(col.getColorBlueRGB())
  local clRed = colr(col.getColorRedRGB())
  local clBlk = colr(col.getColorBlackRGB())
  local scWAV = crt.New("scope"):setInterval(intX, intY):setBorder(minX, maxX, minY, maxY)
        scWAV:setSize(W, H):setDelta(dX, dY):setColor(clBlk, clGry)
   
  open("Wave file plotter")
  size(W,H); zero(0, 0); updt(false) -- disable auto updates

  scWAV:Draw(true, false, true)

  for i = 1, iTop do
    trWAV:Write(i, tData[i]):Draw(clBlu); updt()
  end
end

wait()
