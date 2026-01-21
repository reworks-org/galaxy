local exports = {}

local process = {
  name = '',
  count = 0,
  iterations = 5
}

function process:init()
  print('[lua] ' .. self.name .. ': init()')
end

function process:update(dt)
  print('[lua] ' .. self.name .. ': update(' .. string.format("%.2f", dt) .. ')', self.count)
  self.count = self.count + 1
  if (self.count >= self.iterations) then
    self.succeed()
  end
end

function process:succeeded()
  print('[lua] ' .. self.name .. ': succeeded()')
end

function process:failed()
  print('[lua] ' .. self.name .. ': failed()')
end

function process:aborted()
  print('[lua] ' .. self.name .. ': aborted()')
end

function exports.new(name, iterations)
  local self = {}
  self.name = name or 'unknown'
  self.iterations = iterations or 5
  setmetatable(self, {
    __index = process
  })
  return self
end

return exports
