///
/// ScriptBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>
#include <execution>

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "ScriptBook.hpp"

namespace galaxy
{
	namespace res
	{
		ScriptBook::ScriptBook(std::string_view file)
			: Serializable {this}
		{
			create_from_json(file);
		}

		ScriptBook::~ScriptBook() noexcept
		{
			clear();
		}

		void ScriptBook::create_from_json(std::string_view file)
		{
			const auto json_opt = json::parse_from_disk(file);
			if (json_opt == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create parse/load json file: {0}, for ScriptBook.", file);
			}
			else
			{
				deserialize(json_opt.value());
			}
		}

		void ScriptBook::run(std::string_view script_id)
		{
			const auto str = static_cast<std::string>(script_id);
			SL_HANDLE.lua()->script(m_resources[str]->m_code);
		}

		void ScriptBook::add_definition_script(std::string_view script)
		{
			m_definition_scripts.emplace_back(static_cast<std::string>(script));
		}

		void ScriptBook::remove_definition_script(std::string_view script)
		{
			m_definition_scripts.erase(
				std::remove(std::execution::par, m_definition_scripts.begin(), m_definition_scripts.end(), static_cast<std::string>(script)),
				m_definition_scripts.end());
		}

		void ScriptBook::clear() noexcept
		{
			m_resources.clear();
			m_definition_scripts.clear();
		}

		nlohmann::json ScriptBook::serialize()
		{
			nlohmann::json json = "{\"scriptbook\":{}"_json;

			for (const auto& [name, script] : m_resources)
			{
				json["scriptbook"][name] = script->m_filename;
			}

			json["definitions"] = nlohmann::json::array();

			for (const auto& script : m_definition_scripts)
			{
				json["definitions"].push_back(script);
			}

			return json;
		}

		void ScriptBook::deserialize(const nlohmann::json& json)
		{
			clear();

			for (const auto& [name, script] : json.at("scriptbook").items())
			{
				create(name, script);
			}

			for (const auto& script : json.at("definitions"))
			{
				std::string file = script.get<std::string>();
				m_definition_scripts.push_back(file);

				const auto code = SL_HANDLE.vfs()->open(file);
				if (code == std::nullopt)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load definition script: {0}.", file);
				}
				else
				{
					SL_HANDLE.lua()->script(code.value());
				}
			}
		}
	} // namespace res
} // namespace galaxy