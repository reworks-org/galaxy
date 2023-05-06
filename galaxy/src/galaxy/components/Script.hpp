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

namespace galaxy
{
	namespace components
	{
		///
		/// High level abstraction of a lua script.
		///
		class Script final : public fs::Serializable
		{
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
			/// Load a script.
			///
			/// \param file File in VFS to load as a script.
			///
			void load(std::string_view file);

			///
			/// \brief Reloads script from file.
			///
			/// Only works if load() has already been called.
			///
			void reload();

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
		};
	} // namespace components
} // namespace galaxy

#endif