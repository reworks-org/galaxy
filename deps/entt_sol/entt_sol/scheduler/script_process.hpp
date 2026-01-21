#pragma once

#include <entt/process/process.hpp>
#include <sol/sol.hpp>

#include <chrono>
#include <iostream>

using fsec = std::chrono::duration<float>;

class script_process : public entt::basic_process<fsec> {
public:
  using entt::basic_process<fsec>::basic_process;
  using allocator_type = typename entt::process::allocator_type;

  script_process(const allocator_type &allocator, const sol::table &t,
                 const fsec freq = std::chrono::milliseconds{250})
      : basic_process{allocator}, m_self{t}, m_frequency{freq} {
    m_update = m_self["update"];

#define BIND(func) m_self.set_function(#func, &script_process::func, this)

    BIND(succeed);
    BIND(fail);
    BIND(pause);
    BIND(unpause);

    BIND(abort);
    BIND(alive);
    BIND(finished);
    BIND(paused);
    BIND(rejected);

#undef BIND
  }
  ~script_process() {
    std::cout << "script_process: " << m_self.pointer() << " terminated"
              << std::endl;
    m_self.clear();
    m_self.abandon();
  }

  void init() {
    std::cout << "script_process: " << m_self.pointer() << " joined"
              << std::endl;
    _call("init");
  }

  void update(delta_type dt, void *) {
    if (!m_update.valid()) return fail();

    m_time += dt;
    if (m_time >= m_frequency) {
      m_update(m_self, dt.count());
      m_time = fsec{0};
    }
  }
  void succeeded() { _call("succeeded"); }
  void failed() { _call("failed"); }
  void aborted() { _call("aborted"); }

private:
  void _call(const std::string_view function_name) {
    if (auto &&f = m_self[function_name]; f.valid()) f(m_self);
  }

private:
  sol::table m_self;
  sol::function m_update;

  fsec m_frequency;
  fsec m_time{0};
};
