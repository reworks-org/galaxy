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
		void Layer::set_name(std::string_view name)
		{
			m_name = static_cast<std::string>(name);
		}

		const std::string& Layer::get_name()
		{
			return m_name;
		}

		Layer::Layer()
		    : m_name {"default"}
		{
		}

		Layer::Layer(std::string_view name)
		{
			set_name(name);
		}
	} // namespace core
} // namespace galaxy