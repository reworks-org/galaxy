function define_event()
  assert(BaseScriptEvent ~= nil)

  local ScriptEvent = {}
  ScriptEvent.__index = ScriptEvent

  function ScriptEvent:_init(evt)
    for k, v in pairs(evt) do
      self[k] = v
    end
  end

  setmetatable(ScriptEvent, {
    __index = BaseScriptEvent,     -- This is what makes the inheritance work
    __call = function(cls, ...)
      local self = setmetatable({}, cls)
      self:_init(... or {})
      return self
    end
  })
  assert(ScriptEvent.type_id() == BaseScriptEvent.type_id())
  return ScriptEvent
end
