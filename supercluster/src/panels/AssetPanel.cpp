///
/// AssetPanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imgui_internal.h>
#include <imgui/imnotify/material_design_icons.h>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/utils/Globals.hpp>

#include "AssetPanel.hpp"

using namespace galaxy;

namespace sc
{
	AssetPanel::AssetPanel()
		: m_toolbar_vec {24, 24}
		, m_icon {nullptr}
		, m_open_config {false}
		, m_open_preview {false}
		, m_folder_popup {false}
		, m_show_delete {false}
		, m_update_directories {true}
		, m_contextmenu_opened {false}
	{
		auto& fs      = core::ServiceLocator<fs::VirtualFileSystem>::ref();
		m_root        = GALAXY_ROOT_DIR / GALAXY_ASSET_DIR;
		m_current_dir = m_root;

		m_root_str = m_root.string();
		m_root_str.pop_back();

		m_thumb_size = 96.0f;
		m_padding    = 48.0f;

		if (m_backward.load("icons/backward.png"))
		{
			m_backward.filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_file.load("icons/file.png"))
		{
			m_file.filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_folder.load("icons/folder.png"))
		{
			m_folder.filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_forward.load("icons/forward.png"))
		{
			m_forward.filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_refresh.load("icons/reload.png"))
		{
			m_refresh.filter(graphics::TextureFilter::TRILINEAR);
		}

		m_ext_map.emplace(".ogg", FileType::AUDIO);
		m_ext_map.emplace(".wav", FileType::AUDIO);
		m_ext_map.emplace(".mp3", FileType::AUDIO);
		m_ext_map.emplace(".ttf", FileType::FONT);
		m_ext_map.emplace(".otf", FileType::FONT);
		m_ext_map.emplace(".glsl", FileType::SHADER);
		m_ext_map.emplace(".json", FileType::JSON);
		m_ext_map.emplace(".lang", FileType::LANG);
		m_ext_map.emplace(".lua", FileType::LUA);
		m_ext_map.emplace(".scproj", FileType::PROJ);
		m_ext_map.emplace(".png", FileType::TEXTURE);
		m_ext_map.emplace(".bmp", FileType::TEXTURE);
		m_ext_map.emplace(".jpg", FileType::TEXTURE);
		m_ext_map.emplace(".jpeg", FileType::TEXTURE);
		m_ext_map.emplace(".ldtk", FileType::MAP);
		m_ext_map.emplace(".galfab", FileType::PREFAB);

		m_tex_map.try_emplace(FileType::AUDIO, std::make_unique<graphics::Texture2D>());
		m_tex_map.try_emplace(FileType::FONT, std::make_unique<graphics::Texture2D>());
		m_tex_map.try_emplace(FileType::SHADER, std::make_unique<graphics::Texture2D>());
		m_tex_map.try_emplace(FileType::JSON, std::make_unique<graphics::Texture2D>());
		m_tex_map.try_emplace(FileType::LANG, std::make_unique<graphics::Texture2D>());
		m_tex_map.try_emplace(FileType::LUA, std::make_unique<graphics::Texture2D>());
		m_tex_map.try_emplace(FileType::PROJ, std::make_unique<graphics::Texture2D>());
		m_tex_map.try_emplace(FileType::TEXTURE, std::make_unique<graphics::Texture2D>());
		m_tex_map.try_emplace(FileType::MAP, std::make_unique<graphics::Texture2D>());
		m_tex_map.try_emplace(FileType::PREFAB, std::make_unique<graphics::Texture2D>());

		if (m_tex_map[FileType::AUDIO]->load("icons/audio.png"))
		{
			m_tex_map[FileType::AUDIO]->filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_tex_map[FileType::FONT]->load("icons/font.png"))
		{
			m_tex_map[FileType::FONT]->filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_tex_map[FileType::SHADER]->load("icons/glsl.png"))
		{
			m_tex_map[FileType::SHADER]->filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_tex_map[FileType::JSON]->load("icons/json.png"))
		{
			m_tex_map[FileType::JSON]->filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_tex_map[FileType::LANG]->load("icons/lang.png"))
		{
			m_tex_map[FileType::LANG]->filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_tex_map[FileType::LUA]->load("icons/lua.png"))
		{
			m_tex_map[FileType::LUA]->filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_tex_map[FileType::PROJ]->load("icons/proj.png"))
		{
			m_tex_map[FileType::PROJ]->filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_tex_map[FileType::TEXTURE]->load("icons/texture.png"))
		{
			m_tex_map[FileType::TEXTURE]->filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_tex_map[FileType::MAP]->load("icons/map.png"))
		{
			m_tex_map[FileType::MAP]->filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_tex_map[FileType::PREFAB]->load("icons/prefab.png"))
		{
			m_tex_map[FileType::PREFAB]->filter(graphics::TextureFilter::TRILINEAR);
		}
	}

	AssetPanel::~AssetPanel()
	{
	}

	void AssetPanel::render(CodeEditor& editor)
	{
		// First update current_dir will always be root.
		if (m_update_directories)
		{
			m_selected           = {};
			m_update_directories = false;
			update_directories(m_current_dir);
		}

		if (ImGui::Begin(ICON_MDI_FOLDER_ARROW_DOWN " Asset Browser", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			top();

			if (ImGui::BeginTable("AssetPanelLayoutTable",
					2,
					ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX,
					ImGui::GetContentRegionAvail()))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				tree();

				ImGui::TableNextColumn();
				body(editor);

				ImGui::EndTable();
			}
		}

		ImGui::End();
	}

	void AssetPanel::top()
	{
		const auto scaled_thumb = (m_thumb_size * ImGui::GetIO().FontGlobalScale) * 0.55f;
		m_size_vec.x            = scaled_thumb;
		m_size_vec.y            = scaled_thumb;

		const auto disable_back = m_current_dir.parent_path().string().find(m_root_str) == std::string::npos;
		if (disable_back)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ui::imgui_imagebutton(m_backward, m_toolbar_vec))
		{
			m_prev_dir           = m_current_dir;
			m_current_dir        = m_current_dir.parent_path();
			m_update_directories = true;
		}

		if (ImGui::BeginItemTooltip())
		{
			ImGui::TextUnformatted("Back");
			ImGui::EndTooltip();
		}

		if (disable_back)
		{
			ImGui::PopStyleVar();
			ImGui::PopItemFlag();
		}

		ImGui::SameLine();

		const auto disable_forward = m_prev_dir.empty();

		if (disable_forward)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ui::imgui_imagebutton(m_forward, m_toolbar_vec))
		{
			m_current_dir = m_prev_dir;
			m_prev_dir.clear();

			m_update_directories = true;
		}

		if (ImGui::BeginItemTooltip())
		{
			ImGui::TextUnformatted("Forward");
			ImGui::EndTooltip();
		}

		if (disable_forward)
		{
			ImGui::PopStyleVar();
			ImGui::PopItemFlag();
		}

		ImGui::SameLine();

		if (ui::imgui_imagebutton(m_refresh, m_toolbar_vec))
		{
			m_update_directories = true;
		}

		if (ImGui::BeginItemTooltip())
		{
			ImGui::TextUnformatted("Refresh");
			ImGui::EndTooltip();
		}

		ImGui::SameLine();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
		m_filter.DrawWithHint("###AssetPanelSearch", ICON_MDI_MAGNIFY "Search...", ImGui::GetContentRegionAvail().x);

		ImGui::Spacing();
		auto str = m_current_dir.string();
		std::replace(str.begin(), str.end(), '\\', '/');
		ImGui::TextWrapped(str.c_str());
		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Spacing();
	}

	void AssetPanel::tree()
	{
		constexpr const ImGuiTableFlags flags =
			ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_ScrollY;

		if (ImGui::BeginTable("AssetPanelTreeViewTable", 1, flags))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0.0f, 0.0f});

			if (ImGui::CollapsingHeader("Assets", ImGuiTreeNodeFlags_SpanAvailWidth))
			{
				auto count = static_cast<unsigned int>(m_directories.size());
				directory_tree_view_recursive(m_root, &count);
			}

			ImGui::PopStyleVar();

			ImGui::EndTable();
		}
	}

	void AssetPanel::body(CodeEditor& editor)
	{
		const auto columns = static_cast<int>((ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize) / (m_size_vec.x + m_padding));

		constexpr const ImGuiTableFlags flags =
			ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoClip | ImGuiTableFlags_ScrollY;

		if (ImGui::BeginTable("AssetPanelColumns", std::max(columns, 1), flags))
		{
			for (const auto& path : m_directories)
			{
				const auto file = path.filename().string();

				if (m_filter.PassFilter(file.c_str()))
				{
					ImGui::TableNextColumn();

					const auto ext = path.extension().string();

					ImGui::PushID(file.c_str());

					if (!path.has_extension())
					{
						m_icon = &m_folder;
					}
					else
					{
						if (m_ext_map.contains(ext))
						{
							m_icon = m_tex_map[m_ext_map[ext]].get();
						}
						else
						{
							m_icon = &m_file;
						}
					}

					if (m_selected.path == path)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, {1, 1, 1, 0.1});
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});
					}

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((ImGui::GetColumnWidth() / 2.0f) - (m_size_vec.x / 2.0f)));
					ui::imgui_imagebutton(*m_icon, m_size_vec);

					ImGui::PopStyleColor();

					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("AssetPanelItem", file.c_str(), file.size(), ImGuiCond_Once);
						ImGui::EndDragDropSource();
					}

					if (!ImGui::IsItemHovered() && !m_contextmenu_opened &&
						(ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
					{
						m_selected = {};
					}

					if (path.has_extension() && ImGui::IsItemHovered() &&
						(ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
					{
						m_selected.path      = path;
						m_selected.extension = ext;
					}

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						if (!path.has_extension())
						{
							m_current_dir        /= file;
							m_update_directories  = true;
						}
						else
						{
							switch (m_ext_map[ext])
							{
								case FileType::LUA:
									load_lua_script(editor);
									break;

								case FileType::TEXTURE:
									load_preview();
									break;
							}
						}
					}

					auto text_x = ImGui::GetCursorPosX() + ((ImGui::GetColumnWidth() / 2.0f) - (ImGui::CalcTextSize(file.c_str()).x / 2.0f));
					if (text_x < ImGui::GetCursorPosX())
					{
						text_x = ImGui::GetCursorPosX();
					}

					ImGui::SetCursorPosX(text_x);
					ImGui::TextUnformatted(file.c_str());

					ImGui::PopID();
				}
			}

			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				ImGui::OpenPopup("AssetPanelContextMenu");
			}

			if (ImGui::BeginPopupContextWindow("AssetPanelContextMenu"))
			{
				m_contextmenu_opened = true;

				if (ImGui::BeginMenu(ICON_MDI_PLUS " Create"))
				{
					if (ImGui::MenuItem(ICON_MDI_FOLDER_PLUS " Folder"))
					{
						m_folder_popup = true;
					}

					if (ImGui::MenuItem(ICON_MDI_LANGUAGE_LUA " Script"))
					{
						auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

						const auto path = std::filesystem::path(m_current_dir / "script.lua");
						if (!fs.write("-- galaxy script (lua)", path.string()))
						{
							ui::imgui_notify_error("Failed to save new script.");
						}

						m_update_directories = true;
					}

					ImGui::EndMenu();
				}

				if (!m_selected.extension.empty())
				{
					switch (m_ext_map[m_selected.extension])
					{
						case FileType::LUA:
							if (ImGui::MenuItem(ICON_MDI_FILE_EDIT " Edit"))
							{
								load_lua_script(editor);
							}
							break;

						case FileType::TEXTURE:
							if (ImGui::MenuItem(ICON_MDI_FILE_SEARCH " Preview"))
							{
								load_preview();
							}
							break;
					}

					if (ImGui::MenuItem(ICON_MDI_DELETE_ALERT " Delete"))
					{
						m_show_delete = true;
					}
				}

				if (ImGui::MenuItem(ICON_MDI_COG " Settings"))
				{
					m_open_config = true;
				}

				ImGui::EndPopup();
			}
			else
			{
				m_contextmenu_opened = false;
			}

			ui::imgui_popup("AssetCreateFolder", m_folder_popup, [&]() {
				static std::string str;
				if (ImGui::InputText("Folder Name", &str, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					if (!str.empty())
					{
						std::filesystem::create_directory(m_current_dir / str);

						str.clear();
						ImGui::CloseCurrentPopup();

						m_update_directories = true;
					}
				}
			});

			ui::imgui_popup("AssetDeleteConfirm", m_show_delete, [&]() {
				ui::imgui_confirm("Create a new project?\nUnsaved progress will be lost.", [&]() {
					std::filesystem::remove(m_selected.path);
				});
			});

			if (m_open_preview)
			{
				ImGui::SetNextWindowSize({256, 256});
			}

			ui::imgui_popup("AssetPreview", m_open_preview, [&]() {
				ui::imgui_image(m_preview, {m_preview.width(), m_preview.height()});
			});

			ui::imgui_popup("AssetConfigPopup", m_open_config, [&]() {
				ImGui::SliderFloat("Thumbnail Size", &m_thumb_size, 16.0f, 128.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::SliderFloat("Padding", &m_padding, 0.0f, 64.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp);
			});

			ImGui::EndTable();
		}
	}

	void AssetPanel::load_lua_script(CodeEditor& editor)
	{
		editor.load(m_selected.path.string());
	}

	void AssetPanel::load_preview()
	{
		m_preview.recreate();

		if (m_preview.load(m_selected.path.string()))
		{
			m_open_preview = true;
		}
	}

	void AssetPanel::directory_tree_view_recursive(const std::filesystem::path& path, uint32_t* count)
	{
		ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			ImGuiTreeNodeFlags node_flags = base_flags;

			std::string name = entry.path().string();

			auto last_slash = name.find_last_of("/\\");
			last_slash      = last_slash == std::string::npos ? 0 : last_slash + 1;
			name            = name.substr(last_slash, name.size() - last_slash);

			const auto is_file = !std::filesystem::is_directory(entry.path());
			if (is_file)
			{
				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			}

			const auto node_open = ImGui::TreeNodeEx((void*)(intptr_t)(*count), node_flags, "");

			if (ImGui::IsItemClicked())
			{
				m_prev_dir           = m_current_dir;
				m_current_dir        = entry.path();
				m_update_directories = true;
			}

			if (!is_file)
			{
				ImGui::SameLine();
				ImGui::TextUnformatted(node_open ? ICON_MDI_FOLDER_OPEN : ICON_MDI_FOLDER);
			}

			ImGui::SameLine();
			ImGui::TextUnformatted(name.data());

			(*count)--;

			if (!is_file)
			{
				if (node_open)
				{
					directory_tree_view_recursive(entry.path(), count);
					ImGui::TreePop();
				}
				else
				{
					for (const auto& e : std::filesystem::recursive_directory_iterator(entry.path()))
					{
						(*count)--;
					}
				}
			}
		}
	}

	void AssetPanel::update_directories(const std::filesystem::path& path)
	{
		m_directories.clear();

		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			m_directories.emplace_back(entry.path());
		}
	}
} // namespace sc
