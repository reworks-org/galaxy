require "spirograph"

pncl("#F0A0A0")
--    +-------------------------- radius of equator circle
--    |    +--------------------- radius of the bicycle wheel circle
--    |    |    +---------------- (optional) position of the drawing point
--    |    |    |   +------------ (optional) resolution of the graph
--    |    |    |   |    +------- (optional) delay in seconds
--    |    |    |   |    |     +- (optional) show spirograph circles
--    v    v    v   v    v     v
spiro(60,  15,  45, 360, 0.01)
wait()

-- Try varying one of the radiuses to get 3, 5, or 6 leaves
-- Try changing the bicycle wheel radius sign to get different leaf shapes
