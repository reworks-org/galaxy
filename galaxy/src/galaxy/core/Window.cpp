///
/// Window.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <raylib.h>

#include "galaxy/core/Settings.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Window.hpp"

namespace galaxy
{
	namespace core
	{
		Window::Window()
		{
			auto flags = FLAG_WINDOW_ALWAYS_RUN | FLAG_WINDOW_RESIZABLE;

			switch (settings::screenmode())
			{
				case graphics::ScreenMode::MAXIMIZED:
					flags |= FLAG_WINDOW_MAXIMIZED;
					break;

				case graphics::ScreenMode::FULLSCREEN:
					flags |= FLAG_FULLSCREEN_MODE;
					break;

				case graphics::ScreenMode::BORDERLESS:
					flags |= FLAG_BORDERLESS_WINDOWED_MODE;
					break;

				default:
					break;
			}

			if (settings::vsync())
			{
				flags |= FLAG_VSYNC_HINT;
			}

			if (settings::msaa())
			{
				flags |= FLAG_MSAA_4X_HINT;
			}

			if (settings::highdpi())
			{
				flags |= FLAG_WINDOW_HIGHDPI;
			}

			SetWindowState(flags);
			InitWindow(settings::window_width(), settings::window_height(), settings::window_title().c_str());
			DisableEventWaiting();

			if (settings::mouse_visible())
			{
				ShowCursor();
			}
			else
			{
				HideCursor();
			}

			set_icon(settings::window_icon());
		}

		Window::~Window()
		{
			CloseWindow();
		}

		void Window::poll_events()
		{
			PollInputEvents();
		}

		void Window::focus()
		{
			SetWindowFocused();
		}

		void Window::restore()
		{
			RestoreWindow();
		}

		void Window::minimize()
		{
			MinimizeWindow();
		}

		void Window::maximize()
		{
			MaximizeWindow();
		}

		void Window::toggle_fullscreen()
		{
			ToggleFullscreen();
		}

		void Window::toggle_borderless()
		{
			ToggleBorderlessWindowed();
		}

		void Window::set_icon(const std::string& icon)
		{
			if (!icon.empty())
			{
				auto& fs   = entt::locator<fs::VirtualFileSystem>::value();
				auto  data = fs.read_binary(icon);

				const char* ext = GetFileExtension(icon.c_str());
				Image       img = LoadImageFromMemory(ext, data.data(), data.size());

				SetWindowIcon(img);
			}
			else
			{
				GALAXY_LOG(GALAXY_INFO, "No icon found to set.");
			}
		}

		void Window::set_title(const std::string& title)
		{
			m_title = title;
			SetWindowTitle(title.c_str());
		}

		void Window::append_title(const std::string& append)
		{
			auto title = m_title + append;
			SetWindowTitle(title.c_str());
		}

		void Window::set_pos(const int x, int y)
		{
			SetWindowPosition(x, y);
		}

		void Window::set_size(const int width, int height)
		{
			SetWindowSize(width, height);
		}

		bool Window::should_close()
		{
			return WindowShouldClose();
		}

		bool Window::is_fullscreen()
		{
			return IsWindowFullscreen();
		}

		bool Window::is_hidden()
		{
			return IsWindowHidden();
		}

		bool Window::is_minimized()
		{
			return IsWindowMinimized();
		}

		bool Window::is_maximized()
		{
			return IsWindowMaximized();
		}

		bool Window::is_focused()
		{
			return IsWindowFocused();
		}

		int Window::width()
		{
			return GetScreenWidth();
		}

		int Window::height()
		{
			return GetScreenHeight();
		}

		int Window::render_width()
		{
			return GetRenderWidth();
		}

		int Window::render_height()
		{
			return GetRenderHeight();
		}
	} // namespace core
} // namespace galaxy
