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

#include "editor/CodeEditor.hpp"
#include "editor/SelectedAsset.hpp"
#include "editor/UpdateStack.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		class AssetPanel final
		{
		public:
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

		private:
			SelectedAsset m_selected;
			ImGuiTextFilter m_filter;

			ImVec2 m_size_vec;
			ImVec2 m_toolbar_vec;

			float m_padding;
			float m_thumb_size;

			bool m_open_config;
			bool m_create_folder_popup;

			std::filesystem::path m_root;
			std::filesystem::path m_current_dir;
			std::filesystem::path m_prev_dir;

			graphics::Texture m_audio;
			graphics::Texture m_backward;
			graphics::Texture m_file;
			graphics::Texture m_folder;
			graphics::Texture m_font;
			graphics::Texture m_forward;
			graphics::Texture m_glsl;
			graphics::Texture m_json;
			graphics::Texture m_lang;
			graphics::Texture m_lua;
			graphics::Texture m_proj;
			graphics::Texture m_texture;
			graphics::Texture m_reload;
			graphics::Texture m_map;
			graphics::Texture m_prefab;

			graphics::Texture* m_icon;
		};
	} // namespace panel
} // namespace sc

#endif