require "spirograph"

pncl(ranc())
--    +-------------------------- radius of equator circle
--    |    +--------------------- radius of the bicycle wheel circle
--    |    |    +---------------- (optional) position of the drawing point
--    |    |    |   +------------ (optional) resolution of the graph
--    |    |    |   |    +------- (optional) delay in seconds
--    |    |    |   |    |     +- (optional) show spirograph circles
--    v    v    v   v    v     v
spiro(60,  59,  90,  11, 0.01)
wait()

-- Try varying the resolution
