///
/// RMLInput.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_RMLINPUT_HPP_
#define GALAXY_UI_RMLINPUT_HPP_

#include <robin_hood.h>

#include <RmlUi/Core/Input.h>

#include "galaxy/input/InputMods.hpp"
#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// RML <-> Galaxy input.
		///
		class RMLInput final
		{
		public:
			///
			/// Destructor.
			///
			~RMLInput();

			///
			/// Retrieve instance.
			///
			/// \return Returns static reference.
			///
			[[nodiscard]] static RMLInput& handle();

		private:
			///
			/// Constructor.
			///
			RMLInput();

			///
			/// Move constructor.
			///
			RMLInput(RMLInput&&) = delete;

			///
			/// Move assignment operator.
			///
			RMLInput& operator=(RMLInput&&) = delete;

			///
			/// Copy constructor.
			///
			RMLInput(const RMLInput&) = delete;

			///
			/// Copy assignment operator.
			///
			RMLInput& operator=(const RMLInput&) = delete;

		public:
			///
			/// Map of galaxy keys to RML.
			///
			robin_hood::unordered_flat_map<input::Keys, Rml::Input::KeyIdentifier> m_keymap;

			///
			/// Map of galaxy key modifiers to RML.
			///
			robin_hood::unordered_flat_map<input::InputMods, int> m_modmap;
		};
	} // namespace ui
} // namespace galaxy

#endif