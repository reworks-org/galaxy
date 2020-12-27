///
/// Config.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_CONFIG_HPP_
#define GALAXY_FS_CONFIG_HPP_

#include <filesystem>

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace fs
	{
		///
		/// Allows you to read, write and manipulate JSON config files.
		///
		class Config final
		{
		public:
			///
			/// Default constructor.
			///
			Config();

			///
			/// Clean up the config reader.
			///
			~Config() = default;

			///
			/// \brief Checks if config exists and flags if a config needs to be created.
			///
			/// Also sets up filepath and json.
			///
			/// \param file Path to config to open / use (does not matter if does not exist).
			///
			void init(std::string_view file);

			///
			/// \brief Open a config file for reading and writing.
			///
			/// \param file Path to file to open.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] bool open();

			///
			/// Define a default key-value pair for the config file.
			///
			/// \param key Name of the variable.
			/// \param value The variable value to set.
			///
			template<meta::standard_type Value>
			void define(std::string_view key, const Value value);

			///
			/// Change the value in an existing config file.
			///
			/// \param key Name of the variable.
			/// \param value The variable value to set.
			///
			template<meta::standard_type Value>
			void change(std::string_view key, const Value value);

			///
			/// \brief Creates the actual config file in the FS.
			//
			/// Also checks that there is actually data in the config file.
			///
			void create();

			///
			/// Save the config file. Must be opened first.
			///
			/// \return True if successful.
			///
			void save();

			///
			/// Retrieve a config value.
			///
			/// \param key Name of the value to retrieve.
			///
			/// \return Returns the value retrieved from the key.
			///
			template<meta::standard_type Value>
			[[nodiscard]] Value get(std::string_view key);

		private:
			///
			/// Has the file been opened.
			///
			bool m_opened;

			///
			/// Does the file exist.
			///
			bool m_exists;

			///
			/// Config file as JSON.
			///
			nlohmann::json m_config;

			///
			/// Filepath.
			///
			std::filesystem::path m_path;
		};

		template<meta::standard_type Value>
		inline void Config::define(std::string_view key, const Value value)
		{
			// Only need to set definitions first time around.
			if (!m_exists)
			{
				const auto str = static_cast<std::string>(key);
				m_config[str]  = value;
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to define value of an unloaded config file.");
			}
		}

		template<meta::standard_type Value>
		inline void Config::change(std::string_view key, const Value value)
		{
			if (m_config.count("key") > 0)
			{
				m_config[key] = value;
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to change non-existant config value.");
			}
		}

		template<meta::standard_type Value>
		inline Value Config::get(std::string_view key)
		{
			auto str = static_cast<std::string>(key);

			if (!m_opened)
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to retrieve value from config that was not open!");
			}

			return m_config[str].get<Value>();
		}
	} // namespace fs
} // namespace galaxy

#endif