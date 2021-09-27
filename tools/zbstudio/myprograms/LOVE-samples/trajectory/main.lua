function love.load(arg)
  if arg and arg[#arg] == "-debug" then require("mobdebug").start() end
end

-- trajectory logic from http://developer.coronalabs.com/code/trajectory-plotting

local rad = 180 / math.pi
local g = 9.8
local function vxf(v, a) return v * math.cos(a / rad) end
local function vyt(v, a, t) return v * math.sin(a / rad) - g * t end
local function yt(v, a, t) return v * math.sin(a / rad) * t - 0.5 * g * t * t end

local function totalRangeHeightFlightTime(v, ag)
  local h = vyt(v, ag, 0) * vyt(v, ag, 0) / (2 * g)
  local t = 2 * vyt(v, ag, 0) / g
  local r = v * v * math.sin(2 * ag / rad) / g
  return r, h, t
end

local function positionAtTime(v, ag, t)
  local vx = vxf(v,ag) -- horizontal velocity
  local x = vx * t -- horizontal distance
  local vy = vyt(v, ag, t) -- vertical velocity
  local y = yt(v, ag, t) -- height at time 't'
  return x, y, vx, vy
end

-- returns a collection of points calculated as the trajectory of the object
local function calcTrajectoryPoints(velocity, angle, iteration)
  if (not iteration) then iteration = 0.1 end
  local r, h, f = totalRangeHeightFlightTime(velocity, angle) -- total range, height and flight time
  local points = {}
  for t=0, f, iteration do
    local x, y, vx, vy = positionAtTime(velocity, angle, t)
    points[#points+1] = { x=x, y=y, time=t, vx=vx, vy=vy }
  end
  return points, r, h, f
end

local width = love.graphics.getWidth()
local height = love.graphics.getHeight()
local function loveupdate()
  local angle = 45
  local speed = 90
  points = calcTrajectoryPoints(speed, angle, 0.4)
  position = (position or 0) + 2
  if position > width-25 then position = 0 end
end

local start = {x = 20, y = height-25}
function lovedraw()
  love.graphics.setColor(0,255,0,255)
  love.graphics.rectangle("fill", position, start.y+-15, 30, 15)
  love.graphics.print("FPS: "..love.timer.getFPS(), 50, height-25)
  love.graphics.setColor(0,0,255,255)
  for i=1, #points do
    local radius = (points[i].vx + points[i].vy) * 0.1
    if (radius < 3) then radius = 3 end
    love.graphics.circle("fill", start.x + points[i].x, start.y - points[i].y, radius)
  end
end

function love.update() pcall(loveupdate) end
function love.draw() pcall(lovedraw) end