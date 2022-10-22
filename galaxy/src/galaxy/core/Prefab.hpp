///
/// Prefab.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_PREFAB_HPP_
#define GALAXY_CORE_PREFAB_HPP_

#include <nlohmann/json.hpp>

namespace galaxy
{
	namespace core
	{
		///
		/// Custom entity definition that can be loaded at any time.
		///
		struct Prefab final
		{
			///
			/// Required json data to construct entity from.
			///
			nlohmann::json m_data;
		};
	} // namespace core
} // namespace galaxy

#endif