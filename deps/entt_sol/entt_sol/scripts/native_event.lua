-- TestEvent (struct) defined in c++ and exposed to Lua
print('--- native_event.lua ---')

function test_event_handler(evt)
  print('[lua:test_event_handler] received TestEvent: ' .. tostring(evt))
end

-- A global variable = long-lived connection
conn = dispatcher:connect(TestEvent, test_event_handler)

-- The event is going to be received by listeners defined in either Lua or c++
dispatcher:trigger(TestEvent('lua', 0))
