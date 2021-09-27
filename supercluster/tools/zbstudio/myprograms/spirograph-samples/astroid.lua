require "spirograph"

pncl("#A0A0A0")
--    +-------------------------- radius of equator circle
--    |    +--------------------- radius of the bicycle wheel circle
--    |    |    +---------------- (optional) position of the drawing point
--    |    |    |   +------------ (optional) resolution of the graph
--    |    |    |   |    +------- (optional) delay in seconds
--    |    |    |   |    |     +- (optional) show spirograph circles
--    v    v    v   v    v     v
spiro(60, -15,  15, 360, 0.01, true)
wait()

-- Try changing position to get the same picture rotated 45 degrees
-- Vary the radius and try to get 3,5 or 6 cusps
-- How can you turn it into a square?
