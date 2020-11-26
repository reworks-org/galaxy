///
/// JsonEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>
#include <fstream>

#include <galaxy/scripting/JSONUtils.hpp>
#include <imgui/imgui_stdlib.h>
#include <imgui/addons/ToggleButton.h>
#include <pulsar/Log.hpp>

#include "JsonEditor.hpp"

#define INDENT_PIXELS 16.0f

namespace sc
{
	JsonEditor::JsonEditor()
	    : m_loaded {false}
	{
	}

	void JsonEditor::load_file(std::string_view file)
	{
		m_root   = galaxy::json::parse_from_disk(file);
		m_loaded = true;
	}

	void JsonEditor::load_mem(std::span<char> memory)
	{
		m_root   = galaxy::json::parse_from_mem(memory);
		m_loaded = true;
	}

	void JsonEditor::save(std::string_view path)
	{
		if (m_loaded)
		{
			galaxy::json::save_to_disk(path, m_root);
		}
		else
		{
			PL_LOG(PL_WARNING, "Attempted to save json file when no json was open.");
		}
	}

	void JsonEditor::parse_and_display()
	{
		if (m_loaded)
		{
			if (m_root.is_object())
			{
				do_object(m_root);
			}
			else if (m_root.is_array())
			{
				do_array(m_root);
			}
			else
			{
				PL_LOG(PL_FATAL, "JSON was not object or array.");
			}
		}
	}

	const bool JsonEditor::is_loaded() const
	{
		return m_loaded;
	}

	void JsonEditor::do_object(nlohmann::json& json)
	{
		ImGui::Text("{");
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
				ImGui::ToggleButton(key.c_str(), value.get<bool*>());
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
				ImGui::InputDouble(key.c_str(), value.get<double*>());
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
		ImGui::Text("}");
	}

	void JsonEditor::do_array(nlohmann::json& json)
	{
		ImGui::Text("[");
		ImGui::Indent(INDENT_PIXELS);

		unsigned int counter = 0;
		std::string name     = "";
		for (auto& elem : json)
		{
			name = "[" + std::to_string(counter) + "]";
			if (elem.is_array())
			{
				do_array(elem);
			}
			else if (elem.is_boolean())
			{
				ImGui::ToggleButton(name.c_str(), elem.get<bool*>());
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
				ImGui::InputDouble(name.c_str(), elem.get<double*>());
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
		ImGui::Text("]");
	}
} // namespace sc