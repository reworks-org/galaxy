///
/// Config.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_CONFIG_HPP_
#define GALAXY_CORE_CONFIG_HPP_

#include <nlohmann/json.hpp>

#include "galaxy/logging/Log.hpp"
#include "galaxy/meta/Concepts.hpp"
#include "galaxy/utils/StringUtils.hpp"

namespace galaxy
{
	///
	/// Allows you to read, write and manipulate JSON config files.
	///
	class Config final
	{
	public:
		///
		/// Constructor.
		///
		Config() noexcept;

		///
		/// \brief Argument constructor.
		///
		/// Calls load().
		///
		/// \param file Path to config to open / use (does not matter if does not exist). This is relative to the app.
		///
		Config(std::string_view file) noexcept;

		///
		/// Destructor.
		///
		~Config() noexcept;

		///
		/// \brief Loads config json from disk.
		///
		/// Will override any existing configuration.
		///
		/// \param file Path to config to open / use (does not matter if does not exist). This is relative to the app.
		///
		void load(std::string_view file);

		///
		/// Save the config file. Must be opened first.
		///
		void save();

		///
		/// \brief Set a new key-value pair for the config.
		///
		/// Will create if not exist, otherwise update.
		///
		/// \tparam Value Standard JSON type to use.
		///
		/// \param key Name of the variable.
		/// \param value The variable value to set.
		///
		template<meta::standard_type Value>
		void set(const std::string& key, const Value& value) noexcept;

		///
		/// \brief Sets a new key-value pair in a section. Can use a delimiter to seperate sections.
		///
		/// Will create if not exist, otherwise update.
		///
		/// \tparam Value Standard JSON type to use.
		///
		/// \param key Name of the variable.
		/// \param value The variable value to set.
		/// \param section Section of the config file to use. Can use a delimiter to seperate sections.
		/// \param delim Delimiter to seperate sections with. Optional.
		///
		template<meta::standard_type Value>
		void set(const std::string& key, const Value& value, const std::string& section, const std::string& delim = ".");

		///
		/// Sets a new setting, only if key or value does not already exist.
		///
		/// \tparam Value Standard JSON type to use.
		///
		/// \param key Name of the variable.
		/// \param value The variable value to set.
		///
		template<meta::standard_type Value>
		void restore(const std::string& key, const Value& value) noexcept;

		///
		/// Sets a new setting, only if key or value does not already exist.
		///
		/// \tparam Value Standard JSON type to use.
		///
		/// \param key Name of the variable.
		/// \param value The variable value to set.
		/// \param section Section of the config file to use. Can use a delimiter to seperate sections.
		/// \param delim Delimiter to seperate sections with. Optional.
		///
		template<meta::standard_type Value>
		void restore(const std::string& key, const Value& value, const std::string& section, const std::string& delim = ".");

		///
		/// Check if the config file actually has a value.
		///
		/// \param key Name of the value to check.
		///
		/// \return True if the value exists.
		///
		[[nodiscard]]
		bool has(const std::string& key) noexcept;

		///
		/// Check if the config file actually has a value.
		///
		/// \param key Key to lookup value.
		/// \param section Delimiter seperated sections.
		/// \param delim Delimiter to use. Optional.
		///
		/// \return True if the value exists.
		///
		[[nodiscard]]
		bool has(const std::string& key, const std::string& section, const std::string& delim = ".");

		///
		/// Retrieve a root config value.
		///
		/// \tparam Value Standard JSON type to use.
		///
		/// \param key Name of the value to retrieve.
		///
		/// \return Returns the value retrieved from the key.
		///
		template<meta::standard_type Value>
		[[nodiscard]]
		std::optional<Value> get(const std::string& key);

		///
		/// Retrieve a config value in a section.
		///
		/// \tparam Value Standard JSON type to use.
		///
		/// \param key Key to lookup value.
		/// \param section Delimiter seperated sections.
		/// \param delim Delimiter to use. Optional.
		///
		/// \return Returns the value retrieved from the key.
		///
		template<meta::standard_type Value>
		[[nodiscard]]
		std::optional<Value> get(const std::string& key, const std::string& section, const std::string& delim = ".");

		///
		/// Is the config file blank.
		///
		/// \return True if theres no config data.
		///
		[[nodiscard]]
		bool empty() const;

		///
		/// Set as external json object.
		///
		/// \param json New json config data to set.
		///
		void raw(const nlohmann::json& json) noexcept;

		///
		/// Get raw json object.
		///
		/// \return Const reference to internal json object.
		///
		[[nodiscard]]
		const nlohmann::json& raw() const noexcept;

	private:
		///
		/// Copy constructor.
		///
		Config(const Config&) = delete;

		///
		/// Move constructor.
		///
		Config(Config&&) = delete;

		///
		/// Copy assignment operator.
		///
		Config& operator=(const Config&) = delete;

		///
		/// Move assignment operator.
		///
		Config& operator=(Config&&) = delete;

	private:
		///
		/// Config file as JSON.
		///
		nlohmann::json m_config;

		///
		/// Filepath.
		///
		std::string m_path;
	};

	template<meta::standard_type Value>
	inline void Config::set(const std::string& key, const Value& value) noexcept
	{
		m_config[key] = value;
	}

	template<meta::standard_type Value>
	inline void Config::set(const std::string& key, const Value& value, const std::string& section, const std::string& delim)
	{
		const auto sections = str::split(section, delim);
		if (sections.empty())
		{
			// Single section.
			m_config[section][key] = value;
		}
		else
		{
			// Multiple sections.
			nlohmann::json* leaf = &m_config;
			for (const auto& sec : sections)
			{
				if (!leaf->contains(sec))
				{
					(*leaf)[sec] = nlohmann::json::object();
				}

				leaf = &leaf->at(sec);
			}

			(*leaf)[key] = value;
		}
	}

	template<meta::standard_type Value>
	inline void Config::restore(const std::string& key, const Value& value) noexcept
	{
		if (!has(key))
		{
			set(key, value);
		}
	}

	template<meta::standard_type Value>
	inline void Config::restore(const std::string& key, const Value& value, const std::string& section, const std::string& delim)
	{
		if (!has(key, section, delim))
		{
			set(key, value, section, delim);
		}
	}

	template<meta::standard_type Value>
	inline std::optional<Value> Config::get(const std::string& key)
	{
		const auto& section = m_config;

		if (section.contains(key))
		{
			return section[key].get<Value>();
		}

		return std::nullopt;
	}

	template<meta::standard_type Value>
	inline std::optional<Value> Config::get(const std::string& key, const std::string& section, const std::string& delim)
	{
		const auto sections = str::split(section, delim);
		if (sections.empty())
		{
			// Single section.
			const auto& root = m_config;
			if (root.contains(section))
			{
				const auto& obj = root[section];
				if (obj.contains(key))
				{
					return obj[key].get<Value>();
				}
			}

			return std::nullopt;
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
				return (*leaf)[key].get<Value>();
			}

			return std::nullopt;
		}
	}
} // namespace galaxy

#endif
