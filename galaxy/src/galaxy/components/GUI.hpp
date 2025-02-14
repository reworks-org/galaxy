///
/// GUI.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_GUI_HPP_
#define GALAXY_COMPONENTS_GUI_HPP_

#include <sol/sol.hpp>

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Script for running an active UI.
		///
		class GUI final : public fs::Serializable
		{
		  public:
			///
			/// Constructor.
			///
			GUI();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			GUI(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			GUI(GUI&&);

			///
			/// Move assignment operator.
			///
			GUI& operator=(GUI&&);

			///
			/// Destructor.
			///
			virtual ~GUI();

			///
			/// Load script and set context.
			///
			/// \param file Script file to load.
			///
			void load(const std::string& file);

			///
			/// Get script file.
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
			/// Copy assignment operator.
			///
			GUI& operator=(const GUI&) = delete;

			///
			/// Copy constructor.
			///
			GUI(const GUI&) = delete;

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
