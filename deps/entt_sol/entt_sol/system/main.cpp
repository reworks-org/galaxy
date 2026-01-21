#include "../common/kbhit.hpp"

#include "../registry/bond.hpp"
#include "../common/transform.hpp"

#include <thread>
#include <chrono>
#include <iostream>

#define AUTO_ARG(x) decltype(x), x

using fsec = std::chrono::duration<float>;

namespace {

struct ScriptComponent {
  sol::table self;
  struct {
    sol::function update;
  } hooks;
};

void inspect_script(const ScriptComponent &script) {
  script.self.for_each([](const sol::object &key, const sol::object &value) {
    std::cout << key.as<std::string>() << ": "
              << sol::type_name(value.lua_state(), value.get_type())
              << std::endl;
  });
}

void init_script(entt::registry &registry, entt::entity entity) {
  auto &script = registry.get<ScriptComponent>(entity);
  assert(script.self.valid());
  script.hooks.update = script.self["update"];
  assert(script.hooks.update.valid());

  script.self["id"] = sol::readonly_property([entity] { return entity; });
  script.self["owner"] = std::ref(registry);
  if (auto &&f = script.self["init"]; f.valid()) f(script.self);
  // inspect_script(script);
}
void release_script(entt::registry &registry, entt::entity entity) {
  auto &script = registry.get<ScriptComponent>(entity);
  if (auto &&f = script.self["destroy"]; f.valid()) f(script.self);
  script.self.abandon();
}

void script_system_update(entt::registry &registry, fsec delta_time) {
  auto view = registry.view<ScriptComponent>();
  for (auto entity : view) {
    auto &script = view.get<ScriptComponent>(entity);
    assert(script.self.valid());
    script.hooks.update(script.self, delta_time);
  }
}

} // namespace

int main(int argc, char *argv[]) {
#if WIN32 && _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

  try {
    register_meta_component<Transform>();

    entt::registry registry{};
    registry.on_construct<ScriptComponent>().connect<&init_script>();
    registry.on_destroy<ScriptComponent>().connect<&release_script>();

    sol::state lua{};
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);
    lua.require("registry", sol::c_call<AUTO_ARG(&open_registry)>, false);
    register_transform(lua); // Make Transform struct available to Lua

    auto behavior_script = lua.load_file("lua/behavior_script.lua");
    assert(behavior_script.valid());

    for (int i = 0; i < 5; ++i) {
      auto e = registry.create();
      registry.emplace<Transform>(e, Transform{i, i});
      registry.emplace<ScriptComponent>(e, behavior_script.call());
    }

    using namespace std::chrono_literals;

    constexpr auto target_frame_time = 500ms;
    fsec delta_time{target_frame_time};

    while (true) {
      using clock = std::chrono::high_resolution_clock;
      const auto begin_ticks = clock::now();

      script_system_update(registry, delta_time);
      std::this_thread::sleep_for(target_frame_time);

      delta_time = std::chrono::duration_cast<fsec>(clock::now() - begin_ticks);
      if (delta_time > 1s) delta_time = target_frame_time;

      if (_kbhit()) break;
    }
    registry.clear();
  } catch (const std::exception &e) {
    std::cout << "exception: " << e.what();
    return -1;
  }

  return 0;
}
