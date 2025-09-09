///
/// Mouse.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define SDL_STBIMG_SDL3

#include <SDL_stbimage.h>

#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/logging/Log.hpp"

#include "Mouse.hpp"

namespace galaxy
{
	Mouse::Mouse()
		: m_cursor {nullptr}
	{
	}

	Mouse::~Mouse()
	{
		destroy_cursor();
	}

	bool Mouse::has_mouse() const noexcept
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

	void Mouse::show_cursor() const noexcept
	{
		SDL_ShowCursor();
	}

	void Mouse::hide_cursor() const noexcept
	{
		SDL_HideCursor();
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

	void Mouse::set_cursor_custom(const std::string& cursor, const glm::ivec2& hotspot) noexcept
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
					m_cursor = SDL_CreateColorCursor(surface, hotspot.x, hotspot.y);
					if (m_cursor)
					{
						SDL_SetCursor(m_cursor);
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to create custom cursor: '{0}'.", SDL_GetError());
					}
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

	void Mouse::restore_cursor()
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
} // namespace galaxy
