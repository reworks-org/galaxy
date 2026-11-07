///
/// ConfigPanel.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/Config.hpp"
#include "galaxy/core/Settings.hpp"
#include "galaxy/ui/ImGuiHelpers.hpp"

#include "ConfigPanel.hpp"

#define INDENT_PIXELS 16.0f

namespace galaxy
{
	ConfigPanel::ConfigPanel() noexcept
		: m_counter {0}
	{
	}

	ConfigPanel::~ConfigPanel() noexcept
	{
	}

	void ConfigPanel::draw()
	{
		if (ImGui::Begin("Config Editor", &m_show, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::MenuItem("Save"))
				{
					entt::locator<Config>::value().save();
					ui::imgui_notify_success("Config changed, a restart is needed.");
				}

				if (ImGui::MenuItem("Reset"))
				{
					Settings::set_config_to_default();
					ui::imgui_notify_success("Config restored to default.");
				}

				if (ImGui::MenuItem("Reload"))
				{
					Settings::set_settings_from_config();
					ui::imgui_notify_success("Settings restored from updated config.");
				}

				ImGui::EndMenuBar();
			}

			m_counter = 0;

			nlohmann::json& root = entt::locator<Config>::value().raw();
			if (root.is_object())
			{
				do_object(root);
			}
			else if (root.is_array())
			{
				do_array(root);
			}
		}

		ImGui::End();
	}

	void ConfigPanel::do_object(nlohmann::json& json)
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
				ImGui::Checkbox(key.c_str(), value.get<bool*>());
			}
			else if (value.is_number_integer())
			{
				auto data = value.get<int>();
				if (ImGui::InputInt(key.c_str(), &data))
				{
					value = data;
				}
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

	void ConfigPanel::do_array(nlohmann::json& json)
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
				ImGui::Checkbox(name.c_str(), elem.get<bool*>());

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
} // namespace galaxy
