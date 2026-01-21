#pragma once

#include <sstream>

struct Transform {
  // https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#pointer-stability
  static constexpr auto in_place_delete = true;

  int x, y;

  [[nodiscard]] std::string to_string() const {
    std::stringstream ss;
    ss << "{ x=" << std::to_string(x) << ", y=" << std::to_string(y) << " }";
    return ss.str();
  }
};

void register_transform(sol::state &lua) {
  // clang-format off
  lua.new_usertype<Transform>("Transform",
    "type_id", &entt::type_hash<Transform>::value,

    sol::call_constructor,
    sol::factories([](int x, int y) {
      return Transform{ x, y };
    }),
    "x", &Transform::x,
    "y", &Transform::y,

    sol::meta_function::to_string, &Transform::to_string
  );
  // clang-format on
}
