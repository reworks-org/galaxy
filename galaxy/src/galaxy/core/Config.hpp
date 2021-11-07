///
/// Config.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_CONFIG_HPP_
#define GALAXY_CORE_CONFIG_HPP_

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace core
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
			Config();

			///
			/// \brief Argument constructor.
			///
			/// Calls load().
			///
			/// \param file Path to config to open / use (does not matter if does not exist). This is relative to the app.
			///
			Config(std::string_view file);

			///
			/// Destructor.
			///
			~Config() noexcept;

			///
			/// \brief Checks if config exists and flags if a config needs to be created.
			///
			/// Also sets up filepath and json.
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
			void set(std::string_view key, const Value& value);

			///
			/// Retrieve a config value.
			///
			/// \tparam Value Standard JSON type to use.
			///
			/// \param key Name of the value to retrieve.
			///
			/// \return Returns the value retrieved from the key.
			///
			template<meta::standard_type Value>
			[[nodiscard]] std::optional<Value> get(std::string_view key);

			///
			/// Get a section as JSON.
			///
			/// \param key Name of the section to retrieve.
			///
			/// \return Json object. Could also be a value if not a section.
			///
			[[nodiscard]] nlohmann::json get_section(std::string_view key);

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
			/// Config loaded flag.
			///
			bool m_loaded;

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
		inline void Config::set(std::string_view key, const Value& value)
		{
			if (m_loaded)
			{
				m_config[key] = value;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to set value of an unloaded config file.");
			}
		}

		template<meta::standard_type Value>
		inline std::optional<Value> Config::get(std::string_view key)
		{
			if (m_loaded)
			{
				if (m_config.count(key) > 0)
				{
					return std::make_optional(m_config[key].get<Value>());
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Attempted to retrieve value from key: '{0}' that does not exist.", key);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to get value of an unloaded config file.");
			}

			return std::nullopt;
		}
	} // namespace core
} // namespace galaxy

#endif