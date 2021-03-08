///
/// TiledWorld.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_TILEDWORLD_HPP_
#define GALAXY_MAP_TILEDWORLD_HPP_

#include <robin_hood.h>

#include "galaxy/map/Map.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Shortcut for map storage.
		///
		using MapHolder = robin_hood::unordered_map<std::string, Map>;

		///
		/// Tiled TiledWorld file parser.
		///
		class TiledWorld final
		{
		public:
			///
			/// Constructor.
			///
			TiledWorld() noexcept = default;

			///
			/// Load and Parse constructor.
			///
			/// \param file Tiled TiledWorld definition in VFS.
			///
			TiledWorld(std::string_view file);

			///
			/// Destructor.
			///
			~TiledWorld() noexcept;

			///
			/// Load tiled TiledWorld from disk.
			///
			/// \param file Tiled TiledWorld definition in VFS.
			///
			[[maybe_unused]] const bool load(std::string_view file);

			///
			/// Parse out data from TiledWorld definition.
			///
			[[maybe_unused]] const bool parse();

			///
			/// Get a map by name.
			///
			/// \param map Name of the map to retrieve.
			///
			/// \return Pointer to requested map. nullptr if not found.
			///
			[[nodiscard]] map::Map* get_map(std::string_view map) noexcept;

			///
			/// Retrieve all loaded maps.
			///
			/// \return Const reference to all maps (unordered_flat_map).
			///
			[[nodiscard]] const MapHolder& get_maps() const noexcept;

			///
			/// Retrieve raw json object.
			///
			/// \return Const reference.
			///
			[[nodiscard]] const nlohmann::json& get_raw() const noexcept;

		private:
			///
			/// Raw json.
			///
			nlohmann::json m_json;

			///
			/// Contains list of loaded maps.
			///
			MapHolder m_maps;
		};
	} // namespace map
} // namespace galaxy

#endif