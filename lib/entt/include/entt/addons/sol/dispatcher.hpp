#pragma once

#include <entt/signal/dispatcher.hpp>
#include "meta_helper.hpp"

namespace entt_sol
{
	template<typename Event>
	auto connect_listener(entt::dispatcher* dispatcher, const sol::function& f)
	{
		assert(dispatcher && f.valid());

		struct script_listener
		{
			script_listener(entt::dispatcher& dispatcher, const sol::function& f)
				: callback {f}
			{
				connection = dispatcher.sink<Event>().connect<&script_listener::receive>(*this);
				std::cout << "Registered lua listener: " << callback.pointer() << std::endl;
			}
			script_listener(const script_listener&)     = delete;
			script_listener(script_listener&&) noexcept = default;
			~script_listener()
			{
				connection.release();
				std::cout << "Unregistered lua listener: " << callback.pointer() << std::endl;
				callback.abandon();
			}

			script_listener& operator=(const script_listener&)     = delete;
			script_listener& operator=(script_listener&&) noexcept = default;

			void receive(const Event& evt)
			{
				if (connection && callback.valid())
					callback(evt);
			}

			sol::function callback;
			entt::connection connection;
		};

		return std::make_unique<script_listener>(*dispatcher, f);
	}

	template<typename Event>
	void trigger_event(entt::dispatcher* dispatcher, const sol::table& evt)
	{
		assert(dispatcher && evt.valid());
		dispatcher->trigger(evt.as<Event>());
	}

	template<typename Event>
	void enqueue_event(entt::dispatcher* dispatcher, const sol::table& evt)
	{
		assert(dispatcher && evt.valid());
		dispatcher->enqueue(evt.as<Event>());
	}

	template<typename Event>
	void clear_event(entt::dispatcher* dispatcher)
	{
		assert(dispatcher);
		dispatcher->clear<Event>();
	}

	template<typename Event>
	void update_event(entt::dispatcher* dispatcher)
	{
		assert(dispatcher);
		dispatcher->update<Event>();
	}

	template<typename Event>
	void register_meta_event()
	{
		using namespace entt::literals;

		entt::meta<Event>()
			.template func<&connect_listener<Event>>("connect_listener"_hs)
			.template func<&trigger_event<Event>>("trigger_event"_hs)
			.template func<&enqueue_event<Event>>("enqueue_event"_hs)
			.template func<&clear_event<Event>>("clear_event"_hs)
			.template func<&update_event<Event>>("update_event"_hs);
	}

	[[nodiscard]] sol::table open_dispatcher(sol::this_state s)
	{
		// To create a dispatcher in a script: entt.dispatcher.new()

		sol::state_view lua {s};
		auto entt_module = lua["entt"].get_or_create<sol::table>();

		struct base_script_event
		{
			sol::table self;
		};

		lua.new_usertype<base_script_event>("BaseScriptEvent", "type_id", [] {
			return entt::type_hash<base_script_event>::value();
		});

		struct scripted_event_listener
		{
			scripted_event_listener(entt::dispatcher& dispatcher, const sol::table& type, const sol::function& f)
				: key {get_key(type)}
				, callback {f}
			{
				connection = dispatcher.sink<base_script_event>().connect<&scripted_event_listener::receive>(*this);
			}
			scripted_event_listener(const scripted_event_listener&)     = delete;
			scripted_event_listener(scripted_event_listener&&) noexcept = default;
			~scripted_event_listener()
			{
				connection.release();
			}

			scripted_event_listener& operator=(const scripted_event_listener&)     = delete;
			scripted_event_listener& operator=(scripted_event_listener&&) noexcept = default;

			static uintptr_t get_key(const sol::table& t)
			{
				return reinterpret_cast<uintptr_t>(t["__index"].get<sol::table>().pointer());
			}

			void receive(const base_script_event& evt) const
			{
				assert(connection && callback.valid());
				if (auto& [self] = evt; get_key(self) == key)
					callback(self);
			}

			const uintptr_t key;
			const sol::function callback;
			entt::connection connection;
		};

		using namespace entt::literals;

		auto dispatcher_type = entt_module.new_usertype<entt::dispatcher>("dispatcher", sol::meta_function::construct, sol::factories([] {
			return entt::dispatcher {};
		}));

		dispatcher_type["trigger"] = [](entt::dispatcher& self, const sol::table& evt) {
			const auto event_id = get_type_id(evt);
			if (event_id == entt::type_hash<base_script_event>::value())
			{
				self.trigger(base_script_event {evt});
			}
			else
			{
				invoke_meta_func(event_id, "trigger_event"_hs, &self, evt);
			}
		};

		dispatcher_type["enqueue"] = [](entt::dispatcher& self, const sol::table& evt) {
			const auto event_id = get_type_id(evt);
			if (event_id == entt::type_hash<base_script_event>::value())
			{
				self.enqueue(base_script_event {evt});
			}
			else
			{
				invoke_meta_func(event_id, "enqueue_event"_hs, &self, evt);
			}
		};

		dispatcher_type["clear"] = [](entt::dispatcher& self, const sol::object& type_or_id) {
			invoke_meta_func(deduce_type(type_or_id), "clear_event"_hs, &self);
		};

		dispatcher_type["update"] = [](entt::dispatcher& self, const sol::object& type_or_id) {
			invoke_meta_func(deduce_type(type_or_id), "update_event"_hs, &self);
		};

		dispatcher_type["connect"] = [](entt::dispatcher& self, const sol::object& type_or_id, const sol::function& listener, sol::this_state s) {
			if (!listener.valid())
			{
				// TODO: warning message
				return entt::meta_any {};
			}

			const auto event_id = deduce_type(type_or_id);
			if (event_id == entt::type_hash<base_script_event>::value())
			{
				return entt::meta_any {std::make_unique<scripted_event_listener>(self, type_or_id, listener)};
			}
			else
				return invoke_meta_func(event_id, "connect_listener"_hs, &self, listener);
		};

		dispatcher_type["disconnect"] = [](const sol::table& connection) {
			connection.as<entt::meta_any>().reset();
			const_cast<sol::table&>(connection) = sol::nil;
		};

		return entt_module;
	}
} // namespace entt_sol