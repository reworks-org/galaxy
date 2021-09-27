if not initialized then
  MOAISim.openWindow("Live coding", 320, 480)
  local viewport = MOAIViewport.new()
  viewport:setSize(320, 480)
  viewport:setScale(320, 480)
  
  local gfxQuad = MOAIGfxQuad2D.new()
  gfxQuad:setTexture("zbstudio.png")
  gfxQuad:setRect(-32, -32, 32, 32)

  prop1 = MOAIProp2D.new()
  prop1:setDeck(gfxQuad)

  prop2 = MOAIProp2D.new()
  prop2:setDeck(gfxQuad)
  
  local layer = MOAILayer2D.new()
  layer:setViewport(viewport)
  layer:insertProp(prop1)
  layer:insertProp(prop2)
  MOAISim.pushRenderPass(layer)

  local thread = MOAICoroutine.new()
  thread:run(function()
      while true do
        update()
        coroutine.yield()
      end
    end)
  initialized = true
end

function update()
  prop1:setLoc(40, 80)
  prop2:setLoc(-40, -120)
  if MOAIGfxDevice.setClearColor then
    MOAIGfxDevice.setClearColor(0.80,0.80,0.90,1)
  else -- MOAI 1.4 changed setClearColor interface
    MOAIGfxDevice.getFrameBuffer():setClearColor(0.80,0.80,0.90,1)
  end
end
