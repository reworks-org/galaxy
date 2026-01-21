print('--- scripted_event.lua ---')

require 'lua.define_event'

function foo_event_handler(evt)
  print('[foo_event_handler] evt.message = ' .. evt.message)
end

Foo = define_event()
Foo.connection = dispatcher:connect(Foo, foo_event_handler)

dispatcher:trigger(Foo({
  message = 'hello event!'
}))
