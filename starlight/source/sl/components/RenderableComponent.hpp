///
/// RenderableComponent.hpp
/// starlight
///
///  Created by reworks on 12/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RENDERABLECOMPONENT_HPP_
#define STARLIGHT_RENDERABLECOMPONENT_HPP_

#include "sl/core/World.hpp"
#include "sl/utils/Utils.hpp"
#include "sol2/sol_forward.hpp"
#include "entt/entity/registry.hpp"
#include "entt/core/hashed_string.hpp"

namespace sl
{
	class RenderableComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		/// 
		RenderableComponent(const sol::table& table);

		///
		/// Move Constructor.
		///
		RenderableComponent(RenderableComponent&&) = default;

		///
		/// Destructor.
		///
		~RenderableComponent() = default;

		template<typename... Renderables>
		void attachRenderables(entt::Entity e);


	private:
		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

		///
		/// Default constructor.
		/// Deleted.
		///
		RenderableComponent() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		RenderableComponent(const RenderableComponent&) = delete;
	};

	template<typename... Renderables>
	void RenderableComponent::attachRenderables(entt::Entity e)
	{
		auto tuple = entt::getMultiComponents<Renderables...>(World::get()->m_registery, e);
		utils::for_each_in_tuple(tuple, [this](auto& elem)
		{
			if (elem)
			{

			}
		});
	}
}

#endif