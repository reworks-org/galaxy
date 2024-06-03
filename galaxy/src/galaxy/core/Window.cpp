///
/// Window.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/signal/dispatcher.hpp>
#include <glad/glad.h>
#include <stb_image.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/events/ContentScale.hpp"
#include "galaxy/events/KeyChar.hpp"
#include "galaxy/events/KeyPress.hpp"
#include "galaxy/events/KeyRepeat.hpp"
#include "galaxy/events/MouseEnter.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowClosed.hpp"
#include "galaxy/events/WindowResized.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/graphics/text/FontContext.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/input/InputMods.hpp"
#include "galaxy/platform/Pragma.hpp"
#include "galaxy/ui/ImGuiHelpers.hpp"
#include "galaxy/utils/StringUtils.hpp"

#include "Window.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 26462)
#endif

void* glfw_alloc(size_t size, void* user)
{
	return mi_malloc(size);
}

void* glfw_realloc(void* block, size_t size, void* user)
{
	return mi_realloc(block, size);
}

void glfw_dealloc(void* block, void* user)
{
	mi_free(block);
}

namespace galaxy
{
	namespace core
	{
		Window::Window(const WindowSettings& settings)
			: m_window {nullptr}
			, m_dispatcher {nullptr}
			, m_title {settings.title}
			, m_window_width {settings.window_width}
			, m_window_height {settings.window_height}
			, m_frame_width {settings.frame_width}
			, m_frame_height {settings.frame_height != 0 ? settings.frame_height : 1}
			, m_aspect_ratio {static_cast<float>(m_frame_width / m_frame_height)}
		{
			m_glfw_allocator.allocate   = &glfw_alloc;
			m_glfw_allocator.reallocate = &glfw_realloc;
			m_glfw_allocator.deallocate = &glfw_dealloc;

			// Set error handling callback.
			glfwSetErrorCallback([](int error, const char* description) {
				GALAXY_LOG(GALAXY_ERROR, "[GLFW] Code: {0}, Desc: {1}.", error, description);
			});

			glfwInitAllocator(&m_glfw_allocator);

			glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
			if (!glfwInit())
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to initialize glfw!");
			}
			else
			{
				m_cursor.init();

				// Configure window setup using hints.
				// If not here, then default is the preference.
				glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_FALSE);
				glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
				glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
				glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
				glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
				glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
				glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
				glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
				glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

				// Configure framebuffer setup.
				auto* vm = glfwGetVideoMode(glfwGetPrimaryMonitor());

				glfwWindowHint(GLFW_RED_BITS, vm->redBits);     // 8
				glfwWindowHint(GLFW_GREEN_BITS, vm->greenBits); // 8
				glfwWindowHint(GLFW_BLUE_BITS, vm->blueBits);   // 8
				glfwWindowHint(GLFW_REFRESH_RATE, vm->refreshRate);
				glfwWindowHint(GLFW_ALPHA_BITS, 8);
				glfwWindowHint(GLFW_DEPTH_BITS, 24);
				glfwWindowHint(GLFW_STENCIL_BITS, 8);
				glfwWindowHint(GLFW_STEREO, GLFW_FALSE);
				glfwWindowHint(GLFW_SAMPLES, 0); // 4
				glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
				glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

				// Configure OpenGL context hints.
				glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings.debug);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

				m_window = glfwCreateWindow(m_window_width, m_window_height, m_title.c_str(), nullptr, nullptr);
				if (m_window)
				{
					if (!settings.maximized && !settings.fullscreen)
					{
						int sx = 0, sy = 0;
						int px = 0, py = 0;
						int mx = 0, my = 0;
						int monitor_count = 0;
						int best_area     = 0;
						int final_x = 0, final_y = 0;

						glfwGetWindowSize(m_window, &sx, &sy);
						glfwGetWindowPos(m_window, &px, &py);

						auto m = glfwGetMonitors(&monitor_count);

						for (auto j = 0; j < monitor_count; ++j)
						{
							glfwGetMonitorPos(m[j], &mx, &my);
							auto mode = glfwGetVideoMode(m[j]);
							if (!mode)
							{
								continue;
							}

							const auto min_x = std::max(mx, px);
							const auto min_y = std::max(my, py);

							const auto max_x = std::min(mx + mode->width, px + sx);
							const auto max_y = std::min(my + mode->height, py + sy);

							const auto area = std::max(max_x - min_x, 0) * std::max(max_y - min_y, 0);

							if (area > best_area)
							{
								final_x = mx + (mode->width - sx) / 2;
								final_y = my + (mode->height - sy) / 2;

								best_area = area;
							}
						}

						if (best_area)
						{
							glfwSetWindowPos(m_window, final_x, final_y);
						}
						else
						{
							auto primary = glfwGetPrimaryMonitor();
							if (primary)
							{
								auto desktop = glfwGetVideoMode(primary);
								if (desktop)
								{
									glfwSetWindowPos(m_window, (desktop->width - sx) / 2, (desktop->height - sy) / 2);
								}
							}
						}
					}
					else if (settings.fullscreen)
					{
						fullscreen();
					}
					else if (settings.maximized)
					{
						maximize();
					}

					glfwShowWindow(m_window);
					glfwSetInputMode(m_window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
					glfwGetWindowSize(m_window, &m_window_width, &m_window_height); // Correct window size after setup.

					// Set input devices.
					m_keyboard.set_window(m_window);
					m_mouse.set_window(m_window);
					m_cursor.set_window(m_window);
					m_clipboard.set_window(m_window);
					input::Input::s_window = m_window;

					// Set close callback.
					glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
						if (win->m_dispatcher)
						{
							win->m_dispatcher->trigger<events::WindowClosed>();
						}
					});

					// Set resize callback.
					glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
						if (win->m_dispatcher)
						{
							// clang-format off
							events::WindowResized wr
							{
								.width = width,
								.height = height
							};
							// clang-format on

							graphics::Renderer::ref().on_window_resized(wr);
							win->m_dispatcher->trigger(wr);
						}

						win->m_window_width  = width;
						win->m_window_height = height;
					});

					// Content scale callback.
					glfwSetWindowContentScaleCallback(m_window, [](GLFWwindow* window, float xscale, float yscale) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
						if (win->m_dispatcher)
						{
							// clang-format off
							events::ContentScale sc
							{
								.xscale = xscale,
								.yscale = yscale
							};
							// clang-format on

							win->m_dispatcher->trigger(sc);
						}

						if (!ServiceLocator<graphics::FontContext>::empty())
						{
							auto& fc = ServiceLocator<graphics::FontContext>::ref();
							fc.set_dpi(xscale * 96.0f, yscale * 96.0f);
						}

						if (ui::imgui_loaded())
						{
							ui::scale_and_load_fonts();
						}
					});

					// Key input callback.
					glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
						if (win->m_dispatcher && !win->m_keyboard.is_text_input_enabled())
						{
							// clang-format off
							events::KeyPress kp
							{
								.keycode  = input::int_to_key(key),
								.mod      = input::int_to_mod(mods),
								.scancode = scancode,
								.pressed  = action == GLFW_PRESS,
								.repeat   = action == GLFW_REPEAT
							};
							// clang-format on

							win->m_dispatcher->trigger(kp);
						}
					});

					// Text input callback.
					glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
						if (win->m_dispatcher && win->m_keyboard.is_text_input_enabled())
						{
							// clang-format off
							events::KeyChar kc
							{
								.codepoint = codepoint
							};
							// clang-format on

							win->m_dispatcher->trigger(kc);
						}
					});

					// Mouse entered callback.
					glfwSetCursorEnterCallback(m_window, [](GLFWwindow* window, int entered) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
						if (win->m_dispatcher)
						{
							// clang-format off
							events::MouseEnter me
							{
								.entered = static_cast<bool>(entered)
							};
							// clang-format on

							win->m_dispatcher->trigger(me);
						}
					});

					// Mouse movement callback.
					glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
						if (win->m_dispatcher)
						{
							// clang-format off
							events::MouseMoved mm
							{
								.xpos = xpos,
								.ypos = ypos
							};
							// clang-format on

							win->m_dispatcher->trigger(mm);
						}
					});

					// Mouse button callback.
					glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
						if (win->m_dispatcher)
						{
							const auto pos = win->m_mouse.get_pos();

							// clang-format off
							switch (action)
							{
								case GLFW_PRESS:
								{
									events::MousePressed mp
									{
										.xpos = pos.x,
										.ypos = pos.y,
										.button = input::int_to_mouse(button),
										.mod = input::int_to_mod(mods)
									};
								
									win->m_dispatcher->trigger(mp);
								}
								break;

								case GLFW_RELEASE:
								{
									events::MouseReleased mr
									{
										.xpos = pos.x,
										.ypos = pos.y,
										.button = input::int_to_mouse(button),
										.mod = input::int_to_mod(mods)
									};
								
									win->m_dispatcher->trigger(mr);
								}
								break;

								default:
									break;
							}
							// clang-format on
						}
					});

					// Set scroll wheel callback.
					glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
						if (win->m_dispatcher)
						{
							// clang-format off
							events::MouseWheel mw
							{
								.xoff = xoffset,
								.yoff = yoffset
							};
							// clang-format on

							win->m_dispatcher->trigger(mw);
						}
					});

					// clang-format off
					#ifdef GALAXY_WIN_PLATFORM
					GALAXY_DISABLE_WARNING_PUSH
					GALAXY_DISABLE_WARNING(26487)
					#endif
					// clang-format on

					// Set dropped item callback.
					glfwSetDropCallback(m_window, [](GLFWwindow* window, int count, const char** paths) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

						win->m_drop_paths.clear();
						for (auto i = 0; i < count; i++)
						{
							win->m_drop_paths.emplace_back(paths[i]);
						}
					});

					// clang-format off
					#ifdef GALAXY_WIN_PLATFORM
					GALAXY_DISABLE_WARNING_POP
					#endif
					// clang-format on

					glfwMakeContextCurrent(m_window);
					glfwSetWindowUserPointer(m_window, this);

					// Set up glad.
					if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to initialize glad.");
					}
					else
					{
						// Set vsync.
						glfwSwapInterval(settings.vsync);

						// Debug configuration.
						if (settings.debug)
						{
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
											GALAXY_LOG(GALAXY_WARNING, "[OpenGL] - {0}", message);
											break;
										case GL_DEBUG_SEVERITY_LOW:
											GALAXY_LOG(GALAXY_DEBUG, "[OpenGL] - {0}", message);
											break;
										default:
											GALAXY_LOG(GALAXY_INFO, "[OpenGL] - {0}", message);
											break;
									};
								},
								nullptr);

							glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
						}

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
				}
				else
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create window.");
				}
			}
		}

		Window::~Window()
		{
			// Call again to ensure everything is cleaned up.
			// Has checks to ensure no null data is destroyed.
			destroy();
		}

		void Window::set_title(const std::string& title)
		{
			glfwSetWindowTitle(m_window, title.c_str());
		}

		void Window::append_title(const std::string& append)
		{
			auto title = m_title + append;
			glfwSetWindowTitle(m_window, title.c_str());
		}

		void Window::set_icon(const std::string& icon)
		{
			if (!icon.empty())
			{
				auto& fs = ServiceLocator<fs::VirtualFileSystem>::ref();

				auto data = fs.read_binary(icon);
				if (!data.empty())
				{
					// Fill glfw-compatible struct.
					stbi_set_flip_vertically_on_load(true);

					GLFWimage img = {};
					img.pixels    = stbi_load_from_memory(data.data(), static_cast<int>(data.size()), &img.width, &img.height, nullptr, STBI_rgb_alpha);

					if (img.pixels)
					{
						// Copies data so safe to destroy.
						glfwSetWindowIcon(m_window, 1, &img);
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to load image '{0}' for window icon.", icon);
					}

					stbi_image_free(img.pixels);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' from the vfs.", icon);
				}
			}
		}

		void Window::set_icon(std::span<std::uint8_t> buffer)
		{
			if (!buffer.empty())
			{
				// Fill glfw-compatible struct.
				stbi_set_flip_vertically_on_load(true);

				GLFWimage img = {};
				img.pixels    = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &img.width, &img.height, nullptr, STBI_rgb_alpha);

				if (img.pixels)
				{
					// Copies data so safe to destroy.
					glfwSetWindowIcon(m_window, 1, &img);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load window icon from memory");
				}

				stbi_image_free(img.pixels);
			}
		}

		void Window::set_dispatcher(entt::dispatcher* dispatcher)
		{
			m_dispatcher = dispatcher;
		}

		bool Window::is_open() const
		{
			return !glfwWindowShouldClose(m_window);
		}

		void Window::close()
		{
			glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		}

		void Window::destroy()
		{
			if (m_window != nullptr)
			{
				m_cursor.destroy();
				m_cursor.destroy_system_cursors();

				glfwDestroyWindow(m_window);

				m_window               = nullptr;
				input::Input::s_window = nullptr;

				input::Input::s_cursor_pos.x = 0.0;
				input::Input::s_cursor_pos.y = 0.0;

				glfwTerminate();
			}
		}

		void Window::poll_events()
		{
			glfwPollEvents();
		}

		void Window::resize(const int width, const int height)
		{
			m_window_width  = width;
			m_window_height = height;

			glfwSetWindowSize(m_window, m_window_width, m_window_height);
		}

		void Window::request_attention()
		{
			glfwRequestWindowAttention(m_window);
		}

		void Window::focus() const
		{
			glfwFocusWindow(m_window);
		}

		void Window::maximize()
		{
			glfwMaximizeWindow(m_window);
			glfwGetWindowSize(m_window, &m_window_width, &m_window_height);
		}

		void Window::restore()
		{
			glfwRestoreWindow(m_window);
			glfwGetWindowSize(m_window, &m_window_width, &m_window_height);
		}

		void Window::minimize() const
		{
			glfwIconifyWindow(m_window);
		}

		void Window::fullscreen()
		{
			auto* monitor = glfwGetPrimaryMonitor();
			auto* mode    = glfwGetVideoMode(monitor);

			glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			glfwGetWindowSize(m_window, &m_window_width, &m_window_height);
		}

		bool Window::is_focused() const
		{
			return glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
		}

		int Window::window_width() const
		{
			return m_window_width;
		}

		int Window::window_height() const
		{
			return m_window_height;
		}

		int Window::frame_width() const
		{
			return m_frame_width;
		}

		int Window::frame_height() const
		{
			return m_frame_height;
		}

		float Window::aspect_ratio() const
		{
			return m_aspect_ratio;
		}

		const meta::vector<std::string>& Window::get_drop_paths() const
		{
			return m_drop_paths;
		}

		glm::ivec2 Window::get_framebuffer_size()
		{
			glm::ivec2 size;
			glfwGetFramebufferSize(m_window, &size.x, &size.y);

			return size;
		}

		glm::vec2 Window::get_content_scale()
		{
			glm::vec2 size;
			glfwGetWindowContentScale(m_window, &size.x, &size.y);

			return size;
		}

		float Window::get_content_scale_max()
		{
			const auto scale = get_content_scale();
			return std::max(scale.x, scale.y);
		}

		GLFWwindow* Window::handle()
		{
			return m_window;
		}
	} // namespace core
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
