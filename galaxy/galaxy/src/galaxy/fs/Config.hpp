///
/// Config.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CONFIG_HPP_
#define GALAXY_CONFIG_HPP_

#include <filesystem>

#include <pulsar/Log.hpp>
#include <nlohmann/json.hpp>

///
/// Core namespace.
///
namespace galaxy
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
		Config() noexcept;

		///
		/// Clean up the config reader.
		///
		~Config() noexcept;

		///
		/// \brief Checks if config exists and flags if a config needs to be created.
		///
		/// Also sets up filepath and json.
		///
		/// \param file Path to config to open / use (does not matter if does not exist).
		///
		void init(const std::string& file) noexcept;

		///
		/// \brief Open a config file for reading and writing.
		///
		/// \param file Path to file to open.
		///
		/// \return True if successful.
		///
		bool open() noexcept;

		///
		/// Define a default key-value pair for the config file.
		///
		/// \param key Name of the variable.
		/// \param value The variable value to set.
		///
		template<typename Type>
		void define(const std::string& key, const Type value) noexcept;

		///
		/// Change the value in an existing config file.
		///
		/// \param key Name of the variable.
		/// \param value The variable value to set.
		///
		template<typename Type>
		void change(const std::string& key, const Type value) noexcept;

		///
		/// \brief Creates the actual config file in the FS.
		//
		/// Also checks that there is actually data in the config file.
		///
		void create() noexcept;

		///
		/// Save the config file. Must be opened first.
		///
		/// \return True if successful.
		///
		void save() noexcept;

		///
		/// Retrieve a config value.
		/// 
		/// \param key Name of the value to retrieve.
		/// 
		/// \return Returns the value retrieved from the key.
		///
		template<typename Value>
		Value get(const std::string& key) noexcept;

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

	template<typename Type>
	inline void Config::define(const std::string& key, const Type value) noexcept
	{
		// Make sure type is floating, integral, boolean or char.
		static_assert(std::is_arithmetic<Type>::value || std::is_same<std::string, Type>::value);
		
		// Only need to set definitions first time around.
		if (!m_exists)
		{
			m_config[key] = value;
		}
	}

	template<typename Type>
	inline void Config::change(const std::string& key, const Type value) noexcept
	{
		// Make sure type is floating, integral, boolean or char.
		static_assert(std::is_arithmetic<Type>::value || std::is_same<std::string, Type>::value);

		if (m_config.count("key") > 0)
		{
			m_config[key] = value;
		}
	}

	template<typename Value>
	inline Value Config::get(const std::string& key) noexcept
	{
		if (!m_opened)
		{
			PL_LOG(PL_FATAL, "Attempted to retrieve value from config that was not open!");
		}

		return m_config[key].get<Value>();
	}
}

#endif