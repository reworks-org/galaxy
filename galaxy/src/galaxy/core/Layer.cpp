///
/// Layer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Layer.hpp"

namespace galaxy
{
	namespace core
	{
		void Layer::set_name(std::string_view name) noexcept
		{
			m_name = static_cast<std::string>(name);
		}

		const std::string& Layer::get_name() const noexcept
		{
			return m_name;
		}

		Layer::Layer() noexcept
		    : m_name {"default_layer_name"}, m_active_scene {nullptr}
		{
		}
	} // namespace core
} // namespace galaxy