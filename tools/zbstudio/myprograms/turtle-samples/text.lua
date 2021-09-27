require "turtle"

local shot = snap() -- store snapshot
for angle = 0, 720, 6 do
  undo(shot) -- restore saved snapshot to clear screen
  text("some text", -angle, -50, 0)
  text("some text", angle, -50, 0)
  text("more text", -angle)
  text("more text", angle)
  wait(0.01)
end

wait()