///
/// AssetPanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/Config.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>

#include "AssetPanel.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		AssetPanel::AssetPanel() noexcept
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
		}

		void AssetPanel::render()
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

				ImGui::SetNextItemWidth(panel_width / 4.0f);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);

				ImGui::InputTextWithHint("##AssetPanelSearch", "Search...", &m_search_term, ImGuiInputTextFlags_AutoSelectAll);

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

					if (file.find(m_search_term) != std::string::npos)
					{
						ImGui::PushID(file.c_str());

						if (entry.is_directory())
						{
							m_icon = &m_folder;
						}
						else
						{
							const auto ext = path.extension().string();
							if (ext == ".ogg")
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
							else
							{
								m_icon = &m_file;
							}
						}

						static const constexpr auto s_btn_col = ImVec4 {0, 0, 0, 0};
						ImGui::PushStyleColor(ImGuiCol_Button, s_btn_col);

						ui::imgui_imagebutton(*m_icon, m_size_vec);

						/*
						if (ImGui::BeginDragDropSource())
						{
							auto relativePath       = std::filesystem::relative(path, g_AssetPath);
							const wchar_t* itemPath = relativePath.c_str();
							ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
							ImGui::EndDragDropSource();
						}*/

						ImGui::PopStyleColor();

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							if (entry.is_directory())
							{
								m_current_dir /= file;
							}
						}

						ImGui::TextWrapped(file.c_str());

						ImGui::NextColumn();
						ImGui::PopID();
					}
				}

				ImGui::Columns();
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc