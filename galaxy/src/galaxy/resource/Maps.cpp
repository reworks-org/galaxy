///
/// Maps.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Maps.hpp"

namespace galaxy
{
	namespace resource
	{
		std::shared_ptr<map::Map> MapLoader::operator()(const std::string& file)
		{
			return std::make_shared<map::Map>(file);
		}
	} // namespace resource
} // namespace galaxy
