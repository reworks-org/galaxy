///
/// SettingsPanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/toggle/imgui_toggle.h>

#include <galaxy/core/Config.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "SettingsPanel.hpp"

#define INDENT_PIXELS 16.0f

using namespace galaxy;

namespace sc
{
	SettingsPanel::SettingsPanel()
		: m_counter {0}
	{
	}

	SettingsPanel::~SettingsPanel()
	{
	}

	void SettingsPanel::load(const nlohmann::json& json)
	{
		m_root = json;
	}

	const nlohmann::json& SettingsPanel::save()
	{
		return m_root;
	}

	void SettingsPanel::render()
	{
		if (m_show)
		{
			if (ImGui::Begin("Settings", &m_show, ImGuiWindowFlags_MenuBar))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem("Save"))
					{
						auto& config = core::ServiceLocator<core::Config>::ref();
						config.raw(save());
						config.save();

						ui::imgui_notify_success("Settings changed, a restart is needed.");
					}

					if (ImGui::MenuItem("Refresh"))
					{
						load(core::ServiceLocator<core::Config>::ref().raw());
					}

					ImGui::EndMenuBar();
				}

				m_counter = 0;

				if (m_root.is_object())
				{
					do_object(m_root);
				}
				else if (m_root.is_array())
				{
					do_array(m_root);
				}
			}

			ImGui::End();
		}
	}

	void SettingsPanel::do_object(nlohmann::json& json)
	{
		m_counter++;
		ImGui::PushID(m_counter);

		ImGui::Indent(INDENT_PIXELS);

		for (auto& [key, value] : json.items())
		{
			if (value.is_array())
			{
				if (!key.empty())
				{
					ImGui::Text(key.c_str());
				}

				do_array(value);
			}
			else if (value.is_boolean())
			{
				ImGui::Toggle(key.c_str(), value.get<bool*>());
				ImGui::SameLine();
				ImGui::Text(key.c_str());
			}
			else if (value.is_number_integer())
			{
				ImGui::InputScalar(key.c_str(), ImGuiDataType_S64, reinterpret_cast<void*>(value.get<std::int64_t*>()));
			}
			else if (value.is_number_unsigned())
			{
				ImGui::InputScalar(key.c_str(), ImGuiDataType_U64, reinterpret_cast<void*>(value.get<std::uint64_t*>()));
			}
			else if (value.is_number_float())
			{
				ImGui::InputDouble(key.c_str(), value.get<double*>(), 0.1f, 1.0f, "%.1f");
			}
			else if (value.is_string())
			{
				ImGui::InputText(key.c_str(), value.get<std::string*>(), ImGuiInputTextFlags_EnterReturnsTrue);
			}
			else if (value.is_object())
			{
				if (!key.empty())
				{
					ImGui::Text(key.c_str());
				}

				do_object(value);
			}
		}

		ImGui::Unindent(INDENT_PIXELS);

		ImGui::PopID();
	}

	void SettingsPanel::do_array(nlohmann::json& json)
	{
		m_counter++;
		ImGui::PushID(m_counter);

		ImGui::Indent(INDENT_PIXELS);

		unsigned int counter = 0;
		std::string  name;
		for (auto& elem : json)
		{
			name = std::format("[{0}]", counter);
			if (elem.is_array())
			{
				do_array(elem);
			}
			else if (elem.is_boolean())
			{
				ImGui::Toggle(name.c_str(), elem.get<bool*>());
				ImGui::SameLine();
				ImGui::Text(name.c_str());

				counter++;
			}
			else if (elem.is_number_integer())
			{
				ImGui::InputScalar(name.c_str(), ImGuiDataType_S64, reinterpret_cast<void*>(elem.get<std::int64_t*>()));
				counter++;
			}
			else if (elem.is_number_unsigned())
			{
				ImGui::InputScalar(name.c_str(), ImGuiDataType_U64, reinterpret_cast<void*>(elem.get<std::uint64_t*>()));
				counter++;
			}
			else if (elem.is_number_float())
			{
				ImGui::InputDouble(name.c_str(), elem.get<double*>(), 0.1f, 1.0f, "%.1f");
				counter++;
			}
			else if (elem.is_string())
			{
				ImGui::InputText(name.c_str(), elem.get<std::string*>(), ImGuiInputTextFlags_EnterReturnsTrue);
				counter++;
			}
			else if (elem.is_object())
			{
				do_object(elem);
			}
		}

		ImGui::Unindent(INDENT_PIXELS);
		ImGui::PopID();
	}
} // namespace sc
