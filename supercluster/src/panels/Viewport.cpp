///
/// Viewport.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imnotify/ImGuiNotify.hpp>

#include <galaxy/ui/ImGuiHelpers.hpp>

#include "Viewport.hpp"

namespace sc
{
	Viewport::Viewport()
		: m_show {true}
		, m_viewport_focused {false}
		, m_viewport_hovered {false}
		, m_viewport_size {0.0f, 0.0f}
		, m_padding {4, 4}
	{
		m_framebuffer.create(1, 1);

		auto& fb = m_framebuffer.fbo();
		fb.add_storage_attachment();
		fb.create();

		if (m_camera_btn.load("icons/camera.png"))
		{
			m_camera_btn.filter(graphics::TextureFilter::TRILINEAR);
		}

		if (m_editor_cam_btn.load("icons/video.png"))
		{
			m_editor_cam_btn.filter(graphics::TextureFilter::TRILINEAR);
		}
	}

	Viewport::~Viewport()
	{
	}

	void Viewport::set_active()
	{
		m_framebuffer.bind(true);
		m_framebuffer.fbo().clear_storagebuffer(-1);
	}

	void Viewport::render(EditorState state, graphics::Camera& editor_camera)
	{
		constexpr const ImGuiWindowFlags flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		if (ImGui::Begin(ICON_MDI_WINDOW_MAXIMIZE " Viewport", nullptr, flags))
		{
			const auto viewport_min = ImGui::GetWindowContentRegionMin();
			const auto viewport_max = ImGui::GetWindowContentRegionMax();
			const auto offset       = ImGui::GetWindowPos();

			m_viewport_bounds[0] = {viewport_min.x + offset.x, viewport_min.y + offset.y};
			m_viewport_bounds[1] = {viewport_max.x + offset.x, viewport_max.y + offset.y};

			m_viewport_focused = ImGui::IsWindowFocused();
			m_viewport_hovered = ImGui::IsWindowHovered();

			const auto size_avail = ImGui::GetContentRegionAvail();
			if (size_avail != m_viewport_size)
			{
				m_viewport_size = size_avail;
				m_framebuffer.recreate(m_viewport_size.x, m_viewport_size.y);
				editor_camera.set_viewport(m_viewport_size.x, m_viewport_size.y);
			}

			if (state == EditorState::EDITOR)
			{
				ImGui::BeginGroup();
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, m_padding);

					ImGui::SetCursorPos(ImGui::GetCursorPos() + m_padding);
					if (ui::imgui_imagebutton(m_camera_btn, {32, 32}))
					{
						m_editor_cam_enabled = false;
					}

					ImGui::SameLine();

					if (ui::imgui_imagebutton(m_editor_cam_btn, {32, 32}))
					{
						m_editor_cam_enabled = true;
					}

					ImGui::PopStyleVar(2);
				}
				ImGui::EndGroup();
			}

			ui::imgui_image(m_framebuffer, m_viewport_size);
		}

		ImGui::PopStyleVar(1);
		ImGui::End();
	}
} // namespace sc
