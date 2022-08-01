///
/// InputDevice.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_INPUTDEVICE_HPP_
#define GALAXY_INPUT_INPUTDEVICE_HPP_

struct GLFWwindow;

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
			virtual ~InputDevice() noexcept;

		protected:
			///
			/// Constructor.
			///
			InputDevice() noexcept;

			///
			/// Move constructor.
			///
			InputDevice(InputDevice&&) noexcept = default;

			///
			/// Move assignment operator.
			///
			InputDevice& operator=(InputDevice&&) noexcept = default;

			///
			/// Copy constructor.
			///
			InputDevice(const InputDevice&) noexcept = default;

			///
			/// Copy assignment operator.
			///
			InputDevice& operator=(const InputDevice&) noexcept = default;

			///
			/// Sets the window pointer.
			///
			/// \param window Window pointer from graphics::Window.
			///
			void set_window(GLFWwindow* window) noexcept;

		protected:
			///
			/// Pointer to glfw window.
			///
			GLFWwindow* m_window;
		};
	} // namespace input
} // namespace galaxy

#endif