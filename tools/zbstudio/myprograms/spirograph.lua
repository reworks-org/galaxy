--
-- Copyright (C) 2012 Paul Kulchenko
-- Epicycloid (spirograph) calculation
-- The algorithm is based on http://www.math.psu.edu/dlittle/java/parametricequations/spirograph/SpiroGraph1.0/index.html
-- and http://en.wikipedia.org/wiki/Spirograph
--

if not debug.getinfo(3) then
  print("This is a module to load with `require('spirograph')`.")
  os.exit(1)
end

require "turtle"

open("Spirograph Window")

local function gcd(x, y) return x % y == 0 and y or gcd(y, x % y) end

function spiro(R, r, p, n, w, show)
  local xp, yp
  local revs = r / gcd(R, r)
  local old = {}
  n = n or 360 -- default resolution
  local auto = updt(false) -- disable auto update
  for count = 0, n * revs do
    local theta = (count == n * revs) and 0 or - (2 * math.pi * count / n)
    local phi = theta * (1 + R / r)
    local x = (R + r) * math.cos(theta) + p * math.cos(phi)
    local y = (R + r) * math.sin(theta) + p * math.sin(phi)
    if xp and yp then
      if show and #old > 0 then circles(show, false, unpack(old)) end
      line(xp, yp, x, y)
      old = {x, y, R, r, p, theta, phi}
      if show then circles(show, true, unpack(old)) end
    end
    xp,yp = x,y
    if not show then updt() end
    if w then wait(w) end
  end
  if show then circles(show, false, unpack(old)) end -- erase circles
  updt(auto)
end

function circles(show, new, x, y, R, r, p, theta, phi)
  local func = logf(wx.wxXOR)
  local width = pnsz(1)
  local color = pncl("#FF00FF")
  local cx = (R + r) * math.cos(theta)
  local cy = (R + r) * math.sin(theta)

  local skipUpdate = false
  if type(show) == 'function' then
    skipUpdate = show(cx, cy, x, y)
  else
    pnsz(2)
    crcl(0, 0, R)
    pncl("#FFFF00")
    crcl(cx, cy, r)
    pncl("#00FFFF")
    crcl(x, y, 4)
    line(cx, cy, x, y)
  end
  if new and not skipUpdate then updt() end

  pncl(color)
  pnsz(width)
  logf(func)
end
