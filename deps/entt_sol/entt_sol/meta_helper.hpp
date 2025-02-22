#pragma once

#include <entt/meta/factory.hpp>
#include <entt/meta/resolve.hpp>
#include <sol/sol.hpp>

namespace entt_sol
{
	[[nodiscard]] inline entt::id_type get_type_id(const sol::table& obj)
	{
		const auto f = obj["type_id"].get<sol::function>();
		return f.valid() ? f().get<entt::id_type>() : -1;
	}

	template<typename T>
	[[nodiscard]] inline entt::id_type deduce_type(T&& obj)
	{
		switch (obj.get_type())
		{
			// in lua: registry:has(e, Transform.type_id())
			case sol::type::number:
				return obj.template as<entt::id_type>();
			// in lua: registry:has(e, Transform)
			case sol::type::table:
				return get_type_id(obj);
		}

		return -1;
	}

	template<typename... Args>
	inline auto invoke_meta_func(entt::meta_type meta_type, entt::id_type function_id, Args&&... args)
	{
		if (meta_type)
		{
			if (auto&& meta_function = meta_type.func(function_id); meta_function)
				return meta_function.invoke({}, std::forward<Args>(args)...);
		}

		return entt::meta_any {};
	}

	template<typename... Args>
	inline auto invoke_meta_func(entt::id_type type_id, entt::id_type function_id, Args&&... args)
	{
		return invoke_meta_func(entt::resolve(type_id), function_id, std::forward<Args>(args)...);
	}
} // namespace entt_sol
