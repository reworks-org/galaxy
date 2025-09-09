///
/// WindowBindable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_WINDOWBINDABLE_HPP_
#define GALAXY_META_WINDOWBINDABLE_HPP_

struct SDL_Window;

namespace galaxy
{
	///
	/// Binds the mouse to the window.
	///
	class WindowBindable
	{
		friend class Window;

	public:
		///
		/// Virtual destructor.
		///
		virtual ~WindowBindable() noexcept;

	protected:
		///
		/// Constructor.
		///
		WindowBindable() noexcept;

		///
		/// Move constructor.
		///
		WindowBindable(WindowBindable&&) noexcept;

		///
		/// Move assignment operator.
		///
		virtual WindowBindable& operator=(WindowBindable&&) noexcept;

		///
		/// Copy constructor.
		///
		WindowBindable(const WindowBindable&) noexcept;

		///
		/// Copy assignment operator.
		///
		virtual WindowBindable& operator=(const WindowBindable&) noexcept;

		///
		/// Bind SDL3 window to this object.
		///
		/// \param window Window pointer..
		///
		void bind_window(SDL_Window* window) noexcept;

	protected:
		///
		/// Pointer to SDL_Window handle.
		///
		SDL_Window* m_window;
	};
} // namespace galaxy

#endif
