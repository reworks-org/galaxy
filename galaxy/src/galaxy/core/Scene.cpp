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