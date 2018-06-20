///
/// Sol2ImguiAutoImpl.hpp
/// starlight
///
/// Created by reworks on 20/04/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SOL2IMGUIAUTOIMPL_HPP_
#define STARLIGHT_SOL2IMGUIAUTOIMPL_HPP_

#include "sl/libs/sol2/sol.hpp"
#include "sl/libs/imgui/auto/core.h"
#include "sl/libs/loguru/loguru.hpp"

///
/// Binds Sol2 to ImGui::Auto() function.
///
#pragma region SOL2
	IMGUI_AUTO_DEFINE_BEGIN(template<>, sol::table)
		var.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			// Loop over each type until the basic type and call ImGui::Auto() on it in order to
			// build up the object.
			ImGui::Auto(pair.first.as<std::string>(), "Name:");
			
			switch (pair.second.get_type())
			{
			case sol::type::boolean:
				ImGui::Auto(pair.second.as<bool>(), "Value:");
				break;

			case sol::type::lua_nil:
				LOG_S(WARNING) << "Nill value for: " << pair.first.as<const char*>();
				break;

			case sol::type::nil:
				LOG_S(WARNING) << "Nill value for: " << pair.first.as<const char*>();
				break;

			case sol::type::none:
				LOG_S(WARNING) << "No value for: " << pair.first.as<const char*>();
				break;

			case sol::type::number:
				ImGui::Auto(pair.second.as<double>(), "Value:");
				break;

			case sol::type::string:
				ImGui::Auto(pair.second.as<std::string>(), "Value:");
				break;

			case sol::type::table:
				ImGui::Auto(pair.second.as<sol::table>(), "Value:");
				break;

			default:
				LOG_S(WARNING) << "Unknown type! Please stick to using basic types for now.";
				break;
			}
		});
	IMGUI_AUTO_DEFINE_END

#pragma endregion

#endif