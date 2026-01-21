print('--- more_events.lua ---')

require 'lua.define_event'

listeners = {
  connections = {},

  receive = function(evt)
    print('[lua/ listeners.receive] TestEvent: ' .. tostring(evt))
  end,
  notify = function(evt)
    print('[lua/ listeners.notify] Bar: ' .. tostring(evt))
  end
}
listeners.connections[0] = dispatcher:connect(TestEvent, listeners.receive)

Bar = define_event()
listeners.connections[1] = dispatcher:connect(Bar, listeners.notify)

dispatcher:trigger(TestEvent('lua', 123))
dispatcher:trigger(Bar())

-- return value (connection) discarded
-- The following callback is going to be disconnected during the garbage collection step
dispatcher:connect(TestEvent, listeners.notify)

-- ...

-- No listener for the following event, nothing will happen
local Baz = define_event()
dispatcher:trigger(Baz({
  err = '404?'
}))
