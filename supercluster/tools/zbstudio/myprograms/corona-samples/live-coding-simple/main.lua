-- Sample code is MIT licensed, see http://www.coronalabs.com/links/code/license
-- Copyright (C) 2010 Corona Labs Inc. All Rights Reserved.
-- Based on Corona sample modified to work with live coding (Paul Kulchenko).

done = nil
function onUpdate(event)
  -- let the first call to onUpdate to return quickly;
  -- start the debugging during the second call to trick Corona SDK
  -- and avoid restarting the app.
  if done == nil then done = false return end
  if not done then
    require("mobdebug").start()
    done = true
  end
  
  -- try to modify the following three lines while running this code live
  -- (using `Project | Run as Scratchpad`)
  ball:setFillColor(0, 127, 255, 255)
  bkgd:setFillColor(127, 0, 127)
  xspeed, yspeed = 6, 9
end

if not initialized then
  bkgd = display.newRect(0, 0,
    display.contentWidth, display.contentHeight)
  bkgd:setFillColor(127, 0, 0)

  local radius = 40
  local xdirection, ydirection = 1, 1
  local xpos = display.contentWidth * 0.5
  local ypos = display.contentHeight * 0.5

  xspeed, yspeed = 7.5, 6.4

  ball = display.newCircle(0, 0, radius)
  ball:setFillColor(255, 255, 255, 166)
  ball.x = xpos
  ball.y = ypos

  -- Get current edges of visible screen
  local top = display.screenOriginY
  local bottom = display.viewableContentHeight + display.screenOriginY
  local left = display.screenOriginX
  local right = display.viewableContentWidth + display.screenOriginX

  local function animate(event)
    xpos = xpos + (xspeed * xdirection)
    ypos = ypos + (yspeed * ydirection)

    if (xpos > right - radius or xpos < left + radius) then
      xdirection = xdirection * -1
    end
    if (ypos > bottom - radius or ypos < top + radius) then
      ydirection = ydirection * -1
    end

    ball.x, ball.y = xpos, ypos
  end

  Runtime:addEventListener("enterFrame",
    function(event) pcall(onUpdate, event) animate(event) end)
  initialized = true
end
