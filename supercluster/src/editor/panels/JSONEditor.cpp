///
/// JSONEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/error/Log.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/scripting/JSONUtils.hpp>

#include <imgui/addons/ToggleButton.h>
#include <imgui/imgui_stdlib.h>

#include "JSONEditor.hpp"

#define INDENT_PIXELS 16.0f

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		JSONEditor::JSONEditor()
		    : m_counter {0}, m_loaded {false}, m_external {nullptr}
		{
		}

		void JSONEditor::create_new()
		{
			if (ImGui::BeginPopup("create_new", ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Select root object type:");
				ImGui::Separator();

				if (ImGui::Button("Array"))
				{
					m_root   = nlohmann::json::array();
					m_loaded = true;

					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("Object"))
				{
					m_root   = nlohmann::json::object();
					m_loaded = true;

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		void JSONEditor::load_file(std::string_view file)
		{
			if (!m_loaded)
			{
				const auto json_opt = json::parse_from_disk(file);
				if (json_opt == std::nullopt)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to create parse/load json file: {0}, for JSONEditor panel.", file);
					m_loaded = false;
				}
				else
				{
					m_root   = json_opt.value();
					m_loaded = true;
				}
			}
		}

		void JSONEditor::load_mem(std::span<char> memory)
		{
			if (!m_loaded)
			{
				const auto json_opt = json::parse_from_mem(memory);
				if (json_opt == std::nullopt)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to create parse/load json from memory for JSONEditor. panel.");
					m_loaded = false;
				}
				else
				{
					m_root   = json_opt.value();
					m_loaded = true;
				}
			}
		}

		void JSONEditor::load_json(nlohmann::json* json)
		{
			if (!m_loaded)
			{
				m_external = json;
				m_loaded   = true;
			}
		}

		void JSONEditor::save(std::string_view path)
		{
			if (m_loaded)
			{
				if (!m_external)
				{
					if (!json::save_to_disk(path, m_root))
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to save json to disk using file: {0}, for JSONEditor panel.", path);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Cannot save external json.");
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to save json file when no json was open.");
			}
		}

		void JSONEditor::parse_and_display()
		{
			if (ImGui::Begin("JSON Editor", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem("New"))
					{
						ImGui::OpenPopup("create_new", ImGuiPopupFlags_NoOpenOverExistingPopup);
					}

					if (ImGui::MenuItem("Open"))
					{
						const auto path = SL_HANDLE.vfs()->show_open_dialog("*.json");
						if (path == std::nullopt)
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to find file to open for JSONEditor panel.");
						}
						else
						{
							load_file(path.value());
						}
					}

					if (ImGui::MenuItem("Save"))
					{
						if (is_loaded())
						{
							const auto path = SL_HANDLE.vfs()->show_save_dialog();
							if (path == std::nullopt)
							{
								GALAXY_LOG(GALAXY_ERROR, "Failed to find file to save to for JSONEditor panel.");
							}
							else
							{
								save(path.value());
							}
						}
					}

					ImGui::EndMenuBar();
				}

				create_new();

				if (m_loaded)
				{
					m_counter = 0;

					if (m_external)
					{
						if (m_external->is_object())
						{
							do_object(*m_external);
						}
						else if (m_external->is_array())
						{
							do_array(*m_external);
						}
						else
						{
							GALAXY_LOG(GALAXY_FATAL, "JSONEditor was not object or array.");
						}
					}
					else
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
							GALAXY_LOG(GALAXY_FATAL, "JSONEditor was not object or array.");
						}
					}
				}
			}

			ImGui::End();
		}

		const bool JSONEditor::is_loaded() const
		{
			return m_loaded;
		}

		void JSONEditor::do_object(nlohmann::json& json)
		{
			ImGui::Text("{");
			ImGui::Indent(INDENT_PIXELS);

			m_counter++;
			ImGui::PushID(m_counter);

			if (ImGui::Button("New Object"))
			{
				ImGui::OpenPopup("New Object", ImGuiPopupFlags_NoOpenOverExistingPopup);
			}

			new_object(json);
			ImGui::PopID();

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

		void JSONEditor::do_array(nlohmann::json& json)
		{
			ImGui::Text("[");
			ImGui::Indent(INDENT_PIXELS);

			m_counter++;
			ImGui::PushID(m_counter);

			if (ImGui::Button("New Element"))
			{
				ImGui::OpenPopup("New Element", ImGuiPopupFlags_NoOpenOverExistingPopup);
			}

			add_to_array(json);
			ImGui::PopID();

			unsigned int counter = 0;
			std::string name     = "";
			for (auto& elem : json)
			{
				name = fmt::format("[{0}]", counter);
				if (elem.is_array())
				{
					do_array(elem);
				}
				else if (elem.is_boolean())
				{
					ImGui::ToggleButton(name.c_str(), elem.get<bool*>());
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

		void JSONEditor::new_object(nlohmann::json& json)
		{
			if (ImGui::BeginPopup("New Object", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar))
			{
				static std::string s_key_str = "";
				static std::string s_val_str = "";
				static std::string s_err_str = "";
				static int s_index           = 0;
				static bool s_show_error     = false;

				// clang-format off
				static const std::vector<const char*> s_types =
				{
					"...",
					"bool",
					"integer",
					"unsigned",
					"float",
					"string",
					"object",
					"array"
				};
				// clang-format on

				ImGui::InputText("Key", &s_key_str, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
				ImGui::SameLine();
				ImGui::InputText("Value", &s_val_str, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
				ImGui::Combo("Type", &s_index, s_types.data(), s_types.size());

				if (ImGui::Button("Add"))
				{
					if (s_key_str.empty() || s_val_str.empty())
					{
						s_err_str    = "Input fields cannot be empty.";
						s_show_error = true;
					}
					else
					{
						if (s_types[s_index] == "bool")
						{
							if (s_val_str == "true")
							{
								json[s_key_str] = true;
								s_show_error    = false;
							}
							else if (s_val_str == "false")
							{
								json[s_key_str] = false;
								s_show_error    = false;
							}
							else
							{
								s_err_str    = "Boolean string must be \"true\" or \"false\".";
								s_show_error = true;
							}
						}
						else if (s_types[s_index] == "integer")
						{
							json[s_key_str] = std::stoi(s_val_str);
							s_show_error    = false;
						}
						else if (s_types[s_index] == "unsigned")
						{
							json[s_key_str] = static_cast<unsigned int>(std::stoi(s_val_str));
							s_show_error    = false;
						}
						else if (s_types[s_index] == "float")
						{
							json[s_key_str] = std::stof(s_val_str);
							s_show_error    = false;
						}
						else if (s_types[s_index] == "string")
						{
							json[s_key_str] = s_val_str;
							s_show_error    = false;
						}
						else if (s_types[s_index] == "object")
						{
							json[s_key_str] = nlohmann::json::object();
							s_show_error    = false;
						}
						else if (s_types[s_index] == "array")
						{
							json.push_back(nlohmann::json::array());
							s_show_error = false;
						}
						else
						{
							s_err_str    = "No type selected.";
							s_show_error = true;
						}

						if (!s_show_error)
						{
							s_key_str    = "";
							s_val_str    = "";
							s_err_str    = "";
							s_index      = 0;
							s_show_error = false;

							ImGui::CloseCurrentPopup();
						}
					}
				}

				if (s_show_error)
				{
					ImGui::Text(s_err_str.c_str());
				}

				ImGui::EndPopup();
			}
		}

		void JSONEditor::add_to_array(nlohmann::json& json)
		{
			if (ImGui::BeginPopup("New Element", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar))
			{
				static std::string s_val_str = "";
				static std::string s_err_str = "";
				static int s_index           = 0;
				static bool s_show_error     = false;

				// clang-format off
				static const std::vector<const char*> s_types =
				{
					"...",
					"bool",
					"integer",
					"unsigned",
					"float",
					"string",
					"object",
					"array"
				};
				// clang-format on

				ImGui::InputText("Value", &s_val_str, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
				ImGui::Combo("Type", &s_index, s_types.data(), s_types.size());

				if (ImGui::Button("Add"))
				{
					if (s_val_str.empty())
					{
						s_err_str    = "Input fields cannot be empty.";
						s_show_error = true;
					}
					else
					{
						if (s_types[s_index] == "bool")
						{
							if (s_val_str == "true")
							{
								json.push_back(true);
								s_show_error = false;
							}
							else if (s_val_str == "false")
							{
								json.push_back(false);
								s_show_error = false;
							}
							else
							{
								s_err_str    = "Boolean string must be \"true\" or \"false\".";
								s_show_error = true;
							}
						}
						else if (s_types[s_index] == "integer")
						{
							json.push_back(std::stoi(s_val_str));
							s_show_error = false;
						}
						else if (s_types[s_index] == "unsigned")
						{
							json.push_back(static_cast<unsigned int>(std::stoi(s_val_str)));
							s_show_error = false;
						}
						else if (s_types[s_index] == "float")
						{
							json.push_back(std::stof(s_val_str));
							s_show_error = false;
						}
						else if (s_types[s_index] == "string")
						{
							json.push_back(s_val_str);
							s_show_error = false;
						}
						else if (s_types[s_index] == "object")
						{
							json.push_back(nlohmann::json::object());
							s_show_error = false;
						}
						else if (s_types[s_index] == "array")
						{
							json.push_back(nlohmann::json::array());
							s_show_error = false;
						}
						else
						{
							s_err_str    = "No type selected.";
							s_show_error = true;
						}

						if (!s_show_error)
						{
							s_val_str    = "";
							s_err_str    = "";
							s_index      = 0;
							s_show_error = false;

							ImGui::CloseCurrentPopup();
						}
					}
				}

				if (s_show_error)
				{
					ImGui::Text(s_err_str.c_str());
				}

				ImGui::EndPopup();
			}
		}
	} // namespace panel
} // namespace sc