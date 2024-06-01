#pragma once

#include <chrono>
#include <entt/entt.hpp>
#include <sol/sol.hpp>

namespace entt_sol
{
	using fsec      = std::chrono::duration<float>;
	using scheduler = entt::basic_scheduler<fsec>;

	class script_process : public entt::process<script_process, fsec>
	{
	  public:
		script_process(const sol::table& t, const fsec freq = std::chrono::milliseconds {250})
			: m_self {t}
			, m_update {m_self["update"]}
			, m_frequency {freq}
		{
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

		~script_process()
		{
			m_self.clear();
			m_self.abandon();
		}

		void init()
		{
			_call("init");
		}

		void update(fsec dt, void*)
		{
			if (!m_update.valid())
				return fail();

			m_time += dt;
			if (m_time >= m_frequency)
			{
				m_update(m_self, dt.count());
				m_time = fsec {0};
			}
		}

		void succeeded()
		{
			_call("succeeded");
		}

		void failed()
		{
			_call("failed");
		}

		void aborted()
		{
			_call("aborted");
		}

	  private:
		void _call(const std::string_view function_name)
		{
			if (auto&& f = m_self[function_name]; f.valid())
				f(m_self);
		}

	  private:
		sol::table    m_self;
		sol::function m_update;

		fsec m_frequency;
		fsec m_time {0};
	};

	[[nodiscard]] inline sol::table open_scheduler(sol::this_state s)
	{
		// To create a scheduler inside a script: entt.scheduler.new()

		sol::state_view lua {s};
		auto            entt_module = lua["entt"].get_or_create<sol::table>();

		entt_module.new_usertype<scheduler>(
			"scheduler",
			sol::meta_function::construct,
			sol::factories([] {
				return scheduler {};
			}),
			"size",
			&scheduler::size,
			"empty",
			&scheduler::empty,
			"clear",
			&scheduler::clear,
			"attach",
			[](scheduler& self, const sol::table& process, const sol::variadic_args& va) {
				// TODO: validate process before attach?
				auto& continuator = self.template attach<script_process>(process);
				for (sol::table child_process : va)
				{
					continuator.template then<script_process>(std::move(child_process));
				}
			},
			"update",
			sol::resolve<void(fsec, void*)>(&scheduler::update),
			"abort",
			sol::overload(
				[](scheduler& self) {
					self.abort();
				},
				sol::resolve<void(bool)>(&scheduler::abort)));

		return entt_module;
	}
} // namespace entt_sol
