--[[ Details of the implementation:
  http://giderosmobile.com/forum/discussion/comment/12833
  http://giderosmobile.com/forum/discussion/comment/12680

  [set remote debugging](macro:shell(ide.config.gideros = {remote = "192.168.1.100"}))
  [set local debugging](macro:shell(ide.config.gideros = nil))
--]]

LiveCodeSimple = LiveCodeSimple or Core.class(Sprite)

function rgb(r, g, b) return ((r*256)+g)*256+b end

function LiveCodeSimple:onEnterFrame() end
function LiveCodeSimple:init(x, y)
  application:setBackgroundColor(rgb(220, 220, 255))
  local image=Bitmap.new(Texture.new("zbstudio.png"))
  image:setScale(1.0)
  image:setAnchorPoint(0.5, 0.5)
  image:setPosition(x, y)
  image:setRotation(45)
  self.image=image

  self:addChild(image)
  stage:addChild(self)
end

function Application:clearStage()
  while stage:getNumChildren()>0 do
    stage:removeChildAt(1)
  end
end

application:clearStage()
LiveCodeSimple.new(150, 400)
LiveCodeSimple.new(200, 300)

if initialized then return end
initialized = true
require("mobdebug").start()
stage:addEventListener(Event.ENTER_FRAME,
  function(...) pcall(LiveCodeSimple.onEnterFrame, ...) end)
