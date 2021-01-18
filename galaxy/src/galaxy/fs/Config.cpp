///
/// Config.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "Config.hpp"

namespace galaxy
{
	namespace fs
	{
		Config::Config() noexcept
		    : m_loaded {false}
		{
		}

		void Config::init(std::string_view file)
		{
			m_loaded = true;

			m_path = static_cast<std::string>(file);
			if (!std::filesystem::exists(m_path))
			{
				m_config = nlohmann::json::object();
				save();
			}
			else
			{
				m_config = json::parse_from_disk(m_path);
			}
		}

		void Config::save()
		{
			if (m_loaded)
			{
				json::save_to_disk(m_path, m_config);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to save config that was not open.");
			}
		}
	} // namespace fs
} // namespace galaxy