require "turtle"

local width = 30
local x, y = 0, 0 -- starting point
local dx, dy = 5, 3 -- x and y velocity of the box
local side = 250
local color

size(side*2, side*2)

local function square(side, color)
  for i = 1, 4 do
    move(side)
    turn(90)
  end
  if color then fill(color, 2, 2) end
end

-- draw a large square around the entire field
posn(-side, -side)
square(side*2)

updt(false) -- don't refresh screen after each drawing operation
local shot = snap() -- store snapshot
local start = time() -- store time of start

while true do
  undo(shot) -- restore snapshot
  
  -- change background color
  if (time()-start > 5) then
    fill(ranc(), -1, -1)
    shot = snap() -- retake the snapshot to remember the new background
    start = time()
  end

  posn(x, y)
  square(width, color)

  updt() -- force update of the screen
	
  x, y = x+dx, y+dy
  if x < -side or x > side-width or y < -side or y > side-width then
    -- detected a bounce; change color to a random one
    color = ranc()
  end
  if x < -side then x, dx = -side-(x+side), -dx end 
  if x > side-width then x, dx = (side-width)-(x-(side-width)), -dx end 
  if y < -side then y, dy = -side-(y+side), -dy end 
  if y > side-width then y, dy = (side-width)-(y-(side-width)), -dy end 

  local key = char()
  if key then
    if key == char('K') then dx = dx-1 end
    if key == char('L') then dx = dx+1 end
    if key == char('S') then dy = dy-1 end
    if key == char('X') then dy = dy+1 end
  end

  wait(0.01)
end

wait()
