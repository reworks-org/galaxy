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
		    : m_loaded {false}, m_blank {true}
		{
		}

		void Config::init(std::string_view file)
		{
			m_path = SL_HANDLE.vfs()->absolute(file);
			if (m_path.empty())
			{
				m_path   = file;
				m_config = nlohmann::json::object();
				m_blank  = true;

				save();
			}
			else
			{
				m_config = json::parse_from_disk(m_path);
				m_blank  = false;
			}

			m_loaded = true;
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

		const bool Config::is_blank() const noexcept
		{
			return m_blank;
		}
	} // namespace fs
} // namespace galaxy