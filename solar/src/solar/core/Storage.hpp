///
/// Storage.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_STORAGE_HPP_
#define SOLAR_STORAGE_HPP_

#include <vector>

namespace sr
{
	struct BaseStorage{};

	template<typename Component>
	struct ComponentStorage : public BaseStorage
	{
		std::vector<Component> m_components;
	};
}

#endif