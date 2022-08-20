///
/// InputMods.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_INPUTMODS_HPP_
#define GALAXY_INPUT_INPUTMODS_HPP_

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
			UNKNOWN = 0x0000,

			///
			/// If this bit is set one or more Shift keys were held down.
			///
			SHIFT = 0x0001,

			///
			/// If this bit is set one or more Control keys were held down.
			///
			CONTROL = 0x0002,

			///
			/// If this bit is set one or more Alt keys were held down.
			///
			ALT = 0x0004,

			///
			/// If this bit is set one or more Super keys were held down.
			///
			SUPER = 0x0008,

			///
			/// If this bit is set then the Caps Lock key is enabled.
			///
			CAPS_LOCK = 0x0010,

			///
			/// If this bit is set then the Num Lock key is enabled.
			///
			NUM_LOCK = 0x0020
		};
	} // namespace input
} // namespace galaxy

#endif