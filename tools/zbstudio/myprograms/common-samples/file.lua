local cmn = require("common")

local fNam = "common-samples/data.txt"

local f = io.open(fNam, "wb")
--[[
  * http://www.lua.org/pil/21.1.html
  * Here the user can give the function a bunch of read methods
  * [*all]    > Reads the whole file
  * [*line]   > Reads the next line (default)
  * [*number] > Reads a number
  * [N]       > Reads up to N characters
]]
local mode = nil

-- Automatically trim every string gathered
local trim = true

-- How to pad the result ( right-pad )
local pad = cmn.stringPadR

if(f) then local I
  
  local tD = {
    "    aaaaaaaaaaa    ",
    "bbbbbbbbbbb    ",
    "    ccccccccccc",
    "ddddddddddd",
  } -- Write a bunch of values in the file
  
  I = 1; while(tD[I]) do
    f:write(tD[I].."\n"); I = I + 1
  end; f:flush(); f:close()
  
  f = io.open(fNam, "rb")
  
  if(f) then
    local l, e = cmn.fileRead(f, mode, trim)
    --[[
      * The read function takes a stream (file) as first
      * argument and mode for the second
      * The default mode is line by line if a value is not given
      * If a wrong mode is given, an error will be produced
      * The function will always return string for the first argument
      * and EOF flag for the second.
    ]]
    while(not e) do
      cmn.logStatus("Result: "..pad("<"..l..">", 22).." : "..tostring(e))
      l, e = cmn.fileRead(f, mode, trim)
    end
  end
  
  f:close()
end
