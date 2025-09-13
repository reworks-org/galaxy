///
/// AssetPanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_PANELS_ASSETPANEL_HPP_
#define SUPERCLUSTER_PANELS_ASSETPANEL_HPP_

#include <ankerl/unordered_dense.h>
#include <galaxy/graphics/gl/Texture2D.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "CodeEditor.hpp"

using namespace galaxy;

namespace sc
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

		struct Selected final
		{
			std::string           extension;
			std::filesystem::path path;
			bool                  is_hovered = false;
		};

		AssetPanel();
		~AssetPanel();

		void render(CodeEditor& editor);

		void top();
		void tree();
		void body(CodeEditor& editor);

	private:
		void load_lua_script(CodeEditor& editor);
		void load_preview();

		void directory_tree_view_recursive(const std::filesystem::path& path, uint32_t* count);
		void update_directories(const std::filesystem::path& path);

	public:
		bool m_show = true;

	private:
		Selected        m_selected;
		ImGuiTextFilter m_filter;

		ImVec2 m_size_vec;
		ImVec2 m_toolbar_vec;

		float m_padding;
		float m_thumb_size;

		bool m_open_config;
		bool m_open_preview;
		bool m_show_delete;
		bool m_folder_popup;
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
} // namespace sc

#endif
