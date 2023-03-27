///
/// Layer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Layer.hpp"

namespace galaxy
{
	namespace scene
	{
		Layer::Layer(std::string_view name, Scene* scene)
			: m_name {name}
			, m_scene {scene}
		{
		}

		Layer::~Layer()
		{
		}

		void Layer::set_name(std::string_view name)
		{
			m_name = static_cast<std::string>(name);
		}

		const std::string& Layer::get_name() const
		{
			return m_name;
		}

		entt::dispatcher& Layer::dispatcher()
		{
			return m_dispatcher;
		}
	} // namespace scene
} // namespace galaxy