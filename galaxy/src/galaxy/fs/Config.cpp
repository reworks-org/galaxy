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

	void Config::init(const std::string& file)
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

	bool Config::open()
	{
		bool result = true;

		if (m_exists)
		{
			// Makes sure the filepath is correct for the current platform.
			std::fstream ifstream(m_path.string(), std::fstream::in);
			if (ifstream.fail())
			{
				result = false;
				PL_LOG(pl::Log::Level::ERROR, "std::ifstream failed to open config file!");
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
			PL_LOG(pl::Log::Level::WARNING, "Config file must be defined and then created.");
		}

		return result;
	}

	void Config::create()
	{
		if (m_config.empty())
		{
			PL_LOG(pl::Log::Level::FATAL, "Attempted to create empty config file!");
			throw std::runtime_error("");
		}
		else
		{
			save();
			m_exists = true;
		}
	}

	void Config::save()
	{
		std::ofstream ofstream(m_path.string(), std::ofstream::out | std::ofstream::trunc);
		if (ofstream.fail())
		{
			PL_LOG(pl::Log::Level::ERROR, "std::ofstream failed to open config file!");
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