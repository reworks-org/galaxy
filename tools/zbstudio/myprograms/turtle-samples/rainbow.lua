require "turtle"

--[[
  Try varying the colors in the rainbow or adding more colors to it.
  Each color has three components: Red, Green, and Blue
  Each two letters/numbers indicate how much each of the colors to mix:
  for example, FF0000, means maximum red and none of the other two, which
  will produce a red color. Similarly, 00FF00, will give you green.
  FFFF00 will mix maximum of red and green colors, which will produce
  a yellow color. See if you can make a rainbow with your own colors... ]]

local rainbow = {
  "#FF0000", "#FF8F00", "#FFFF00", "#00FF00", "#00FFFF", "#0000FF", "#FF00FF",
}
local width = 15 -- width of each color in the rainbow
local adjustment = (width-1)*0.019 -- make each next color a bit shorter
local angle = 1

show()
jump(-165) -- move to the left to start the rainbow
turn(-90)
pnsz(width+1)
for c = 1, #rainbow do
  pncl(rainbow[c])
  for i = 1, 180/angle do
    move(angle*3-c*adjustment)
    turn(angle)
  end
  move(angle*3-c*adjustment)
  turn(90)
  jump(posn()+165) -- move to the left again to start the next color
  jump(-width * c) -- shift the color to get it start closer to the center
  turn(90)
end
hide()
wait()
