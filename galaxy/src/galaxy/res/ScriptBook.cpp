///
/// ScriptBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

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

		void ScriptBook::clear() noexcept
		{
			m_resources.clear();
		}

		nlohmann::json ScriptBook::serialize()
		{
			nlohmann::json json = "{\"scriptbook\":{}"_json;

			for (const auto& [name, script] : m_resources)
			{
				json["scriptbook"][name] = script->m_filename;
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
		}
	} // namespace res
} // namespace galaxy