require "turtle"

for i = 1, 200 do
  crcl(rand(400)-200, rand(400)-200, rand(70)) -- transparent circle
  crcl(rand(400)-200, rand(400)-200, rand(70), ranc())
  oval(rand(400)-200, rand(400)-200, rand(70), rand(70), ranc())
  rect(rand(400)-200, rand(400)-200, rand(50), rand(50)) -- normal
  rect(rand(400)-200, rand(400)-200, rand(50), rand(50), rand(10)) -- rounded
  wait(0.01)
end
wait()