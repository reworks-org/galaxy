--[[ This program runs scripts that demonstrate what you can do with turtle graphics. To execute this program [click here](macro:run), press `F6` on the keyboard, or click on `Project | Run` menu at the top of this window. After you explore the demos you can open each of the files to study and tweak them. ]]

require "turtle"
local demos = [[
  star.lua
  text.lua
  tree45.lua
  tree60.lua
  flower.lua
  wheel.lua
  snowflake.lua
  shell.lua
  circle.lua
  spiral-two.lua
  squared-spiral.lua
  linearize.lua
  pixels.lua
  rainbow.lua
  circles.lua
  rays.lua
  many-turtles.lua
  tree30.lua
  bounce.lua
]]

local x,y = size()
local waitorg = wait
wait(1)
for file in demos:gmatch("([%w%p]+)") do
  wait = function(seconds)
    if seconds then return waitorg(seconds) end
    local x, y = size()
    posn(-x/2+10, -y/2+10)
    hide() -- hide the turtles (if any are shown)
    text("Done with '"..file.."'; pause for 2s...")
    updt() -- refresh the screen
    waitorg(2)
  end
  dofile('turtle-samples/'..file)
  size(x,y)
end
