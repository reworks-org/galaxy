///
/// UIScript.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_UISCRIPT_HPP_
#define GALAXY_COMPONENTS_UISCRIPT_HPP_

#include <sol/sol.hpp>

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Script for running an active UI.
		///
		class UIScript final : public fs::Serializable
		{
		  public:
			///
			/// Constructor.
			///
			UIScript();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			UIScript(const nlohmann::json& json);

			///
			/// Pointer constructor.
			///
			/// \param ptr Data to COPY into this component.
			///
			UIScript(UIScript* ptr);

			///
			/// Move constructor.
			///
			UIScript(UIScript&&);

			///
			/// Move assignment operator.
			///
			UIScript& operator=(UIScript&&);

			///
			/// Destructor.
			///
			virtual ~UIScript();

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
			/// Load script and set context.
			///
			/// \param file Script file to load.
			///
			void load(const std::string& file);

			///
			/// Copy assignment operator.
			///
			UIScript& operator=(const UIScript&) = delete;

			///
			/// Copy constructor.
			///
			UIScript(const UIScript&) = delete;

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
