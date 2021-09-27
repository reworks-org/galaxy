require "spirograph"

pncl("#DDDDDD")
--    +-------------------------- radius of equator circle
--    |    +--------------------- radius of the bicycle wheel circle
--    |    |    +---------------- (optional) position of the drawing point
--    |    |    |   +------------ (optional) resolution of the graph
--    |    |    |   |    +------- (optional) delay in seconds
--    |    |    |   |    |     +- (optional) show spirograph circles
--    v    v    v   v    v     v
spiro(140, 10.5,20, 720, 0.01, function(cx, cy, x, y)
  pncl("#000000") -- all colors are inverted
  crcl(0, 0, 30, "#0000FF") -- draw sun at the center
  crcl(cx, cy, 12, "#FF00FF") -- draw earth at the earth center
  crcl(x, y, 5, "#888888") -- draw moon at the drawing point
end)
wait()