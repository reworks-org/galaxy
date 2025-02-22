#pragma once

#include <entt/entity/registry.hpp>
#include <entt/entity/runtime_view.hpp>
#include <set>

#include "meta_helper.hpp"

namespace entt_sol
{
	template<typename Component>
	inline auto is_valid(const entt::registry* registry, entt::entity entity)
	{
		return registry->valid(entity);
	}

	template<typename Component>
	inline auto emplace_component(entt::registry* registry, entt::entity entity, const sol::table& instance, sol::this_state s)
	{
		auto& comp = registry->emplace_or_replace<Component>(entity, instance.valid() ? std::move(instance.as<Component&&>()) : Component {});
		return sol::make_reference(s, std::ref(comp));
	}

	template<typename Component>
	inline auto get_component(entt::registry* registry, entt::entity entity, sol::this_state s)
	{
		auto& comp = registry->get_or_emplace<Component>(entity);
		return sol::make_reference(s, std::ref(comp));
	}

	template<typename Component>
	inline bool has_component(entt::registry* registry, entt::entity entity)
	{
		return registry->any_of<Component>(entity);
	}

	template<typename Component>
	inline auto remove_component(entt::registry* registry, entt::entity entity)
	{
		return registry->remove<Component>(entity);
	}

	template<typename Component>
	inline void clear_component(entt::registry* registry)
	{
		registry->clear<Component>();
	}

	template<typename Component>
	inline void register_meta_component()
	{
		using namespace entt::literals;

		entt::meta<Component>()
			.template func<&is_valid<Component>>("valid"_hs)
			.template func<&emplace_component<Component>>("emplace"_hs)
			.template func<&get_component<Component>>("get"_hs)
			.template func<&has_component<Component>>("has"_hs)
			.template func<&clear_component<Component>>("clear"_hs)
			.template func<&remove_component<Component>>("remove"_hs);
	}

	inline auto collect_types(const sol::variadic_args& va)
	{
		std::set<entt::id_type> types;
		std::transform(va.cbegin(), va.cend(), std::inserter(types, types.begin()), [](const auto& obj) {
			return deduce_type(obj);
		});
		return types;
	}

	inline sol::table open_registry(sol::this_state s)
	{
		// To create a registry inside a script: entt.registry.new()

		sol::state_view lua {s};
		auto            entt_module = lua["entt"].get_or_create<sol::table>();

		entt_module.new_usertype<entt::runtime_view>("runtime_view",
			sol::no_constructor,

			"size_hint",
			&entt::runtime_view::size_hint,
			"contains",
			&entt::runtime_view::contains,
			"each",
			[](const entt::runtime_view& self, const sol::function& callback) {
				if (callback.valid())
				{
					for (auto entity : self)
						callback(entity);
				}
			});

		using namespace entt::literals;

		entt_module.new_usertype<entt::registry>(
			"registry",
			sol::meta_function::construct,
			sol::factories([] {
				return entt::registry {};
			}),
			"size",
			[](const entt::registry& self) {
				return self.storage<entt::entity>()->size();
			},
			"alive",
			[](const entt::registry& self) {
				return self.storage<entt::entity>()->free_list();
			},
			"valid",
			&entt::registry::valid,
			"current",
			&entt::registry::current,
			"create",
			[](entt::registry& self) {
				return self.create();
			},
			"destroy",
			[](entt::registry& self, entt::entity entity) {
				return self.destroy(entity);
			},
			"emplace",
			[](entt::registry& self, entt::entity entity, const sol::table& comp, sol::this_state s) -> sol::object {
				if (!comp.valid())
					return sol::lua_nil_t {};
				const auto maybe_any = invoke_meta_func(get_type_id(comp), "emplace"_hs, &self, entity, comp, s);
				return maybe_any ? maybe_any.cast<sol::reference>() : sol::lua_nil_t {};
			},
			"remove",
			[](entt::registry& self, entt::entity entity, const sol::object& type_or_id) {
				const auto maybe_any = invoke_meta_func(deduce_type(type_or_id), "remove"_hs, &self, entity);
				return maybe_any ? maybe_any.cast<size_t>() : 0;
			},
			"has",
			[](entt::registry& self, entt::entity entity, const sol::object& type_or_id) {
				const auto maybe_any = invoke_meta_func(deduce_type(type_or_id), "has"_hs, &self, entity);
				return maybe_any ? maybe_any.cast<bool>() : false;
			},
			"any_of",
			[](const sol::table& self, entt::entity entity, const sol::variadic_args& va) {
				const auto types = collect_types(va);
				const auto has   = self["has"].get<sol::function>();
				return std::any_of(types.cbegin(), types.cend(), [&](auto type_id) {
					return has(self, entity, type_id).template get<bool>();
				});
			},
			"get",
			[](entt::registry& self, entt::entity entity, const sol::object& type_or_id, sol::this_state s) {
				const auto maybe_any = invoke_meta_func(deduce_type(type_or_id), "get"_hs, &self, entity, s);
				return maybe_any ? maybe_any.cast<sol::reference>() : sol::lua_nil_t {};
			},
			"clear",
			sol::overload(&entt::registry::clear<>,
				[](entt::registry& self, sol::object type_or_id) {
					invoke_meta_func(deduce_type(type_or_id), "clear"_hs, &self);
				}),

			"orphan",
			&entt::registry::orphan,

			"runtime_view",
			[](entt::registry& self, const sol::variadic_args& va) {
				const auto types = collect_types(va);

				auto view = entt::runtime_view {};
				for (auto&& [componentId, storage] : self.storage())
				{
					if (types.find(componentId) != types.cend())
					{
						view.iterate(storage);
					}
				}
				return view;
			});

		return entt_module;
	}
} // namespace entt_sol
