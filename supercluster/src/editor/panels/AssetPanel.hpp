///
/// AssetPanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_ASSETPANEL_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_ASSETPANEL_HPP_

#include <ankerl/unordered_dense.h>
#include <galaxy/graphics/gl/Texture2D.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

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

			void render(UpdateStack& updates);

			void top(UpdateStack& updates);
			void tree();
			void body();

		  private:
			void load_lua_script();
			void load_preview();

			// https://gist.github.com/OverShifted/13aec504dfe376dcc2171e8b7451c5b5
			void directory_tree_view_recursive(const std::filesystem::path& path, uint32_t* count);
			void update_directories(const std::filesystem::path& path);

		  private:
			SelectedAsset   m_selected;
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

			graphics::Texture2D m_backward;
			graphics::Texture2D m_file;
			graphics::Texture2D m_folder;
			graphics::Texture2D m_forward;
			graphics::Texture2D m_refresh;
			graphics::Texture2D m_preview;

			graphics::Texture2D* m_icon;

			ankerl::unordered_dense::map<std::string, FileType>                          m_ext_map;
			ankerl::unordered_dense::map<FileType, std::unique_ptr<graphics::Texture2D>> m_tex_map;

			std::vector<std::filesystem::path> m_directories;
		};
	} // namespace panel
} // namespace sc

#endif
