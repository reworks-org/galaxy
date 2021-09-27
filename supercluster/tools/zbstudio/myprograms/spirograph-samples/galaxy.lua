require "spirograph"

local planets = {
  mercury = {
    calc = function() spiro(40, 10, 0, 170, false, coroutine.yield) end,
    draw = function(cx, cy, x, y) pncl("#000000") crcl(cx, cy, 6, "#666688") end,
  },
  venus = {
    calc = function() spiro(90, 10, 0, 480, false, coroutine.yield) end,
    draw = function(cx, cy, x, y) pncl("#000000") crcl(cx, cy, 8, "#FF0000") end,
  },
  mars = {
    calc = function() spiro(190, 10, 0, 1350, false, coroutine.yield) end,
    draw = function(cx, cy, x, y) pncl("#000000") crcl(cx, cy, 8, "#008888") end,
  },
}
pncl("#DDDDDD") -- draw the trajectory of the moon (in light gray)
--    +-------------------------- radius of equator circle
--    |    +--------------------- radius of the bicycle wheel circle
--    |    |    +---------------- (optional) position of the drawing point
--    |    |    |   +------------ (optional) resolution of the graph
--    |    |    |   |    +------- (optional) delay in seconds
--    |    |    |   |    |     +- (optional) show spirograph circles
--    v    v    v   v    v     v
spiro(140, 10.5,20, 720, 0.02, function(cx, cy, x, y)
  pncl("#000000") -- all colors are inverted; this will be white
  crcl(0, 0, 22, "#0000FF") -- draw sun at the center
  crcl(cx, cy, 10, "#FF00FF") -- draw earth at the earth center
  crcl(x, y, 5, "#888888") -- draw moon at the drawing point

  for _,planet in pairs(planets) do
    local ok, cx, cy, x, y
    while not cx or not cy do
      -- create a coroutine that does planet calculation (no drawing)
      if not planet.coro or coroutine.status(planet.coro) == 'dead' then
        planet.coro = coroutine.create(planet.calc)
      end
      pncl("#222222") -- draw a planet trajectory in light gray (inverted)
      ok, cx, cy, x, y = coroutine.resume(planet.coro, true) -- skip update
      if not ok then error(cx) end -- report any errors in the coroutine
      if cx and cy then planet.draw(cx, cy, x, y) end -- draw what's needed
    end
  end
end)
wait()
