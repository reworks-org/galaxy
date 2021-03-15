///
/// ScriptBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

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
				const auto& json = json_opt.value();
				for (auto& [name, script] : json.at("scriptbook").items())
				{
					const auto code = SL_HANDLE.vfs()->open(script);
					if (code == std::nullopt)
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to load script: {0}.", script);
					}
					else
					{
						create(name, code.value());
					}
				}
			}
		}

		void ScriptBook::clear() noexcept
		{
			m_resources.clear();
		}
	} // namespace res
} // namespace galaxy