///
/// Window.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define SDL_STBIMG_SDL3

#include <entt/signal/dispatcher.hpp>
#include <glad/glad.h>
#include <SDL_stbimage.h>

#include "galaxy/core/Settings.hpp"
#include "galaxy/events/GainedFocus.hpp"
#include "galaxy/events/KeyInput.hpp"
#include "galaxy/events/KeyPressed.hpp"
#include "galaxy/events/KeyReleased.hpp"
#include "galaxy/events/LostFocus.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowClosed.hpp"
#include "galaxy/events/WindowResized.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/logging/Log.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "Window.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26818)
#endif

namespace galaxy
{
	Window::Window()
		: m_window {nullptr}
		, m_context {nullptr}
		, m_open {true}
	{
		SDL_zero(m_events);

		if (SDL_ScreenSaverEnabled())
		{
			SDL_DisableScreenSaver();
		}

		SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
		if (Settings::fullscreen())
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		else
		{
			if (Settings::maximized())
			{
				flags |= SDL_WINDOW_MAXIMIZED;
			}
		}

		if (Settings::mouse_grabbed())
		{
			flags |= SDL_WINDOW_MOUSE_GRABBED;
		}

		if (Settings::window_resizable())
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		// SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, );      /**< the minimum number of bits for frame buffer size; defaults to 0. */
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		// SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, );   /**< the minimum number of bits for the red channel of the accumulation buffer; defaults to 0. */
		// SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, ); /**< the minimum number of bits for the green channel of the accumulation buffer; defaults to 0. */
		// SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, );  /**< the minimum number of bits for the blue channel of the accumulation buffer; defaults to 0. */
		// SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, ); /**< the minimum number of bits for the alpha channel of the accumulation buffer; defaults to 0. */
		SDL_GL_SetAttribute(SDL_GL_STEREO, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0);
		SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);
		// SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, );   /**< sets context the release behavior. See SDL_GLContextReleaseFlag; defaults to FLUSH. */
		// SDL_GL_SetAttribute(SDL_GL_CONTEXT_RESET_NOTIFICATION, ); /**< set context reset notification. See SDL_GLContextResetNotification; defaults to NO_NOTIFICATION. */

		m_window = SDL_CreateWindow(Settings::title().c_str(), Settings::window_width(), Settings::window_height(), flags);
		if (m_window)
		{
			m_context = SDL_GL_CreateContext(m_window);
			if (m_context)
			{
				if (SDL_GL_MakeCurrent(m_window, m_context))
				{
					if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
					{
						SDL_GL_SetSwapInterval(Settings::vsync());
						SDL_SetWindowAlwaysOnTop(m_window, false);
						SDL_SetWindowBordered(m_window, Settings::window_border());
						SDL_SetWindowFocusable(m_window, true);
						SDL_SetWindowFullscreenMode(m_window, nullptr); // Borderless Fullscreen mode.
						SDL_SetWindowKeyboardGrab(m_window, false);
						SDL_SetWindowMinimumSize(m_window, 640, 360);
						SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

						m_keyboard.bind_window(m_window);
						m_mouse.bind_window(m_window);
						m_mouse.set_cursor_custom(Settings::cursor_icon(), Settings::cursor_hotspot());
						if (Settings::cursor_show())
						{
							m_mouse.show_cursor();
						}
						else
						{
							m_mouse.hide_cursor();
						}

						// Debug configuration.
						glEnable(GL_DEBUG_OUTPUT);
						glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

						glDebugMessageCallback(
							[](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
								GALAXY_UNUSED(source);
								GALAXY_UNUSED(type);
								GALAXY_UNUSED(id);
								GALAXY_UNUSED(length);
								GALAXY_UNUSED(userParam);

								switch (severity)
								{
									case GL_DEBUG_SEVERITY_HIGH:
										GALAXY_LOG(GALAXY_ERROR, "[OpenGL] - {0}", message);
										break;
									case GL_DEBUG_SEVERITY_MEDIUM:
										GALAXY_LOG(GALAXY_WARN, "[OpenGL] - {0}", message);
										break;
									case GL_DEBUG_SEVERITY_LOW:
										GALAXY_LOG(GALAXY_INFO, "[OpenGL] - {0}", message);
										break;
									default:
										GALAXY_LOG(GALAXY_INFO, "[OpenGL] - {0}", message);
										break;
								};
							},
							nullptr
						);

						glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

						// Configure global GL state.
						glDisable(GL_FRAMEBUFFER_SRGB);
						glDisable(GL_CULL_FACE);
						glDisable(GL_SCISSOR_TEST);
						glDisable(GL_MULTISAMPLE); // Use provided SMAA.

						glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
						glEnable(GL_PROGRAM_POINT_SIZE);
						glEnable(GL_DEPTH_TEST);
						glEnable(GL_STENCIL_TEST);
						glEnable(GL_BLEND);

						// GL state function configuration.
						glCullFace(GL_BACK);
						glDepthFunc(GL_LEQUAL);
						glBlendEquation(GL_FUNC_ADD);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					}
					else
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to initialize glad.");
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to initialize OpenGL context into SDL.");
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to initialize OpenGL context.");
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_FATAL, "Failed to initialize SDL window.");
		}
	}

	Window::~Window()
	{
		SDL_GL_DestroyContext(m_context);
		SDL_DestroyWindow(m_window);

		m_context = nullptr;
		m_window  = nullptr;
		m_open    = false;
	}

	void Window::process_events(entt::dispatcher& dispatcher)
	{
		while (SDL_PollEvent(&m_events))
		{
			switch (m_events.type)
			{
				case SDL_EVENT_QUIT:
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				case SDL_EVENT_WINDOW_DESTROYED:
					dispatcher.trigger(WindowClosed {});
					close();
					break;

				case SDL_EVENT_LOCALE_CHANGED:
					// TODO: Handle Languages.
					break;

				case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
				case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
					// SDL_GetDisplayContentScale
					// SDL_GetWindowDisplayScale
					// https://wiki.libsdl.org/SDL3/README-highdpi
					break;

				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
					// SDL_GetWindowSizeInPixels
					break;

				case SDL_EVENT_WINDOW_RESIZED:
				case SDL_EVENT_WINDOW_MAXIMIZED:
				case SDL_EVENT_WINDOW_RESTORED:
				case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
				case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
					{
						const auto size = get_pixel_size();

						// clang-format off
							auto wr = WindowResized
							{
								.m_width = size.x,
								.m_height = size.y
							};
						// clang-format on

						dispatcher.trigger(wr);
					}
					break;

				case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
					// TODO
					// HANDLE RESIZE + DISPLAY SCALE + PIXEL SIZE SCALE.
					break;

				case SDL_EVENT_WINDOW_MOUSE_ENTER:
				case SDL_EVENT_WINDOW_FOCUS_GAINED:
					dispatcher.trigger(GainedFocus {});
					break;

				case SDL_EVENT_WINDOW_MOUSE_LEAVE:
				case SDL_EVENT_WINDOW_FOCUS_LOST:
					dispatcher.trigger(LostFocus {});
					break;

				case SDL_EVENT_KEY_DOWN:
					{
						// clang-format off
							auto kp = KeyPressed
							{
								.m_scancode = m_events.key.scancode,
								.m_key = static_cast<Keys>(m_events.key.key),
								.m_mod = static_cast<KeyMods>(m_events.key.mod),
								.m_raw = m_events.key.raw,
								.m_repeat = m_events.key.repeat
							};
						// clang-format on

						dispatcher.trigger(kp);
					}
					break;

				case SDL_EVENT_KEY_UP:
					{
						// clang-format off
							auto kr = KeyReleased
							{
								.m_scancode = m_events.key.scancode,
								.m_key = static_cast<Keys>(m_events.key.key),
								.m_mod = static_cast<KeyMods>(m_events.key.mod),
								.m_raw = m_events.key.raw,
								.m_repeat = m_events.key.repeat
							};
						// clang-format on

						dispatcher.trigger(kr);
					}
					break;

				case SDL_EVENT_TEXT_INPUT:
					// case SDL_EVENT_TEXT_EDITING:
					// case SDL_EVENT_TEXT_EDITING_CANDIDATES:
					{
						// clang-format off
						auto ki = KeyInput
						{
							.m_text = m_events.text.text
						};
						// clang-format on

						dispatcher.trigger(ki);
					}
					break;

				case SDL_EVENT_MOUSE_MOTION:
					{
						// clang-format off
							auto mm = MouseMoved
							{
								.m_xpos = m_events.motion.x,
								.m_ypos = m_events.motion.y,
								.m_xrel = m_events.motion.xrel,
								.m_yrel = m_events.motion.yrel
							};
						// clang-format on

						dispatcher.trigger(mm);
					}
					break;

				case SDL_EVENT_MOUSE_BUTTON_DOWN:
					{
						// clang-format off
							auto mp = MousePressed
							{
								.m_xpos = m_events.button.x,
								.m_ypos = m_events.button.y,
								.m_clicks = m_events.button.clicks,
								.m_button = static_cast<MouseButton>(m_events.button.button)
							};
						// clang-format on

						dispatcher.trigger(mp);
					}
					break;

				case SDL_EVENT_MOUSE_BUTTON_UP:
					{
						// clang-format off
							auto mr = MouseReleased
							{
								.m_xpos = m_events.button.x,
								.m_ypos = m_events.button.y,
								.m_clicks = m_events.button.clicks,
								.m_button = static_cast<MouseButton>(m_events.button.button)
							};
						// clang-format on

						dispatcher.trigger(mr);
					}
					break;

				case SDL_EVENT_MOUSE_WHEEL:
					{
						// clang-format off
							auto mw = MouseWheel
							{
								.m_amount_x = m_events.wheel.x,
								.m_amount_y = m_events.wheel.y,
								.m_direction = m_events.wheel.direction,
								.m_mouse_x = m_events.wheel.mouse_x,
								.m_mouse_y = m_events.wheel.mouse_y,
								.m_total_x = m_events.wheel.integer_x,
								.m_total_y = m_events.wheel.integer_y
							};
						// clang-format on

						dispatcher.trigger(mw);
					}
					break;

				case SDL_EVENT_DROP_BEGIN:
					// TODO
					/*Drag and drop events
					SDL_EVENT_DROP_FILE = 0x1000, /**< The system requests a file open *
					SDL_EVENT_DROP_TEXT,      /**< text/plain drag-and-drop event *
					SDL_EVENT_DROP_BEGIN,     /**< A new set of drops is beginning (NULL filename) *
					SDL_EVENT_DROP_COMPLETE,  /**< Current set of drops is now complete (NULL filename) *
					SDL_EVENT_DROP_POSITION,  /**< Position while moving over the window */
					// SDL_DropEvent
					break;

				case SDL_EVENT_JOYSTICK_AXIS_MOTION:
				case SDL_EVENT_JOYSTICK_BALL_MOTION:
				case SDL_EVENT_JOYSTICK_HAT_MOTION:
				case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
				case SDL_EVENT_JOYSTICK_BUTTON_UP:
				case SDL_EVENT_JOYSTICK_REMOVED:
				case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
				case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
					// TODO
					GALAXY_LOG(GALAXY_WARN, "Unsupported joystick event detected.");
					break;

				case SDL_EVENT_GAMEPAD_AXIS_MOTION:
				case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
				case SDL_EVENT_GAMEPAD_BUTTON_UP:
				case SDL_EVENT_GAMEPAD_REMOVED:
				case SDL_EVENT_GAMEPAD_REMAPPED:
				case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
				case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
				case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
				case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
				case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
				case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
					// TODO
					GALAXY_LOG(GALAXY_WARN, "Unsupported gamepad event detected.");
					break;

				case SDL_EVENT_FINGER_DOWN:
				case SDL_EVENT_FINGER_UP:
				case SDL_EVENT_FINGER_MOTION:
				case SDL_EVENT_FINGER_CANCELED:
					// TODO
					GALAXY_LOG(GALAXY_WARN, "Unsupported touch event detected.");
					break;

				case SDL_EVENT_PEN_PROXIMITY_IN:
				case SDL_EVENT_PEN_PROXIMITY_OUT:
				case SDL_EVENT_PEN_DOWN:
				case SDL_EVENT_PEN_UP:
				case SDL_EVENT_PEN_BUTTON_DOWN:
				case SDL_EVENT_PEN_BUTTON_UP:
				case SDL_EVENT_PEN_MOTION:
				case SDL_EVENT_PEN_AXIS:
					// TODO
					GALAXY_LOG(GALAXY_WARN, "Unsupported pen event detected.");
					break;
			}
		}
	}

	void Window::close() noexcept
	{
		m_open = false;
	}

	void Window::swap() const noexcept
	{
		SDL_GL_SwapWindow(m_window);
	}

	void Window::show() const noexcept
	{
		SDL_ShowWindow(m_window);
	}

	void Window::hide() const noexcept
	{
		SDL_HideWindow(m_window);
	}

	void Window::minimize() const noexcept
	{
		SDL_MinimizeWindow(m_window);
		SDL_SyncWindow(m_window);
	}

	void Window::maximize() const noexcept
	{
		SDL_MaximizeWindow(m_window);
		SDL_SyncWindow(m_window);
		// TODO: trigger resize.
	}

	void Window::restore() const noexcept
	{
		SDL_RestoreWindow(m_window);
		SDL_SyncWindow(m_window);
		// TODO: trigger resize.
	}

	void Window::raise() const noexcept
	{
		SDL_RaiseWindow(m_window);
	}

	void Window::request_attention() const noexcept
	{
		SDL_FlashWindow(m_window, SDL_FlashOperation::SDL_FLASH_UNTIL_FOCUSED);
	}

	void Window::resize(const int width, const int height) const noexcept
	{
		SDL_SetWindowSize(m_window, width, height);
		// TODO: trigger resize.
	}

	void Window::set_fullscreen(const bool fullscreen) const noexcept
	{
		SDL_SetWindowFullscreen(m_window, fullscreen);
		// TODO: trigger resize.
	}

	void Window::set_taskbar_progress(const float progress) noexcept
	{
		SDL_SetWindowProgressValue(m_window, std::clamp(progress, 0.0f, 1.0f));
	}

	void Window::append_title(const std::string& append)
	{
		const auto title = Settings::title() + append;
		SDL_SetWindowTitle(m_window, title.c_str());
	}

	void Window::set_icon(const std::string& icon) noexcept
	{
		if (!icon.empty())
		{
			auto& fs = entt::locator<VirtualFileSystem>::value();

			auto data = fs.read_binary(icon);
			if (!data.empty())
			{
				auto surface = STBIMG_LoadFromMemory(data.data(), static_cast<int>(data.size()));
				if (surface)
				{
					SDL_SetWindowIcon(m_window, surface);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, SDL_GetError());
				}

				SDL_DestroySurface(surface);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' from the vfs.", icon);
			}
		}
	}

	bool Window::is_open() const noexcept
	{
		return m_open;
	}

	Mouse& Window::get_mouse() noexcept
	{
		return m_mouse;
	}

	Keyboard& Window::get_keyboard() noexcept
	{
		return m_keyboard;
	}

	glm::ivec2 Window::get_pixel_size() noexcept
	{
		auto vec2 = glm::ivec2();
		SDL_GetWindowSizeInPixels(m_window, &vec2.x, &vec2.y);

		return vec2;
	}

	SDL_Window* Window::handle() const noexcept
	{
		return m_window;
	}

	SDL_GLContext Window::context() const noexcept
	{
		return m_context;
	}
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
