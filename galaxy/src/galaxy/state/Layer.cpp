///
/// Layer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/Window.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "Layer.hpp"

namespace galaxy
{
	namespace state
	{
		Layer::Layer(std::string_view name, Scene* scene) noexcept
			: m_name {name}
			, m_scene {scene}
		{
			m_window = &core::ServiceLocator<core::Window>::ref();
		}

		Layer::~Layer() noexcept
		{
			m_window = nullptr;
		}

		void Layer::update_rendersystem()
		{
			m_world.update_rendersystem(this);
		}

		void Layer::set_name(std::string_view name) noexcept
		{
			m_name = static_cast<std::string>(name);
		}

		const std::string& Layer::get_name() const noexcept
		{
			return m_name;
		}

		core::World& Layer::world() noexcept
		{
			return m_world;
		}
	} // namespace state
} // namespace galaxy