///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Scene.hpp"

namespace galaxy
{
	namespace core
	{
		Scene::Scene(std::string_view name) noexcept
		    : m_name {name}
		{
		}

		const std::string& Scene::get_name() const noexcept
		{
			return m_name;
		}

		World& Scene::world() noexcept
		{
			return m_world;
		}

		graphics::Camera& Scene::camera() noexcept
		{
			return m_camera;
		}

	} // namespace core
} // namespace galaxy