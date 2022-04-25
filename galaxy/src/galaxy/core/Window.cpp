///
/// Window.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/input/InputMods.hpp"
#include "galaxy/utils/Globals.hpp"
#include "galaxy/utils/StringUtils.hpp"

#include "Window.hpp"

namespace galaxy
{
	namespace core
	{
		Window::Window() noexcept
			: m_window {nullptr}
			, m_width {1}
			, m_height {1}
		{
		}

		Window::Window(const WindowSettings& settings)
			: m_window {nullptr}
			, m_width {1}
			, m_height {1}
		{
			open(settings);
		}

		Window::~Window()
		{
			// Call again to ensure everything is cleaned up.
			// Has checks to ensure no null data is destroyed.
			destroy();
		}

		void Window::open(const WindowSettings& settings)
		{
			// Set error handling callback.
			glfwSetErrorCallback([](int error, const char* description) {
				GALAXY_LOG(GALAXY_ERROR, "[GLFW] Code: {0}, Desc: {1}.", error, description);
			});

			glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
			if (!glfwInit())
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to initialize glfw!");
			}
			else
			{
				// Configure window setup using hints.
				// If not here, then default is the preference.
				glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
				glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
				glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
				glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
				glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
				glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
				glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
				glfwWindowHint(GLFW_SCALE_TO_MONITOR, settings.m_scale_to_monitor);

				// Configure framebuffer setup.
				glfwWindowHint(GLFW_RED_BITS, 8);
				glfwWindowHint(GLFW_GREEN_BITS, 8);
				glfwWindowHint(GLFW_BLUE_BITS, 8);
				glfwWindowHint(GLFW_ALPHA_BITS, 8);
				glfwWindowHint(GLFW_DEPTH_BITS, 24);
				glfwWindowHint(GLFW_STENCIL_BITS, 8);
				glfwWindowHint(GLFW_STEREO, GLFW_FALSE);
				glfwWindowHint(GLFW_SAMPLES, 0);
				glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
				glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

				// Configure OpenGL context hints.
				glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings.m_debug);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

				// Window maximized check.
				if (settings.m_maximized)
				{
					glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
				}

				// Create the window from input, ensuring it is centered in the screen.
				m_window = glfwCreateWindow(settings.m_width, settings.m_height, settings.m_title.c_str(), nullptr, nullptr);
				if (!m_window)
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create window.");
				}
				else
				{
					m_keyboard.set_window(m_window);
					m_mouse.set_window(m_window);
					m_cursor.set_window(m_window);
					m_clipboard.set_window(m_window);

					// Set resize callback.
					glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
						auto* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

						// clang-format off
						events::WindowResized wr
						{
							.m_width = width,
							.m_height = height
						};
						// clang-format on

						win->m_event_queue.emplace(std::move(wr));
						win->resize(width, height);
					});

					// Key input callback.
					glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
						auto* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
						if (!win->m_keyboard.is_text_input_enabled())
						{
							// clang-format off
							switch (action)
							{
								case GLFW_PRESS:
								{
									events::KeyDown kd
									{
										.m_keycode = static_cast<input::Keys>(key),
										.m_mod = static_cast<input::InputMods>(mods),
										.m_scancode = scancode
									};
								
									win->m_event_queue.emplace<events::KeyDown>(std::move(kd));
								}
								break;

								case GLFW_RELEASE:
								{
									events::KeyUp ku
									{
										.m_keycode = static_cast<input::Keys>(key),
										.m_mod = static_cast<input::InputMods>(mods),
										.m_scancode = scancode
									};
								
									win->m_event_queue.emplace<events::KeyUp>(std::move(ku));
								}
								break;
							}
							// clang-format on
						}
					});

					// Text input callback.
					glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
						auto* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
						if (win->m_keyboard.is_text_input_enabled())
						{
							// clang-format off
							events::KeyChar kc
							{
								.m_uichar = codepoint,
								.m_char = strutils::parse_codepoint(codepoint)
							};
							// clang-format on

							win->m_event_queue.emplace<events::KeyChar>(std::move(kc));
						}
					});

					// Mouse movement callback.
					glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
						auto* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

						// clang-format off
						events::MouseMoved mm
						{
							.m_xpos = xpos,
							.m_ypos = ypos
						};
						// clang-format on

						win->m_event_queue.emplace<events::MouseMoved>(std::move(mm));
					});

					// Mouse button callback.
					glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
						auto* win      = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
						const auto pos = win->m_mouse.get_pos();

						// clang-format off
						switch (action)
						{
							case GLFW_PRESS:
							{
								events::MousePressed mp
								{
									.m_xpos = pos.x,
									.m_ypos = pos.y,
									.m_button = static_cast<input::Mouse::Buttons>(button),
									.m_mod = static_cast<input::InputMods>(mods)
								};
								
								win->m_event_queue.emplace<events::MousePressed>(std::move(mp));
							}
							break;

							case GLFW_RELEASE:
							{
								events::MouseReleased mr
								{
									.m_xpos = pos.x,
									.m_ypos = pos.y,
									.m_button = static_cast<input::Mouse::Buttons>(button),
									.m_mod = static_cast<input::InputMods>(mods)
								};
								
								win->m_event_queue.emplace<events::MouseReleased>(std::move(mr));
							}
							break;
						}
						// clang-format on
					});

					// Set scroll wheel callback.
					glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
						auto* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

						// clang-format off
						events::MouseWheel mw
						{
							.m_xoff = xoffset,
							.m_yoff = yoffset
						};
						// clang-format on

						win->m_event_queue.emplace<events::MouseWheel>(std::move(mw));
					});

					// Set dropped item callback.
					glfwSetDropCallback(m_window, [](GLFWwindow* window, int count, const char** paths) {
						auto* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

						win->m_drop_paths.clear();
						for (auto i = 0; i < count; i++)
						{
							win->m_drop_paths.emplace_back(paths[i]);
						}
					});

					glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));
					glfwGetWindowSize(m_window, &m_width, &m_height);

					// Set window context and aspect ratio.
					glfwMakeContextCurrent(m_window);

					// Set up glad.
					if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to initialize glad.");
					}
					else
					{
						// Set vsync.
						glfwSwapInterval(settings.m_vsync);

						// Debug configuration.
						if (settings.m_debug)
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
						glDisable(GL_MULTISAMPLE);
						glDisable(GL_FRAMEBUFFER_SRGB);
						glDisable(GL_CULL_FACE);

						glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
						glEnable(GL_PROGRAM_POINT_SIZE);
						glEnable(GL_DEPTH_TEST);
						glEnable(GL_BLEND);

						// GL state function configuration.
						glCullFace(GL_BACK);
						glDepthFunc(GL_LEQUAL);
						glBlendEquation(GL_FUNC_ADD);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

						// Configure renderer and post processing.
						graphics::Renderer::init();
						m_postprocess = std::make_unique<graphics::PostProcess>(m_width, m_height);

						if (settings.m_enable_aa)
						{
							m_postprocess->add<graphics::SMAA>(m_width, m_height);
						}

						if (settings.m_enable_sharpen)
						{
							m_postprocess->add<graphics::Sharpen>(m_width, m_height);
						}
					}
				}
			}
		}

		void Window::set_title(const char* title) noexcept
		{
			glfwSetWindowTitle(m_window, title);
		}

		void Window::set_icon(std::string_view icon) noexcept
		{
			auto& fs  = ServiceLocator<fs::VirtualFileSystem>::ref();
			auto info = fs.find(icon);

			if (info.m_code == fs::FileInfo::Code::FOUND)
			{
				// Fill glfw-compatible struct.
				stbi_set_flip_vertically_on_load(true);

				GLFWimage img = {};
				img.pixels    = stbi_load(info.m_string.c_str(), &img.width, &img.height, nullptr, STBI_rgb_alpha);

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
				GALAXY_LOG(GALAXY_ERROR, "Failed to find {0} to use as a window icon.", icon);
			}
		}

		void Window::set_icon(std::span<unsigned char> buffer) noexcept
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

		bool Window::is_open() const noexcept
		{
			return !glfwWindowShouldClose(m_window);
		}

		void Window::close() noexcept
		{
			glfwSetWindowShouldClose(m_window, true);
		}

		void Window::destroy()
		{
			graphics::Renderer::destroy();

			// Clean up window data, checking to make sure its not already been destroyed.
			if (m_window != nullptr)
			{
				glfwDestroyWindow(m_window);
				m_window = nullptr;
			}

			glfwTerminate();
		}

		void Window::poll_events() noexcept
		{
			while (!m_event_queue.empty())
			{
				m_event_queue.pop();
			}

			glfwPollEvents();
		}

		void Window::begin()
		{
			m_postprocess->bind();
		}

		void Window::end()
		{
			graphics::Renderer::draw();
			m_postprocess->render_effects();

			// Final Output.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_width, m_height);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_postprocess->render_output();

			glfwSwapBuffers(m_window);

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}

		void Window::resize(const int width, const int height) noexcept
		{
			m_width  = width;
			m_height = height;

			m_postprocess->resize(width, height);
			glfwSetWindowSize(m_window, m_width, m_height);
		}

		void Window::trigger_queued_events(entt::dispatcher& dispatcher)
		{
			while (!m_event_queue.empty())
			{
				// clang-format off
				std::visit([&](auto&& event) {
                    dispatcher.trigger<std::decay<decltype(event)>::type>(event);
                }, m_event_queue.front());
				// clang-format on

				m_event_queue.pop();
			}
		}

		void Window::request_attention() noexcept
		{
			glfwRequestWindowAttention(m_window);
		}

		void Window::focus() const noexcept
		{
			glfwFocusWindow(m_window);
		}

		void Window::allow_native_closing() noexcept
		{
			glfwSetWindowCloseCallback(m_window, nullptr);
		}

		void Window::prevent_native_closing() noexcept
		{
			glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
				glfwSetWindowShouldClose(window, GLFW_FALSE);
			});
		}

		bool Window::is_focused() const noexcept
		{
			return glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
		}

		events::Queue& Window::queued_events() noexcept
		{
			return m_event_queue;
		}

		const std::vector<std::string>& Window::get_drop_paths() const noexcept
		{
			return m_drop_paths;
		}

		int Window::get_width() const noexcept
		{
			return m_width;
		}

		int Window::get_height() const noexcept
		{
			return m_height;
		}

		GLFWwindow* Window::handle() noexcept
		{
			return m_window;
		}
	} // namespace core
} // namespace galaxy

/*




		const bool Window::key_down(input::Keys key) noexcept
		{
			return glfwGetKey(m_window, m_keyboard.m_keymap[key]) == GLFW_PRESS;
		}

		const bool Window::key_pressed(input::Keys key) noexcept
		{
			bool      res   = false;
			const int state = glfwGetKey(m_window, m_keyboard.m_keymap[key]);
			if (m_keyboard.m_prev_key_states[key] == GLFW_RELEASE && state == GLFW_PRESS)
			{
				res = true;
			}

			m_keyboard.m_prev_key_states[key] = state;
			return res;
		}

		const bool Window::mouse_button_pressed(input::MouseButtons mouse_button) noexcept
		{
			bool res = false;

			const int state = glfwGetMouseButton(m_window, m_mouse.m_mouse_map[mouse_button]);
			if (state == GLFW_PRESS && m_mouse.m_prev_mouse_btn_states[m_mouse.m_mouse_map[mouse_button]] == GLFW_RELEASE)
			{
				res = true;
			}

			m_mouse.m_prev_mouse_btn_states[m_mouse.m_mouse_map[mouse_button]] = state;

			return res;
		}

		const bool Window::mouse_button_released(input::MouseButtons mouse_button) noexcept
		{
			if (glfwGetMouseButton(m_window, m_mouse.m_mouse_map[mouse_button]) == GLFW_RELEASE)
			{
				m_mouse.m_prev_mouse_btn_states[m_mouse.m_mouse_map[mouse_button]] = GLFW_RELEASE;
				return true;
			}

			return false;
		}



		const double Window::get_scroll_delta() noexcept
		{
			const double old_delta = m_mouse.m_scroll_delta;
			m_mouse.m_scroll_delta = 0.0;

			return old_delta;
		}

		glm::vec2 Window::get_cursor_pos() noexcept
		{
			glfwGetCursorPos(m_window, &m_cursor.m_pos.x, &m_cursor.m_pos.y);
			return m_cursor.m_pos;
		}



		const glm::vec2& Window::cursor_size() const noexcept
		{
			return m_cursor.m_cursor_size;
		}
*/