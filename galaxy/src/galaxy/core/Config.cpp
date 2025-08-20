///
/// Config.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/fs/FileUtils.hpp"

#include "Config.hpp"

namespace galaxy
{
	Config::Config() noexcept
	{
		m_config = "{}"_json;
	}

	Config::Config(std::string_view file) noexcept
	{
		m_config = "{}"_json;
		load(file);
	}

	Config::~Config() noexcept
	{
		m_config.clear();
	}

	void Config::load(std::string_view file)
	{
		auto path = std::filesystem::path(file);
		m_path    = path.string();

		// The first time we load, if there is no config file, we save the default file
		// and use that instead.
		if (!std::filesystem::exists(path))
		{
			const auto result = fileutils::write(m_path, m_config.dump(4));
			if (result != std::nullopt)
			{
				result.value().log();
			}
		}
		else
		{
			const auto result = fileutils::read(m_path);
			if (result)
			{
				m_config = nlohmann::json::parse(result.value());
			}
			else
			{
				result.error().log();
			}
		}
	}

	void Config::save()
	{
		const auto result = fileutils::write(m_path, m_config.dump(4));
		if (result != std::nullopt)
		{
			result.value().log();
		}
	}

	bool Config::has(const std::string& key) noexcept
	{
		return m_config.contains(key);
	}

	bool Config::has(const std::string& key, const std::string& section, const std::string& delim)
	{
		const auto sections = str::split(section, delim);
		if (sections.empty())
		{
			// Single section.
			const auto& root = m_config;
			if (root.contains(section))
			{
				return root[section].contains(key);
			}
		}
		else
		{
			// Multiple sections.
			nlohmann::json* leaf = &m_config;
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

		return false;
	}

	bool Config::empty() const
	{
		return m_config.empty();
	}

	void Config::raw(const nlohmann::json& json) noexcept
	{
		m_config = json;
	}

	const nlohmann::json& Config::raw() const noexcept
	{
		return m_config;
	}
} // namespace galaxy
