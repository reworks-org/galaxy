///
/// Config.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

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
			auto path = std::filesystem::path(file);
			m_path    = path.string();
			if (!std::filesystem::exists(path))
			{
				std::ofstream ofs {m_path, std::ofstream::out | std::ofstream::trunc};

				if (ofs.good())
				{
					ofs << m_config.dump(4);
					ofs.close();
				}
				else
				{
					ofs.close();
					GALAXY_LOG(GALAXY_FATAL, "Failed to save config file to disk.");
				}
			}
			else
			{
				std::ifstream input {m_path, std::ifstream::in};

				if (!input.good())
				{
					input.close();
					GALAXY_LOG(GALAXY_FATAL, "Failed to load config file.");
				}
				else
				{
					input >> m_config;
					input.close();

					m_loaded = true;
				}
			}
		}

		void Config::save()
		{
			if (m_loaded)
			{
				std::ofstream ofs {m_path, std::ofstream::out | std::ofstream::trunc};

				if (ofs.good())
				{
					ofs << m_config.dump(4);
					ofs.close();
				}
				else
				{
					ofs.close();
					GALAXY_LOG(GALAXY_ERROR, "Failed to save config file to disk.");
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