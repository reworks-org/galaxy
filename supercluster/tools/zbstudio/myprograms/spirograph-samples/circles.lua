require "spirograph"

pncl("#A0A0A0")
--    +-------------------------- radius of equator circle
--    |    +--------------------- radius of the bicycle wheel circle
--    |    |    +---------------- (optional) position of the drawing point
--    |    |    |   +------------ (optional) resolution of the graph
--    |    |    |   |    +------- (optional) delay in seconds
--    |    |    |   |    |     +- (optional) show spirograph circles
--    v    v    v   v    v     v
spiro(90,   1, 105, 360, 0.01)
wait()

-- Try changing the radius of the equator circle slightly, between 88 and 92
-- Also try real numbers; for example, try 91.5 or 87.5
