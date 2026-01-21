#include "bond.hpp"
#include "../common/kbhit.hpp"

#include <iostream>

#define AUTO_ARG(x) decltype(x), x

namespace {

struct TestEvent {
  std::string origin;
  int value{-1};

  [[nodiscard]] std::string to_string() const {
    return "#" + std::to_string(value) + " from " + origin;
  }
};

struct native_listener {
  void receive(const TestEvent &evt) const {
    std::cout << "[c++] received TestEvent: " << evt.to_string() << std::endl;
  }
};

void expose_test_event(sol::state &lua) {
  // clang-format off
  lua.new_usertype<TestEvent>("TestEvent",
    // Make sure that every event you define has 'type_id' getter
    "type_id", &entt::type_hash<TestEvent>::value,

    sol::call_constructor,
    sol::factories([](const char *origin, int value) {
      return TestEvent{origin, value};
    }),
    "origin", &TestEvent::origin,
    "value", &TestEvent::value,

    sol::meta_function::to_string, &TestEvent::to_string
  );
  // clang-format on
}

} // namespace

int main(int argc, char *argv[]) {
#if WIN32 && _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

  try {
    entt::dispatcher dispatcher{};
    native_listener listener{};
    dispatcher.sink<TestEvent>().connect<&native_listener::receive>(listener);

    register_meta_event<TestEvent>();

    sol::state lua{};
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);

    lua.require("dispatcher", sol::c_call<AUTO_ARG(&open_dispatcher)>, false);
    expose_test_event(lua); // Make TestEvent available to Lua

    lua["dispatcher"] =
      std::ref(dispatcher); // Make the dispatcher available to Lua

    lua.do_file("lua/native_event.lua");
    dispatcher.trigger(TestEvent{"c++", 2});

    lua.do_file("lua/scripted_event.lua");

    lua.do_file("lua/more_events.lua");

    lua.collect_garbage();

    lua.script("print('--- main loop ---')");

    lua.script("dispatcher:enqueue(TestEvent('lua', 117))");
    lua.script(
      "dispatcher:enqueue(Foo({ message = 'press any key to exit' }))");
    dispatcher.enqueue(TestEvent{"c++", 10});

    while (true) {
      lua.step_gc(4);

      lua.script("dispatcher:update()");

      if (_kbhit()) break;
    }
  } catch (const std::exception &e) {
    std::cout << "exception: " << e.what();
    return -1;
  }

  return 0;
}
