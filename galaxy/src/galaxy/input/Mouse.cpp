///
/// Mouse.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define SDL_STBIMG_SDL3

#include <display.h>
#include <draw.h>
#include <input.h>
#include <SDL_stbimage.h>

#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/logging/Log.hpp"

#include "Mouse.hpp"

namespace galaxy
{
	Mouse::Mouse() noexcept
		: WindowBindable {}
		, m_cursor {nullptr}
		, m_cursor_bitmap {nullptr}
	{
	}

	Mouse::~Mouse() noexcept
	{
		if (m_cursor_bitmap)
		{
			sl::destroy_bitmap(m_cursor_bitmap);
			m_cursor_bitmap = nullptr;
		}

		destroy_cursor();
	}

	bool Mouse::mouse_connected() const noexcept
	{
		return SDL_HasMouse();
	}

	void Mouse::set_mouse_grab(const bool grabbed) const noexcept
	{
		SDL_SetWindowMouseGrab(m_window, grabbed);
	}

	void Mouse::warp_mouse(const float x, const float y) const noexcept
	{
		SDL_WarpMouseInWindow(m_window, x, y);
	}

	bool Mouse::set_window_mouse_rect(iRect& rect) noexcept
	{
		const auto sdlrect = rect.to_sdl();
		return SDL_SetWindowMouseRect(m_window, &sdlrect);
	}

	void Mouse::show_cursor() const noexcept
	{
		sl::show_mouse();
	}

	void Mouse::hide_cursor() const noexcept
	{
		sl::hide_mouse();
	}

	void Mouse::set_cursor_system(const SDL_SystemCursor cursor) noexcept
	{
		destroy_cursor();

		m_cursor = SDL_CreateSystemCursor(cursor);
		if (m_cursor)
		{
			SDL_SetCursor(m_cursor);
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to create system cursor: '{0}'.", SDL_GetError());
		}
	}

	void Mouse::use_custom_mouse(const std::string& cursor, const glm::ivec2& offset)
	{
		if (!cursor.empty())
		{
			destroy_cursor();

			auto& fs = entt::locator<VirtualFileSystem>::value();

			auto data = fs.read_binary(cursor);
			if (!data.empty())
			{
				auto surface = STBIMG_LoadFromMemory(data.data(), static_cast<int>(data.size()));
				if (surface)
				{
					if (m_cursor_bitmap)
					{
						sl::destroy_bitmap(m_cursor_bitmap);
					}

					m_cursor_bitmap = sl::load_bitmap_from_surface(surface);
					sl::set_mouse_bitmap(m_cursor_bitmap, offset.x, offset.y);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, SDL_GetError());
				}

				SDL_DestroySurface(surface);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' from the vfs.", cursor);
			}
		}
	}

	void Mouse::restore_cursor() noexcept
	{
		destroy_cursor();
		SDL_SetCursor(SDL_GetDefaultCursor());
	}

	void Mouse::destroy_cursor() noexcept
	{
		if (m_cursor)
		{
			SDL_DestroyCursor(m_cursor);
			m_cursor = nullptr;
		}
	}

	bool Mouse::is_mouse_down(const MouseButton btn) const noexcept
	{
		const auto buttons = SDL_GetMouseState(nullptr, nullptr);
		return buttons & SDL_BUTTON_MASK(static_cast<SDL_MouseButtonFlags>(btn));
	}

	int Mouse::mouse_x() const noexcept
	{
		return sl::mouse_x();
	}

	int Mouse::mouse_y() const noexcept
	{
		return sl::mouse_y();
	}

	void Mouse::set_mouse_mapping(const int x, const int y, const float scale_x, const float scale_y) const noexcept
	{
		return sl::set_mouse_mapping(x, y, scale_x, scale_y);
	}

	void Mouse::clear_mouse_mapping() const noexcept
	{
		sl::clear_mouse_mapping();
	}

	void Mouse::set_relative_mouse_mode(const bool enabled) const noexcept
	{
		sl::set_relative_mouse_mode(m_window, enabled);
	}

	bool Mouse::relative_mouse_mode() const noexcept
	{
		return sl::relative_mouse_mode(m_window);
	}

	glm::ivec2 Mouse::mouse_delta(int* dx, int* dy) noexcept
	{
		glm::ivec2 vec2;
		sl::mouse_delta(&vec2.x, &vec2.y);

		return vec2;
	}
} // namespace galaxy
