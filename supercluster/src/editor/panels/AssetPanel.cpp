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
			, m_create_folder_popup {false}
		{
			auto& fs      = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			m_root        = fs.root_path();
			m_current_dir = fs.root_path();

			auto& config = core::ServiceLocator<core::Config>::ref();
			config.restore("padding", 48.0f, "editor");
			config.restore("thumb_size", 96.0f, "editor");
			config.save();

			m_thumb_size = config.get<float>("thumb_size", "editor");
			m_padding    = config.get<float>("padding", "editor");

			m_audio.load("../editor_data/icons/audio.png");
			m_audio.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_audio.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_backward.load("../editor_data/icons/backward.png");
			m_backward.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_backward.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_file.load("../editor_data/icons/file.png");
			m_file.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_file.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_folder.load("../editor_data/icons/folder.png");
			m_folder.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_folder.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_font.load("../editor_data/icons/font.png");
			m_font.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_font.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_forward.load("../editor_data/icons/forward.png");
			m_forward.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_forward.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_glsl.load("../editor_data/icons/glsl.png");
			m_glsl.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_glsl.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_json.load("../editor_data/icons/json.png");
			m_json.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_json.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_lang.load("../editor_data/icons/lang.png");
			m_lang.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_lang.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_lua.load("../editor_data/icons/lua.png");
			m_lua.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_lua.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_proj.load("../editor_data/icons/proj.png");
			m_proj.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_proj.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_texture.load("../editor_data/icons/texture.png");
			m_texture.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_texture.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_reload.load("../editor_data/icons/reload.png");
			m_reload.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_reload.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_map.load("../editor_data/icons/map.png");
			m_map.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_map.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_prefab.load("../editor_data/icons/prefab.png");
			m_prefab.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_prefab.set_filter(graphics::TextureFilters::MAG_TRILINEAR);
		}

		void AssetPanel::render(CodeEditor& editor, UpdateStack& updates)
		{
			if (ImGui::Begin(ICON_MDI_FOLDER_ARROW_DOWN " Asset Browser"))
			{
				top(updates);

				if (ImGui::BeginTable("AssetPanelLayoutTable",
						2,
						ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX |
							ImGuiTableFlags_ScrollY,
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

			auto root_str = m_root.string();
			root_str.pop_back();

			const auto disable_back = m_current_dir.parent_path().string().find(root_str) == std::string::npos;
			if (disable_back)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			if (ui::imgui_imagebutton(m_backward, m_toolbar_vec))
			{
				m_prev_dir    = m_current_dir;
				m_current_dir = m_current_dir.parent_path();
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
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0.0f, 0.0f});

			if (ImGui::CollapsingHeader("Assets", ImGuiTreeNodeFlags_SpanAvailWidth))
			{
				unsigned int count = 0;
				for (const auto& entry : std::filesystem::recursive_directory_iterator(m_root))
				{
					count++;
				}

				directory_tree_view_recursive(m_root, &count);
			}

			ImGui::PopStyleVar();
		}

		void AssetPanel::body(CodeEditor& editor)
		{
			const auto columns = static_cast<int>((ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize) / (m_size_vec.x + m_padding));

			constexpr const ImGuiTableFlags flags =
				ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoClip;

			if (ImGui::BeginTable("AssetPanelColumns", std::max(columns, 1), flags))
			{
				for (const auto& entry : std::filesystem::directory_iterator(m_current_dir))
				{
					ImGui::TableNextColumn();

					const auto& path = entry.path();
					const auto file  = path.filename().string();
					const auto ext   = path.extension();

					if (m_filter.PassFilter(file.c_str()))
					{
						ImGui::PushID(file.c_str());

						if (entry.is_directory())
						{
							m_icon = &m_folder;
						}
						else
						{
							if (ext == ".ogg" || ext == ".wav" || ext == ".mp3")
							{
								m_icon = &m_audio;
							}
							else if (ext == ".ttf" || ext == ".otf")
							{
								m_icon = &m_font;
							}
							else if (ext == GALAXY_VERTEX_EXT || ext == GALAXY_FRAGMENT_EXT)
							{
								m_icon = &m_glsl;
							}
							else if (ext == ".json")
							{
								m_icon = &m_json;
							}
							else if (ext == ".lang")
							{
								m_icon = &m_lang;
							}
							else if (ext == ".lua")
							{
								m_icon = &m_lua;
							}
							else if (ext == ".scproj")
							{
								m_icon = &m_proj;
							}
							else if (ext == ".png" || ext == ".bmp" || ext == ".jpg" || ext == ".jpeg" || ext == ".gif")
							{
								m_icon = &m_texture;
							}
							else if (ext == ".tmx" || ext == ".tmj")
							{
								m_icon = &m_map;
							}
							else if (ext == ".gprefab")
							{
								m_icon = &m_prefab;
							}
							else
							{
								m_icon = &m_file;
							}
						}

						ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((ImGui::GetColumnWidth() / 2.0f) - (m_size_vec.x / 2.0f)));
						ui::imgui_imagebutton(*m_icon, m_size_vec);
						ImGui::PopStyleColor();

						if (ImGui::BeginDragDropSource())
						{
							ImGui::SetDragDropPayload("AssetPanelItem", file.c_str(), file.size(), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}

						if (!entry.is_directory() && ImGui::IsItemHovered())
						{
							m_selected.m_path       = path;
							m_selected.m_extension  = ext.string();
							m_selected.m_is_hovered = true;
						}
						else
						{
							m_selected.m_is_hovered = false;
						}

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							if (entry.is_directory())
							{
								m_current_dir /= file;
							}
							else if (ext == ".lua")
							{
								load_lua_script(editor);
							}
						}

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((ImGui::GetColumnWidth() / 2.0f) - (ImGui::CalcTextSize(file.c_str()).x / 2.0f)));
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
					if (ImGui::BeginMenu(ICON_MDI_PLUS " Create"))
					{
						if (ImGui::MenuItem(ICON_MDI_FOLDER_PLUS " Folder"))
						{
							m_create_folder_popup = true;
							ImGui::CloseCurrentPopup();
						}

						if (ImGui::MenuItem(ICON_MDI_LANGUAGE_LUA " Script"))
						{
							auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

							const auto path = std::filesystem::path(m_current_dir / "script.lua");
							if (!fs.save("-- galaxy script (lua)", path.string()))
							{
								ui::imgui_notify_error("Failed to save new script.");
							}

							ImGui::CloseCurrentPopup();
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

					if (m_selected.m_is_hovered)
					{
						if (m_selected.m_extension == ".lua")
						{
							if (ImGui::MenuItem(ICON_MDI_FILE_EDIT " Edit"))
							{
								load_lua_script(editor);

								ImGui::CloseCurrentPopup();
							}
						}

						if (ImGui::MenuItem(ICON_MDI_DELETE_ALERT " Delete"))
						{
							ui::imgui_open_confirm("AssetDeleteConfirm");
						}
					}

					if (ImGui::MenuItem(ICON_MDI_COG " Settings"))
					{
						m_open_config = true;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
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
						}
					}

					ImGui::EndPopup();
				}

				ui::imgui_confirm("AssetDeleteConfirm", [&]() {
					std::filesystem::remove(m_selected.m_path);
				});

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
					m_prev_dir    = m_current_dir;
					m_current_dir = entry.path();
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
	} // namespace panel
} // namespace sc