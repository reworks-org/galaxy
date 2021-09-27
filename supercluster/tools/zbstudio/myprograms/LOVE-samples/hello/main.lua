function love.load(arg)
  if arg[#arg] == "-debug" then require("mobdebug").start() end
end
function love.draw()
  love.graphics.setColor(20,255,0,255)
  love.graphics.print("Hello", 100, 100)
end
