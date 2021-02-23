///
/// World.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_WORLD_HPP_
#define GALAXY_MAP_WORLD_HPP_

#include "galaxy/map/Map.hpp"

namespace galaxy
{
	namespace map
	{
		class World final
		{
		public:
			///
			/// Constructor.
			///
			World() noexcept = default;

			///
			/// Load and Parse constructor.
			///
			/// \param file Tiled world definition in VFS.
			///
			World(std::string_view file);

			///
			/// Destructor.
			///
			~World() noexcept;

			///
			/// Load tiled world from disk.
			///
			/// \param file Tiled world definition in VFS.
			///
			[[maybe_unused]] const bool load(std::string_view file);

			///
			/// Parse out data from world definition.
			///
			[[maybe_unused]] const bool parse();

		private:
			///
			/// Raw json.
			///
			nlohmann::json m_json;

			///
			/// Contains list of loaded maps.
			///
			std::vector<Map> m_maps;
		};
	} // namespace map
} // namespace galaxy

#endif