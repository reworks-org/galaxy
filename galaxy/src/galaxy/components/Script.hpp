///
/// Script.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SCRIPT_HPP_
#define GALAXY_COMPONENTS_SCRIPT_HPP_

#include <sol/sol.hpp>

#include "galaxy/fs/Serializable.hpp"

namespace sc
{
	namespace panel
	{
		class EntityEditor;
	} // namespace panel
} // namespace sc

namespace galaxy
{
	namespace components
	{
		///
		/// High level abstraction of a lua script.
		///
		class Script final : public fs::Serializable
		{
			friend class sc::panel::EntityEditor;

		public:
			////
			/// Constructor.
			///
			Script();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Script(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Script(Script&&);

			///
			/// Move assignment operator.
			///
			Script& operator=(Script&&);

			///
			/// Destructor.
			///
			virtual ~Script();

			///
			/// Get script file path.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& file() const;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
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
			/// Load a script.
			///
			/// \param file File in VFS to load as a script.
			///
			void load_internal(std::string_view file);

		public:
			///
			/// The script object (table) returned by a lua script.
			///
			sol::table m_self;

			///
			/// The update function belonging to the lua table.
			///
			sol::function m_update;

		private:
			///
			/// Script file.
			///
			std::string m_file;

			///
			/// For editor. Show functions.
			///
			bool m_show_functions = false;

			///
			/// For editor. Shows userdata types.
			///
			bool m_show_userdata = false;

			///
			/// For editor. Show unknown type data.
			///
			bool m_show_unknown = false;
		};
	} // namespace components
} // namespace galaxy

#endif