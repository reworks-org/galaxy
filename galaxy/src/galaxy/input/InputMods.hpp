///
/// InputMods.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_INPUTMODS_HPP_
#define GALAXY_INPUT_INPUTMODS_HPP_

#include <GLFW/glfw3.h>

namespace galaxy
{
	namespace input
	{
		///
		/// \brief Modifier key mappings.
		///
		/// Values based on GLFW.
		///
		enum class InputMods : int
		{
			///
			/// Represents an unknown input modifier.
			///
			UNKNOWN = -1,

			///
			/// If this bit is set one or more Shift keys were held down.
			///
			SHIFT = GLFW_MOD_SHIFT,

			///
			/// If this bit is set one or more Control keys were held down.
			///
			CONTROL = GLFW_MOD_CONTROL,

			///
			/// If this bit is set one or more Alt keys were held down.
			///
			ALT = GLFW_MOD_ALT,

			///
			/// If this bit is set one or more Super keys were held down.
			///
			SUPER = GLFW_MOD_SUPER,

			///
			/// If this bit is set then the Caps Lock key is enabled.
			///
			CAPS_LOCK = GLFW_MOD_CAPS_LOCK,

			///
			/// If this bit is set then the Num Lock key is enabled.
			///
			NUM_LOCK = GLFW_MOD_NUM_LOCK
		};

		///
		/// Converts an int to an input mod.
		///
		/// \param mod Integer keymod.
		///
		/// \return Enum keymod.
		///
		[[nodiscard]]
		inline constexpr input::InputMods int_to_mod(const int mod)
		{
			return static_cast<input::InputMods>(mod);
		}

		///
		/// Converts a input mod to an int.
		///
		/// \param mod Enum keymod.
		///
		/// \return Integer keymod.
		///
		[[nodiscard]]
		inline constexpr int mod_to_int(const input::InputMods mod)
		{
			return static_cast<int>(mod);
		}
	} // namespace input
} // namespace galaxy

#endif
