///
/// Popup.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/ui/ImGuiHelpers.hpp"

#include "Popup.hpp"

namespace galaxy
{
	Popup::Popup(std::string_view id) noexcept
		: m_show {false}
	{
		m_id = id;
	}

	Popup::~Popup() noexcept
	{
	}

	void Popup::draw_button(const char* text) noexcept
	{
		m_show = ImGui::Button(text);
	}

	void Popup::draw_menuitem(const char* text) noexcept
	{
		m_show = ImGui::MenuItem(text);
	}

	void Popup::draw(std::move_only_function<void(void)>&& func)
	{
		if (m_show)
		{
			ImGui::OpenPopup(m_id.c_str());
			m_show = false;
		}

		ui::imgui_center_next_window();
		if (ImGui::BeginPopup(m_id.c_str()))
		{
			func();
			ImGui::EndPopup();
		}
	}
} // namespace galaxy
