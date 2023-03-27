///
/// InputDevice.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_INPUTDEVICE_HPP_
#define GALAXY_INPUT_INPUTDEVICE_HPP_

struct GLFWwindow;

#include <concepts>

namespace galaxy
{
	namespace core
	{
		class Window;
	} // namespace core

	namespace input
	{
		class InputDevice
		{
		public:
			///
			/// Virtual destructor.
			///
			virtual ~InputDevice();

		protected:
			///
			/// Constructor.
			///
			InputDevice();

			///
			/// Move constructor.
			///
			InputDevice(InputDevice&&) = default;

			///
			/// Move assignment operator.
			///
			InputDevice& operator=(InputDevice&&) = default;

			///
			/// Copy constructor.
			///
			InputDevice(const InputDevice&) = default;

			///
			/// Copy assignment operator.
			///
			InputDevice& operator=(const InputDevice&) = default;

			///
			/// Sets the window pointer.
			///
			/// \param window Window pointer from graphics::Window.
			///
			void set_window(GLFWwindow* window);

		protected:
			///
			/// Pointer to glfw window.
			///
			GLFWwindow* m_window;
		};
	} // namespace input

	namespace meta
	{
		///
		/// Checks if the input is an input device for a window.
		///
		/// \tparam T Parent type to test.
		///
		template<typename T>
		concept is_input_device = std::derived_from<T, input::InputDevice>;
	} // namespace meta
} // namespace galaxy

#endif