# EnTT meets Sol2

[![CodeFactor Grade](https://img.shields.io/codefactor/grade/github/skaarj1989/entt-meets-sol2)](https://www.codefactor.io/repository/github/skaarj1989/entt-meets-sol2)
![GitHub](https://img.shields.io/github/license/skaarj1989/entt-meets-sol2.svg)

## Requirements

- EnTT v.3.16.0 (latest)
- sol2 v3.5.0 (latest)

## Building

```bash
> git clone https://github.com/skaarj1989/entt-meets-sol2.git
> cd entt-meets-sol2
> cmake -S . -B build
```

### vcpkg quickstart

https://github.com/microsoft/vcpkg#getting-started

```bash
> git clone https://github.com/microsoft/vcpkg
> ./vcpkg/bootstrap-vcpkg.bat
```

Add the following environment variables

```bash
VCPKG_ROOT=path_to_vcpkg
VCPKG_DEFAULT_TRIPLET=x64-windows
```

Install required dependencies

```bash
> ./vcpkg install entt sol2 lua
```

## Registry

[entt/wiki/registry](https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#the-registry-the-entity-and-the-component)

### Goal

- Expose `registry` to Lua scripts:
  - Manage entity lifetime
  - Set components
  - Get component by reference
  - `runtime_view`
  - Some of **MonoBehaviour** functionality

### c++ setup

[examples/registry](https://github.com/skaarj1989/entt-meets-sol2/blob/main/examples/registry/)

```cpp
struct Transform { int x, y; };

register_meta_component<Transform>();

sol::state lua{};
lua.require("dispatcher" ...);

lua.new_usertype<Transform>("Transform",
  "type_id", &entt::type_hash<Transform>::value,
  sol::call_constructor,
  sol::factories([](int x, int y) {
    return Transform{ x, y };
  }),
  "x", &Transform::x,
  "y", &Transform::y
);
```

```cpp
entt::registry registry{};
lua["registry"] = std::ref(registry); // Make the registry available to Lua
```

### Lua script

```lua
-- Create a registry inside a script
registry = entt.registry.new()
```

```lua
mario = registry:create()
registry:emplace(mario, Transform(1, 2))
material = registry:get(mario, Material)

if (registry:has(mario, Transform)) then
  registry:emplace(mario, DeletionFlag())
end
```

```lua
-- Utilizes variadic args - pass as many types as you want
registry:runtime_view(Transform, DeletionFlag):each(
  function(entity)
    registry:remove(entity, DeletionFlag)
  end
)
```

Want something like **MonoBehaviour** in Unity?
[examples/system](https://github.com/skaarj1989/entt-meets-sol2/tree/main/examples/system)

### Lua script

```lua
local node = {}
function node:init()
  self.owner:emplace(self.id(), Transform(5, 9))
end
function node:update(dt)
  local transform = self.owner:get(self.id(), Transform)
  transform.x = transform.x + 1
end
function node:destroy()
  -- ...
end

return node
```

### c++ setup

```cpp
struct ScriptComponent {
  sol::table self;
  struct {
    sol::function update;
  } hooks;
};

registry.emplace<ScriptComponent>(entity, lua.script_file("behavior.lua"));

while (true) {
  auto view = registry.view<ScriptComponent>();
  for (auto entity : view) {
    auto &script = view.get<ScriptComponent>(entity);
    script.hooks.update(script.self, delta_time);
  }
}
```

## Event dispatcher

[entt/wiki/dispatcher](https://github.com/skypjack/entt/wiki/Crash-Course:-events,-signals-and-everything-in-between#event-dispatcher)

### Goal

- Either create new (inside script) or connect existing (native c++) dispatcher
- Trigger/enqueue events from Lua side
- Listen for events in Lua scripts
- Support disconnection of listeners

### c++ setup

[examples/dispatcher](https://github.com/skaarj1989/entt-meets-sol2/tree/main/examples/dispatcher)

```cpp
struct an_event { int value; };

register_meta_event<an_event>();

sol::state lua{}
lua.require("dispatcher", ...)

lua.new_usertype<an_event>("an_event",
  "type_id", &entt::type_hash<an_event>::value,
  sol::call_constructor,
  sol::factories([](int value) {
    return an_event{ value };
  }),
  "value", &an_event::value
);
```

```cpp
entt::dispatcher dispatcher{};
lua["dispatcher"] = std::ref(dispatcher); // Make the dispatcher available to Lua

// load a script ...

dispatcher.update(); // inside loop
```

### Lua script

```lua
-- Create a dispatcher in a script
dispatcher = entt.dispatcher.new()
```

```lua
listener = {}
function listener.receive(evt)
    -- ...
end
function listener.method(evt)
    -- ...
end

-- You have to store result of 'connect()', otherwise listener will be
-- disconnected in lua garbage collection step.
conn = dispatcher:connect(an_event, listener.receive)
dispatcher:connect(another_event, listener.method)

-- Event type is deduced from bound 'type_id()' method
dispatcher:trigger(an_event(42))
dispatcher:enqueue(another_event())

conn = nil -- to disconnect listener
```

## Cooperative scheduler

[entt/wiki/cooperative-scheduler](https://github.com/skypjack/entt/wiki/Crash-Course:-cooperative-scheduler)

### Goal

- Define process class in Lua and attach it to `scheduler` either native c++ or dedicated to script
- Allow process chaining

### c++ setup

[examples/scheduler](https://github.com/skaarj1989/entt-meets-sol2/tree/main/examples/scheduler)

```cpp
sol::state lua{};
lua.require("scheduler", ...);
```

```cpp
entt::scheduler scheduler{};
lua["scheduler"] = std::ref(scheduler); // Make the scheduler available to Lua

scheduler.update(dt); // inside loop
```

### Lua script

```lua
-- Create a scheduler in a script
scheduler = entt.scheduler.new()
```

```lua
local process_a = {}
function process:update(dt)
    -- ...
end
-- define init, succeeded and other methods ...

-- This one differs from entt attach().then().then ... but works the same
scheduler:attach(
    process_a,
    process_b,
    process_n
    -- etc ...
)
```

## License

Code released under [CC0 1.0 Universal](LICENSE)
