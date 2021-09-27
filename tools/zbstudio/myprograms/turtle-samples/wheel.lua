require "turtle"

updt(false) -- don't refresh screen after each drawing operation
local shot = snap() -- store snapshot
for i=1,24 do
  undo(shot) -- restore snapshot
  for j=1,36 do
    move(200)
    turn(170)
  end
  turn(15)
  updt() -- force update of the screen
  wait(0.1)
end

wait()
