local node = {}

function node:init()
  print('node [#' .. self.id() .. '] init()', self)
end

function node:update(dt)
  local transform = self.owner:get(self.id(), Transform)
  transform.x = transform.x + 1
  print('node [#' .. self.id() .. '] update()', transform)
end

function node:destroy()
  print('bye, bye! from: node #' .. self.id())
end

return node
