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
		Config::Config()
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

		Config::~Config()
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
				}
			}

			m_loaded = true;
		}

		void Config::save()
		{
			if (m_loaded)
			{
				std::ofstream ofs {m_path, std::ofstream::out | std::ofstream::trunc};

				if (ofs.good())
				{
					ofs << m_config.dump(4);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to save config file to disk.");
				}

				ofs.close();
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to save config that was not loaded.");
			}
		}

		bool Config::has(const std::string& key)
		{
			if (m_loaded)
			{
				return m_config["config"].contains(key);
			}

			return false;
		}

		bool Config::has(const std::string& key, const std::string& section, const std::string& delim)
		{
			if (m_loaded)
			{
				const auto sections = strutils::split(section, delim);
				if (sections.empty())
				{
					// Single section.
					const auto& root = m_config["config"];
					if (root.contains(section))
					{
						return root[section].contains(key);
					}
				}
				else
				{
					// Multiple sections.
					nlohmann::json* leaf = &m_config["config"];
					for (const auto& sec : sections)
					{
						if (leaf->contains(sec))
						{
							leaf = &leaf->at(sec);
						}
					}

					if (leaf->contains(key))
					{
						return true;
					}
				}
			}

			return false;
		}

		bool Config::empty() const
		{
			if (m_loaded)
			{
				return m_config.at("config").empty();
			}

			return true;
		}

		void Config::raw(const nlohmann::json& json)
		{
			m_config = json;
		}

		const nlohmann::json& Config::raw() const
		{
			return m_config;
		}
	} // namespace core
} // namespace galaxy