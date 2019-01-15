///
/// TiledMap.hpp
/// starlight
///
/// Created by reworks on 11/01/2019.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TILEDMAP_HPP_
#define STARLIGHT_TILEDMAP_HPP_

#include "sl/libs/tmx/tmx.h"

namespace sl
{
	///
	/// Wrapper for Tiled's tmx map format, parsed by libtmx.
	/// Supports loading, parsing and rendering (only orthogonal maps).
	/// () has been overloaded to return a pointer to internal tmx map.
	/// During map load, entities are created with rendering components. This means
	/// that each layer requires a "renderLayer" property, for the rendering of the map.
	/// Textures for tiles, images, etc, do not need to be seperate. I.e. The map background texture in Tiled
	/// should use the texture from the TextureAtlas' texture directory.
	///
	class TiledMap
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Don't forget to call load()!
		///
		TiledMap();
		
		///
		/// \brief Map constructor.
		///
		/// Calls load() for you.
		///
		/// \param map .tmx map file in the VFS to load.
		///
		explicit TiledMap(const std::string& map);

		///
		/// \brief Destructor.
		///
		/// Frees internal tmx_map.
		///
		~TiledMap();

		///
		/// Load a tmx map from the VFS.
		///
		/// \param map .tmx map file in the VFS to load.
		///
		/// \return boolean True if load was successful.
		///
		bool load(const std::string& map);

		///
		/// Ease of use function. By overloading operator() it allows easier access to internal map.
		///
		/// \return Pointer to internal tmx_map. Do not free this. Seriously.
		///
		tmx_map* operator()() const;

	private:
		///
		/// Internal map data structure holding parsed map data.
		///
		tmx_map* m_map;

		///
		/// Map data in memory.
		///
		std::string m_buffer;
	};
}

#endif