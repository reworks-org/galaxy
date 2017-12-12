///
///  SerializeComponent.cpp
///  rework
///
///  Created by reworks on 21/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include "sol2/sol.hpp"
#include "imgui/imgui_impl_a5.h"

#include "SerializeComponent.hpp"

namespace re
{
	SerializeComponent::SerializeComponent(const sol::table& table)
	{
		m_id = table.get<std::string>("id");
	}

	SerializeComponent::~SerializeComponent()
	{
	}

	void SerializeComponent::debug()
	{
		ImGui::Spacing();
		ImGui::stl::InputText("Serialize ID", &m_id, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue);
	}
}