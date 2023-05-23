///
/// AssetPanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_ASSETPANEL_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_ASSETPANEL_HPP_

#include <galaxy/graphics/Texture.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "editor/panels/CodeEditor.hpp"
#include "editor/SelectedAsset.hpp"
#include "editor/UpdateStack.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		// currently doesnt support deleting or selecting folders, or files without extensions.
		class AssetPanel final
		{
		public:
			enum class FileType : int
			{
				AUDIO,
				FONT,
				SHADER,
				JSON,
				LANG,
				LUA,
				PROJ,
				TEXTURE,
				MAP,
				PREFAB
			};

			AssetPanel();
			~AssetPanel() = default;

			void render(CodeEditor& editor, UpdateStack& updates);

			void top(UpdateStack& updates);
			void tree();
			void body(CodeEditor& editor);

		private:
			void load_lua_script(CodeEditor& editor);
			void import_files(const std::string& folder_from_config);

			// https://gist.github.com/OverShifted/13aec504dfe376dcc2171e8b7451c5b5
			void directory_tree_view_recursive(const std::filesystem::path& path, uint32_t* count);
			void update_directories(const std::filesystem::path& path);

		private:
			SelectedAsset m_selected;
			ImGuiTextFilter m_filter;

			ImVec2 m_size_vec;
			ImVec2 m_toolbar_vec;

			float m_padding;
			float m_thumb_size;

			bool m_open_config;
			bool m_open_preview;
			bool m_create_folder_popup;
			bool m_update_directories;
			bool m_contextmenu_opened;

			std::filesystem::path m_root;
			std::filesystem::path m_current_dir;
			std::filesystem::path m_prev_dir;

			std::string m_root_str;

			graphics::Texture m_backward;
			graphics::Texture m_file;
			graphics::Texture m_folder;
			graphics::Texture m_forward;
			graphics::Texture m_reload;

			graphics::Texture* m_icon;

			robin_hood::unordered_map<std::string, FileType> m_ext_map;
			robin_hood::unordered_map<FileType, graphics::Texture> m_tex_map;

			std::vector<std::filesystem::path> m_directories;
		};
	} // namespace panel
} // namespace sc

#endif