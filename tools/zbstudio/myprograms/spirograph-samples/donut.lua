require "spirograph"

pncl(ranc())
--    +-------------------------- radius of equator circle
--    |    +--------------------- radius of the bicycle wheel circle
--    |    |    +---------------- (optional) position of the drawing point
--    |    |    |   +------------ (optional) resolution of the graph
--    |    |    |   |    +------- (optional) delay in seconds
--    |    |    |   |    |     +- (optional) show spirograph circles
--    v    v    v   v    v     v
spiro(125, 23, 53, 360, 0.001)
wait()

-- Try varying the equator circle radius to make a smaller donut
-- Try changing the bicycle wheel radius to positive numbers
