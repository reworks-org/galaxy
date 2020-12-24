///
/// MapEditor.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAPEDITOR_HPP_
#define GALAXY_MAPEDITOR_HPP_

#include <starmap/Map.hpp>

#include "galaxy/tools/JsonEditor.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	class MapEditor final
	{
	public:
		MapEditor();
		~MapEditor() = default;

		void draw();

	private:
		starmap::Map m_map;
		JsonEditor m_json_editor;
	};

} // namespace galaxy

#endif