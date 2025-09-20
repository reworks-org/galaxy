///
/// SystemFactory.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "SystemFactory.hpp"

namespace galaxy
{
	void SystemFactory::create_system(const std::string& name, SystemStack& stack)
	{
		const auto hash = math::fnv1a(name.c_str());
		if (m_factory.contains(hash))
		{
			m_factory[hash](stack);
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "System '{0}' is not registered.", name);
		}
	}
} // namespace galaxy
