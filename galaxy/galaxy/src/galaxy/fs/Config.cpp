///
/// Config.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include "Config.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Config::Config() noexcept
		:m_opened(false), m_exists(false)
	{
	}

	Config::~Config() noexcept
	{
		// Save on exit.
		save();
	}

	void Config::init(const std::string& file) noexcept
	{
		m_path = std::filesystem::path(file);
		if (!std::filesystem::exists(m_path))
		{
			m_exists = false;
		}
		else
		{
			m_exists = true;
		}
	}

	bool Config::open() noexcept
	{
		bool result = true;

		if (m_exists)
		{
			// Makes sure the filepath is correct for the current platform.
			std::fstream ifstream(m_path.string(), std::fstream::in);
			if (ifstream.fail())
			{
				result = false;
				PL_LOG(PL_ERROR, "std::ifstream failed to open config file!");
			}
			else
			{
				// Use JSON stream to deserialize data and parse.
				ifstream >> m_config;
				m_opened = true;
			}

			ifstream.close();
		}
		else
		{
			result = false;
			PL_LOG(PL_WARNING, "Config file must be defined and then created.");
		}

		return result;
	}

	void Config::create() noexcept
	{
		if (m_config.empty())
		{
			PL_LOG(PL_FATAL, "Attempted to create empty config file!");
		}
		else
		{
			save();
			m_exists = true;
		}
	}

	void Config::save() noexcept
	{
		std::ofstream ofstream(m_path.string(), std::ofstream::out | std::ofstream::trunc);
		if (ofstream.fail())
		{
			PL_LOG(PL_ERROR, "std::ofstream failed to open config file!");
		}
		else
		{
			// Use JSON stream to serialize data and write to file.
			ofstream << m_config.dump(4);
			m_opened = true;
		}

		ofstream.close();
	}
}