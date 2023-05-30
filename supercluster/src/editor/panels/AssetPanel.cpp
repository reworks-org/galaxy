///
/// AssetPanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui_internal.h>
#include <imgui_addons/material_design_icons.h>

#include <galaxy/core/Config.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/resource/Fonts.hpp>
#include <galaxy/resource/Language.hpp>
#include <galaxy/resource/Scripts.hpp>
#include <galaxy/resource/Shaders.hpp>
#include <galaxy/resource/Sounds.hpp>
#include <galaxy/resource/TextureAtlas.hpp>

#include "AssetPanel.hpp"

#define BIT(x) (1 << x)

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		AssetPanel::AssetPanel()
			: m_toolbar_vec {24, 24}
			, m_icon {nullptr}
			, m_open_config {false}
			, m_open_preview {false}
			, m_create_folder_popup {false}
			, m_update_directories {true}
			, m_contextmenu_opened {false}
		{
			auto& fs      = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			m_root        = fs.root_path();
			m_current_dir = fs.root_path();

			m_root_str = m_root.string();
			m_root_str.pop_back();

			auto& config = core::ServiceLocator<core::Config>::ref();
			config.restore("padding", 48.0f, "editor");
			config.restore("thumb_size", 96.0f, "editor");
			config.save();

			m_thumb_size = config.get<float>("thumb_size", "editor");
			m_padding    = config.get<float>("padding", "editor");

			m_backward.load("../editor_data/icons/backward.png");
			m_backward.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_backward.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_file.load("../editor_data/icons/file.png");
			m_file.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_file.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_folder.load("../editor_data/icons/folder.png");
			m_folder.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_folder.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_forward.load("../editor_data/icons/forward.png");
			m_forward.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_forward.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_reload.load("../editor_data/icons/reload.png");
			m_reload.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_reload.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_ext_map.emplace(".ogg", FileType::AUDIO);
			m_ext_map.emplace(".wav", FileType::AUDIO);
			m_ext_map.emplace(".mp3", FileType::AUDIO);
			m_ext_map.emplace(".ttf", FileType::FONT);
			m_ext_map.emplace(".otf", FileType::FONT);
			m_ext_map.emplace(GALAXY_VERTEX_EXT, FileType::SHADER);
			m_ext_map.emplace(GALAXY_FRAGMENT_EXT, FileType::SHADER);
			m_ext_map.emplace(".json", FileType::JSON);
			m_ext_map.emplace(".lang", FileType::LANG);
			m_ext_map.emplace(".lua", FileType::LUA);
			m_ext_map.emplace(".scproj", FileType::PROJ);
			m_ext_map.emplace(".png", FileType::TEXTURE);
			m_ext_map.emplace(".bmp", FileType::TEXTURE);
			m_ext_map.emplace(".jpg", FileType::TEXTURE);
			m_ext_map.emplace(".jpeg", FileType::TEXTURE);
			m_ext_map.emplace(".gif", FileType::TEXTURE);
			m_ext_map.emplace(".tmx", FileType::MAP);
			m_ext_map.emplace(".tmj", FileType::MAP);
			m_ext_map.emplace(".gprefab", FileType::PREFAB);

			m_tex_map.try_emplace(FileType::AUDIO);
			m_tex_map.try_emplace(FileType::FONT);
			m_tex_map.try_emplace(FileType::SHADER);
			m_tex_map.try_emplace(FileType::JSON);
			m_tex_map.try_emplace(FileType::LANG);
			m_tex_map.try_emplace(FileType::LUA);
			m_tex_map.try_emplace(FileType::PROJ);
			m_tex_map.try_emplace(FileType::TEXTURE);
			m_tex_map.try_emplace(FileType::MAP);
			m_tex_map.try_emplace(FileType::PREFAB);

			m_tex_map[FileType::AUDIO].load("../editor_data/icons/audio.png");
			m_tex_map[FileType::AUDIO].set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_tex_map[FileType::AUDIO].set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_tex_map[FileType::FONT].load("../editor_data/icons/font.png");
			m_tex_map[FileType::FONT].set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_tex_map[FileType::FONT].set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_tex_map[FileType::SHADER].load("../editor_data/icons/glsl.png");
			m_tex_map[FileType::SHADER].set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_tex_map[FileType::SHADER].set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_tex_map[FileType::JSON].load("../editor_data/icons/json.png");
			m_tex_map[FileType::JSON].set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_tex_map[FileType::JSON].set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_tex_map[FileType::LANG].load("../editor_data/icons/lang.png");
			m_tex_map[FileType::LANG].set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_tex_map[FileType::LANG].set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_tex_map[FileType::LUA].load("../editor_data/icons/lua.png");
			m_tex_map[FileType::LUA].set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_tex_map[FileType::LUA].set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_tex_map[FileType::PROJ].load("../editor_data/icons/proj.png");
			m_tex_map[FileType::PROJ].set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_tex_map[FileType::PROJ].set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_tex_map[FileType::TEXTURE].load("../editor_data/icons/texture.png");
			m_tex_map[FileType::TEXTURE].set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_tex_map[FileType::TEXTURE].set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_tex_map[FileType::MAP].load("../editor_data/icons/map.png");
			m_tex_map[FileType::MAP].set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_tex_map[FileType::MAP].set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_tex_map[FileType::PREFAB].load("../editor_data/icons/prefab.png");
			m_tex_map[FileType::PREFAB].set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_tex_map[FileType::PREFAB].set_filter(graphics::TextureFilters::MAG_TRILINEAR);
		}

		void AssetPanel::render(CodeEditor& editor, UpdateStack& updates)
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
				top(updates);

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

		void AssetPanel::top(UpdateStack& updates)
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

			if (disable_forward)
			{
				ImGui::PopStyleVar();
				ImGui::PopItemFlag();
			}

			ImGui::SameLine();

			if (ui::imgui_imagebutton(m_reload, m_toolbar_vec))
			{
				ImGui::OpenPopup("AssetReloadPopup");
			}

			if (ImGui::BeginPopup("AssetReloadPopup"))
			{
				ImGui::Text("Select resource to reload");
				ImGui::Separator();

				if (ImGui::MenuItem("Languages"))
				{
					updates.push_back([]() {
						core::ServiceLocator<resource::Language>::ref().reload();
					});
				}

				if (ImGui::MenuItem("Texture Atlas"))
				{
					updates.push_back([]() {
						core::ServiceLocator<resource::TextureAtlas>::ref().reload();
					});
				}

				ImGui::EndPopup();
			}

			ImGui::SameLine();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
			m_filter.DrawWithHint("###AssetPanelSearch", ICON_MDI_MAGNIFY "Search...", ImGui::GetContentRegionAvail().x);

			ImGui::Spacing();
			ImGui::TextWrapped(m_current_dir.string().c_str());
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
					ImGui::TableNextColumn();

					const auto file = path.filename().string();
					const auto ext  = path.extension().string();

					if (m_filter.PassFilter(file.c_str()))
					{
						ImGui::PushID(file.c_str());

						if (!path.has_extension())
						{
							m_icon = &m_folder;
						}
						else
						{
							if (m_ext_map.contains(ext))
							{
								m_icon = &m_tex_map[m_ext_map[ext]];
							}
							else
							{
								m_icon = &m_file;
							}
						}

						if (m_selected.m_path == path)
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
							m_selected.m_path      = path;
							m_selected.m_extension = ext;
						}

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							if (!path.has_extension())
							{
								m_current_dir /= file;
								m_update_directories = true;
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
							m_create_folder_popup = true;
						}

						if (ImGui::MenuItem(ICON_MDI_LANGUAGE_LUA " Script"))
						{
							auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

							const auto path = std::filesystem::path(m_current_dir / "script.lua");
							if (!fs.save("-- galaxy script (lua)", path.string()))
							{
								ui::imgui_notify_error("Failed to save new script.");
							}

							m_update_directories = true;
						}

						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu(ICON_MDI_IMPORT " Import"))
					{
						if (ImGui::BeginMenu("Audio"))
						{
							if (ImGui::MenuItem("Music"))
							{
								import_files("music_folder");
							}

							if (ImGui::MenuItem("SFX"))
							{
								import_files("sfx_folder");
							}

							if (ImGui::MenuItem("Dialogue"))
							{
								import_files("dialogue_folder");
							}

							ImGui::EndMenu();
						}

						if (ImGui::BeginMenu("Texture"))
						{
							if (ImGui::MenuItem("Atlas Texture"))
							{
								import_files("atlas_folder");
							}

							if (ImGui::MenuItem("Standalone Texture"))
							{
								import_files("texture_folder");
							}

							ImGui::EndMenu();
						}

						if (ImGui::MenuItem("Shader"))
						{
							import_files("shader_folder");
						}

						if (ImGui::MenuItem("Script"))
						{
							import_files("scripts_folder");
						}

						if (ImGui::MenuItem("Language"))
						{
							import_files("lang_folder");
						}

						if (ImGui::MenuItem("Font"))
						{
							import_files("font_folder");
						}

						if (ImGui::MenuItem("Maps"))
						{
							import_files("maps_folder");
						}

						if (ImGui::MenuItem("Prefabs"))
						{
							import_files("prefabs_folder");
						}

						ImGui::EndMenu();
					}

					if (!m_selected.m_extension.empty())
					{
						switch (m_ext_map[m_selected.m_extension])
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
							ui::imgui_open_confirm("AssetDeleteConfirm");
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

				if (m_create_folder_popup)
				{
					m_create_folder_popup = false;
					ImGui::OpenPopup("AssetCreateFolder");
				}

				if (ImGui::BeginPopup("AssetCreateFolder"))
				{
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

					ImGui::EndPopup();
				}

				ui::imgui_confirm("AssetDeleteConfirm", [&]() {
					std::filesystem::remove(m_selected.m_path);
				});

				if (m_open_preview)
				{
					thread_local constexpr const auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
															  ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;

					ui::imgui_center_next_window();
					ImGui::SetNextWindowSize({256, 256});

					if (ImGui::Begin("Preview", &m_open_preview, flags))
					{
						ui::imgui_image(m_preview, {m_preview.get_widthf(), m_preview.get_heightf()});
					}

					ImGui::End();
				}

				if (m_open_config)
				{
					m_open_config = false;
					ImGui::OpenPopup("AssetConfigPopup");
				}

				if (ImGui::BeginPopup("AssetConfigPopup"))
				{
					if (ImGui::SliderFloat("Thumbnail Size", &m_thumb_size, 16.0f, 128.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
					{
						auto& config = core::ServiceLocator<core::Config>::ref();
						config.set("thumb_size", m_thumb_size, "editor");
						config.save();
					}

					if (ImGui::SliderFloat("Padding", &m_padding, 0.0f, 64.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
					{
						auto& config = core::ServiceLocator<core::Config>::ref();
						config.set("padding", m_padding, "editor");
						config.save();
					}

					ImGui::EndPopup();
				}

				ImGui::EndTable();
			}
		}

		void AssetPanel::load_lua_script(CodeEditor& editor)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto data = fs.open(m_selected.m_path.string());
			if (!data.empty())
			{
				editor.m_editor.SetText(data);
				editor.m_file = m_selected.m_path;
			}
		}

		void AssetPanel::load_preview()
		{
			m_preview.recreate();
			m_preview.load(m_selected.m_path.string());
			m_open_preview = true;
		}

		void AssetPanel::import_files(const std::string& folder_from_config)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto list = fs.open_file_dialog_multi();
			if (!list.empty())
			{
				auto& config  = core::ServiceLocator<core::Config>::ref();
				const auto to = fs.root_path() / config.get<std::string>(folder_from_config, "resource_folders");

				for (const auto& path : list)
				{
					const auto fs_path = std::filesystem::path(path);
					try
					{
						std::filesystem::copy(fs_path, to, std::filesystem::copy_options::overwrite_existing);
					}
					catch (const std::exception& e)
					{
						ui::imgui_notify_error(e.what());
					}
				}

				m_update_directories = true;
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
	} // namespace panel
} // namespace sc