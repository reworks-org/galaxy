///
/// AssetPanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui_addons/imgui_notify.h>

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

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		AssetPanel::AssetPanel()
			: m_padding {48.0f}
			, m_thumb_size {48.0f}
			, m_toolbar_vec {24, 24}
			, m_icon {nullptr}
		{
			auto& fs      = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			m_root        = fs.root_path();
			m_current_dir = fs.root_path();

			auto& config = core::ServiceLocator<core::Config>::ref();
			config.restore("padding", m_padding, "editor");
			config.restore("thumb_size", m_thumb_size, "editor");
			config.save();

			m_audio.load("editor_data/icons/audio.png");
			m_audio.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_audio.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_backward.load("editor_data/icons/backward.png");
			m_backward.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_backward.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_cog.load("editor_data/icons/cog.png");
			m_cog.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_cog.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_file.load("editor_data/icons/file.png");
			m_file.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_file.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_folder.load("editor_data/icons/folder.png");
			m_folder.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_folder.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_font.load("editor_data/icons/font.png");
			m_font.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_font.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_forward.load("editor_data/icons/forward.png");
			m_forward.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_forward.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_glsl.load("editor_data/icons/glsl.png");
			m_glsl.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_glsl.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_json.load("editor_data/icons/json.png");
			m_json.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_json.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_lang.load("editor_data/icons/lang.png");
			m_lang.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_lang.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_lua.load("editor_data/icons/lua.png");
			m_lua.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_lua.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_proj.load("editor_data/icons/proj.png");
			m_proj.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_proj.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_texture.load("editor_data/icons/texture.png");
			m_texture.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_texture.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_reload.load("editor_data/icons/reload.png");
			m_reload.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_reload.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_map.load("editor_data/icons/map.png");
			m_map.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_map.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_prefab.load("editor_data/icons/prefab.png");
			m_prefab.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_prefab.set_filter(graphics::TextureFilters::MAG_TRILINEAR);
		}

		void AssetPanel::render(CodeEditor& editor, UpdateStack& updates)
		{
			if (ImGui::Begin("Asset Browser"))
			{
				m_size_vec.x     = m_thumb_size;
				m_size_vec.y     = m_thumb_size;
				auto cell_size   = m_thumb_size + m_padding;
				auto panel_width = ImGui::GetContentRegionAvail().x;
				auto columns     = std::max(static_cast<int>(panel_width / cell_size), 1);

				if (ui::imgui_imagebutton(m_backward, m_toolbar_vec))
				{
					m_prev_dir    = m_current_dir;
					m_current_dir = m_current_dir.parent_path();
				}

				ImGui::SameLine();

				if (ui::imgui_imagebutton(m_forward, m_toolbar_vec))
				{
					if (!m_prev_dir.empty())
					{
						m_current_dir = m_prev_dir;
					}
				}

				ImGui::SameLine();

				if (ui::imgui_imagebutton(m_reload, m_toolbar_vec))
				{
					ImGui::OpenPopup("AssetReloadPopup");
				}

				if (ImGui::BeginPopup("AssetReloadPopup"))
				{
					ImGui::Text("Select resource to reload.");
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

				ImGui::SetNextItemWidth(panel_width / 4.0f);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);

				ImGui::InputTextWithHint("##AssetPanelSearch", "Search...", &m_search_term, ImGuiInputTextFlags_AutoSelectAll);

				ImGui::SameLine();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
				if (ImGui::Button("Import"))
				{
					ImGui::OpenPopup("AssetPanelContextMenu");
				}

				ImGui::SameLine();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
				ImGui::TextWrapped(m_current_dir.string().c_str());

				ImGui::SameLine();

				ImGui::SetCursorPosX(panel_width - m_toolbar_vec.x);

				if (ui::imgui_imagebutton(m_cog, m_toolbar_vec))
				{
					ImGui::OpenPopup("AssetConfigPopup");
				}

				if (ImGui::BeginPopup("AssetConfigPopup"))
				{
					if (ImGui::SliderFloat("Thumbnail Size", &m_thumb_size, 16.0f, 48.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
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

				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Columns(columns, "AssetPanelColumns", false);

				for (const auto& entry : std::filesystem::directory_iterator(m_current_dir))
				{
					const auto& path = entry.path();
					const auto file  = path.filename().string();
					const auto ext   = path.extension();

					if (file.find(m_search_term) != std::string::npos)
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

						static const constexpr auto s_btn_col = ImVec4 {0, 0, 0, 0};
						ImGui::PushStyleColor(ImGuiCol_Button, s_btn_col);

						ui::imgui_imagebutton(*m_icon, m_size_vec);

						if (ImGui::BeginDragDropSource())
						{
							ImGui::SetDragDropPayload("AssetPanelItem", file.c_str(), file.size(), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}

						ImGui::PopStyleColor();

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

						ImGui::TextWrapped(file.c_str());

						ImGui::NextColumn();
						ImGui::PopID();
					}
				}

				ImGui::Columns();

				if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("AssetPanelContextMenu");
				}

				if (ImGui::BeginPopupContextWindow("AssetPanelContextMenu"))
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

					if (m_selected.m_is_hovered)
					{
						if (m_selected.m_extension == ".lua")
						{
							if (ImGui::MenuItem("Edit"))
							{
								load_lua_script(editor);

								ImGui::CloseCurrentPopup();
							}
						}
					}

					ImGui::EndPopup();
				}
			}

			ImGui::End();
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
						ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, e.what()});
					}
				}
			}
		}
	} // namespace panel
} // namespace sc