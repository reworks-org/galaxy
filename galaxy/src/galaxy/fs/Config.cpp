///
/// Config.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "Config.hpp"

namespace galaxy
{
	namespace fs
	{
		Config::Config() noexcept
			: m_loaded {false}
			, m_blank {true}
		{
		}

		void Config::init(std::string_view file)
		{
			const auto path = SL_HANDLE.vfs()->absolute(file);
			if (path == std::nullopt)
			{
				m_path   = file;
				m_config = nlohmann::json::object();
				m_blank  = true;
				m_loaded = true;

				save();
			}
			else
			{
				m_path            = path.value();
				const auto result = json::parse_from_disk(m_path);
				if (result == std::nullopt)
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to parse json from disk for config file: {0}.", file);
				}
				else
				{
					m_config = result.value();
					m_blank  = false;
					m_loaded = true;
				}
			}
		}

		void Config::save()
		{
			if (m_loaded)
			{
				if (!json::save_to_disk(m_path, m_config))
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to save json to disk using path: {0}.", m_path);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to save config that was not open.");
			}
		}

		const bool Config::is_blank() const noexcept
		{
			return m_blank;
		}
	} // namespace fs
} // namespace galaxy