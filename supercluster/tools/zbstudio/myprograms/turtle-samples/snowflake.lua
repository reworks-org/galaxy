require "turtle"

pncl(colr(127,127,180))

for i=1,24 do
  for j=1,36 do
    move(200)
    turn(170)
  end
  turn(20)
  wait(0.2)
end

save("snowflake")

wait()
