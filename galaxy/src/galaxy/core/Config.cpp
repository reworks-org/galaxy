///
/// Config.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/scripting/JSON.hpp"

#include "Config.hpp"

namespace galaxy
{
	namespace core
	{
		Config::Config() noexcept
			: m_loaded {false}
		{
			m_config = "{\"config\":{}}"_json;
		}

		Config::Config(std::string_view file)
			: m_loaded {false}
		{
			m_config = "{\"config\":{}}"_json;

			load(file);
		}

		Config::~Config() noexcept
		{
			m_loaded = false;
			m_config.clear();
		}

		void Config::load(std::string_view file)
		{
			auto& fs        = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			const auto path = fs.find(file);

			if (path.m_code == fs::FileInfo::Code::NOT_FOUND)
			{
				if (!json::save_to_disk(path.m_string, m_config))
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to save json to disk using path: {0}.", path.m_string);
				}
			}

			const auto result = json::parse_from_disk(path.m_string);

			if (result.has_value())
			{
				m_config = result.value();
				m_loaded = true;
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to config file: {0}.", path.m_string);
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
				GALAXY_LOG(GALAXY_WARNING, "Attempted to save config that was not loaded.");
			}
		}

		bool Config::empty() noexcept
		{
			if (m_loaded)
			{
				return m_config.at("config").empty();
			}

			return true;
		}
	} // namespace core
} // namespace galaxy