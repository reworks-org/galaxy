#include "bond.hpp"
#include "../common/transform.hpp"

#include <iostream>

#define AUTO_ARG(x) decltype(x), x

int main(int argc, char *argv[]) {
#if WIN32 && _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

  try {
    register_meta_component<Transform>();

    sol::state lua{};
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);
    lua.require("registry", sol::c_call<AUTO_ARG(&open_registry)>, false);
    register_transform(lua); // Make Transform struct available to Lua

    entt::registry registry{};
    lua["registry"] = std::ref(registry); // Make the registry available to Lua

    lua.do_file("lua/registry_simple.lua");

    const auto bowser = lua["bowser"].get<entt::entity>();
    const auto *xf = registry.try_get<Transform>(bowser);
    assert(xf != nullptr);
    const Transform &transform = lua["transform"];
    assert(xf->x == transform.x && xf->y == transform.y);

    lua.do_file("lua/iterate_entities.lua");
    assert(registry.orphan(bowser) && "The only component (Transform) should  "
                                      "be removed by the script");
  } catch (const std::exception &e) {
    std::cout << "exception: " << e.what();
    return -1;
  }

  return 0;
}
