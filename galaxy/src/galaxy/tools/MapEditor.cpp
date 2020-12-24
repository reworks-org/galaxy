///
/// MapEditor.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/fs/FileSystem.hpp>

#include "MapEditor.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	MapEditor::MapEditor()
	{
		m_map.load(galaxy::FileSystem::open_file_dialog(".json"));
		m_map.parse();

		m_json_editor.load_json(&m_map.raw_json());
	}

	void MapEditor::draw()
	{
		m_json_editor.parse_and_display();
		m_map.parse();
	}
} // namespace galaxy