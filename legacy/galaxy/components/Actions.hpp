///
/// Actions.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_ACTIONS_HPP_
#define GALAXY_COMPONENTS_ACTIONS_HPP_

#include <robin_hood.h>

#include "galaxy/input/Keys.hpp"
#include "galaxy/input/MouseButtons.hpp"
#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Component data is called when entity is collided.
		///
		class Actions final : public fs::Serializable
		{
			using KeyActionMap   = robin_hood::unordered_flat_map<input::Keys, std::string>;
			using MouseActionMap = robin_hood::unordered_flat_map<input::MouseButtons, std::string>;

		public:
			///
			/// Constructor.
			///
			Actions() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Actions(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Actions(Actions&&) noexcept;

			///
			/// Move assignment operator.
			///
			Actions& operator=(Actions&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Actions() noexcept = default;

			///
			/// \brief Helper function to add key action.
			///
			/// Mainly for Lua, not required.
			///
			/// \param key Key to bind lua function to.
			/// \param func Lua function to call.
			///
			void add_key_action(const input::Keys key, std::string_view func) noexcept;

			///
			/// \brief Helper function to add mouse button action.
			///
			/// Mainly for Lua, not required.
			///
			/// \param mouse_button Mouse button to bind lua function to.
			/// \param func Lua function to call.
			///
			void add_mouse_action(const input::MouseButtons mouse_button, std::string_view func) noexcept;

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
			Actions& operator=(const Actions&) = delete;

			///
			/// Copy constructor.
			///
			Actions(const Actions&) = delete;

		public:
			///
			/// A lua function bound to a key.
			///
			KeyActionMap m_key_actions;

			///
			/// A lua function bound to a mouse button.
			///
			MouseActionMap m_mouse_actions;
		};
	} // namespace components
} // namespace galaxy

#endif