require "spirograph"

pncl("#A0A0A0")
--    +-------------------------- radius of equator circle
--    |    +--------------------- radius of the bicycle wheel circle
--    |    |    +---------------- (optional) position of the drawing point
--    |    |    |   +------------ (optional) resolution of the graph
--    |    |    |   |    +------- (optional) delay in seconds
--    |    |    |   |    |     +- (optional) show spirograph circles
--    v    v    v   v    v     v
spiro(101, 47,  60,  8, 0.01)
wait()

-- Try varying the resolution to change the number of pearls
-- Try changing the bicycle wheel radius to get more or less dense pearls