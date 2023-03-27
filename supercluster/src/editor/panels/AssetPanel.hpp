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

		private:
			void load_lua_script(CodeEditor& editor);
			void import_files(const std::string& folder_from_config);

		private:
			SelectedAsset m_selected;

			ImVec2 m_size_vec;
			ImVec2 m_toolbar_vec;

			float m_padding;
			float m_thumb_size;

			std::filesystem::path m_root;
			std::filesystem::path m_current_dir;
			std::filesystem::path m_prev_dir;

			std::string m_search_term;

			graphics::Texture m_audio;
			graphics::Texture m_backward;
			graphics::Texture m_cog;
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