///
/// SystemFactory.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "SystemFactory.hpp"

namespace galaxy
{
	namespace meta
	{
		void SystemFactory::create_system(const std::string& name, SystemStorage& storage)
		{
			const auto hash = math::fnv1a(name.c_str());
			if (m_factory.contains(hash))
			{
				m_factory[hash](storage);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create system {0} from factory.", name);
			}
		}
	} // namespace meta
} // namespace galaxy
