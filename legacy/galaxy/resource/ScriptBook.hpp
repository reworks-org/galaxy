///
/// ScriptBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_SCRIPTBOOK_HPP_
#define GALAXY_RESOURCE_SCRIPTBOOK_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/resource/ResourceCache.hpp"
#include "galaxy/scripting/LoadedScript.hpp"

namespace galaxy
{
	namespace res
	{
		///
		/// Resource manager for lua scripts.
		///
		class ScriptBook final : public ResourceCache<lua::LoadedScript>, public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			ScriptBook() noexcept = default;

			///
			/// JSON constructor.
			///
			/// \param file JSON file to load.
			///
			ScriptBook(std::string_view file);

			///
			/// Destructor.
			///
			virtual ~ScriptBook() noexcept;

			///
			/// Create ScriptBook from JSON.
			///
			/// \param file JSON file to load.
			///
			void create_from_json(std::string_view file);

			///
			/// Run a script.
			///
			/// \param script_id ID of the script to run.
			///
			void run(std::string_view script_id);

			///
			/// Add a definition script.
			///
			/// \param script Name of the script, including extension.
			///
			void add_definition_script(std::string_view script);

			///
			/// Remove a definition script.
			///
			/// \param script Name of the script, including extension.
			///
			void remove_definition_script(std::string_view script);

			///
			/// Clean up.
			///
			void clear() noexcept override;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			ScriptBook(const ScriptBook&) = delete;

			///
			/// Move constructor.
			///
			ScriptBook(ScriptBook&&) = delete;

			///
			/// Copy assignment operator.
			///
			ScriptBook& operator=(const ScriptBook&) = delete;

			///
			/// Move assignment operator.
			///
			ScriptBook& operator=(ScriptBook&&) = delete;

			///
			/// Definition Scripts.
			///
			std::vector<std::string> m_definition_scripts;
		};
	} // namespace res
} // namespace galaxy

#endif