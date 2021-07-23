///
/// Window.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <stb/stb_image.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/resource/ShaderBook.hpp"

#include "Window.hpp"

namespace galaxy
{
	namespace core
	{
		Window::Window() noexcept
			: m_window {nullptr}
			, m_width {1280}
			, m_height {720}
			, m_post_processor {nullptr}
		{
		}

		Window::Window(const WindowSettings& settings)
			: m_window {nullptr}
			, m_width {1280}
			, m_height {720}
			, m_post_processor {nullptr}
		{
			if (!create(settings))
			{
				GALAXY_LOG(GALAXY_FATAL, "GLFW window creation failed.");
			}
		}

		Window::~Window()
		{
			// Call again to ensure everything is cleaned up.
			// Has checks to ensure no null data is destroyed.
			destroy();
		}

		const bool Window::create(const WindowSettings& settings)
		{
			// Function result.
			bool result = true;

			// Window w/h.
			if (settings.m_width != 0)
			{
				m_width = settings.m_width;
			}

			if (settings.m_height != 0)
			{
				m_height = settings.m_height;
			}

			// Error callbacks.
			// clang-format off
			glfwSetErrorCallback([](int error, const char* description) {
				GALAXY_LOG(GALAXY_ERROR, "[GLFW] Code: {0}. Desc: {1}.", error, description);
			});
			// clang-format on

			// Init glfw.
			if (!glfwInit())
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to initialize glfw!");
			}
			else
			{
				// Set the OpenGL flags.
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

				// Set debug when compiling for debug mode.
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings.m_gl_debug);

				// Window related hints.
				glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
				glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
				glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
				glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

				// Framebuffer stuff.
				glfwWindowHint(GLFW_RED_BITS, 8);
				glfwWindowHint(GLFW_GREEN_BITS, 8);
				glfwWindowHint(GLFW_BLUE_BITS, 8);
				glfwWindowHint(GLFW_ALPHA_BITS, 8);
				glfwWindowHint(GLFW_DEPTH_BITS, 24);
				glfwWindowHint(GLFW_STENCIL_BITS, 8);
				glfwWindowHint(GLFW_SAMPLES, 0);
				glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
				glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

				// Window maximized check.
				if (settings.m_maximized)
				{
					glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
				}

				// Create the window from input, ensuring it is centered in the screen.
				m_window = glfwCreateWindow(m_width, m_height, settings.m_title.c_str(), nullptr, nullptr);
				if (settings.m_maximized)
				{
					glfwGetWindowSize(m_window, &m_width, &m_height);
				}

				// Then if the window failed to create:
				if (!m_window)
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create window.");
					result = false;
				}
				else
				{
					// Set window context and aspect ratio.
					glfwMakeContextCurrent(m_window);

					if (!(settings.m_aspect_ratio_x == -1 || settings.m_aspect_ratio_y == -1))
					{
						glfwSetWindowAspectRatio(m_window, settings.m_aspect_ratio_x, settings.m_aspect_ratio_y);
					}

					// Set up glad.
					if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to init glad.");
						result = false;
					}
					else
					{
						// Set internal pointer references.
						glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));

						// Set resize callback.
						// clang-format off
						glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							this_win->m_event_queue.emplace<events::WindowResized>({width, height});
							this_win->resize(width, height);
						});

						// Set vsync.
						glfwSwapInterval(settings.m_vsync);
						
						// Key input callback.
						glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							switch (action)
							{
								case GLFW_PRESS:
									this_win->m_event_queue.emplace<events::KeyDown>({this_win->m_keyboard.m_reverse_keymap[key], static_cast<input::KeyMod>(mods)});
									break;

								case GLFW_REPEAT:
									this_win->m_event_queue.emplace<events::KeyRepeat>({this_win->m_keyboard.m_reverse_keymap[key], static_cast<input::KeyMod>(mods)});
									break;

								case GLFW_RELEASE:
									this_win->m_event_queue.emplace<events::KeyUp>({this_win->m_keyboard.m_reverse_keymap[key], static_cast<input::KeyMod>(mods)});
									break;
							}
						});

						// Text input callback.
						glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							if (this_win->m_keyboard.m_inputting_text)
							{
								this_win->m_keyboard.m_text_input += static_cast<char>(codepoint);
							}
						});

						// Mouse movement callback.
						glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							this_win->m_event_queue.emplace<events::MouseMoved>({xpos, ypos});
						});

						// Mouse button callback.
						glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							const auto pos = this_win->get_cursor_pos();
							switch (action)
							{
								case GLFW_PRESS:
									this_win->m_event_queue.emplace<events::MousePressed>({pos.x, pos.y, this_win->m_mouse.m_reverse_mouse_map[button]});
									break;

								case GLFW_RELEASE:
									this_win->m_event_queue.emplace<events::MouseReleased>({pos.x, pos.y, this_win->m_mouse.m_reverse_mouse_map[button]});
									break;
							}
						});

						// Set scroll wheel callback.
						glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							this_win->m_event_queue.emplace<events::MouseWheel>({xoffset, yoffset});
							this_win->m_mouse.m_scroll_delta = yoffset;
						});
						// clang-format on

						if (settings.m_gl_debug)
						{
							glEnable(GL_DEBUG_OUTPUT);
							glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

							// clang-format off
							glDebugMessageCallback(
							[](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
							{
								switch (severity)
								{
									case GL_DEBUG_SEVERITY_HIGH: GALAXY_LOG(GALAXY_ERROR, "[OpenGL] - {0}", message); break;
									case GL_DEBUG_SEVERITY_MEDIUM: GALAXY_LOG(GALAXY_WARNING, "[OpenGL] - {0}", message); break;
									case GL_DEBUG_SEVERITY_LOW: GALAXY_LOG(GALAXY_DEBUG, "[OpenGL] - {0}", message); break;
									case GL_DEBUG_SEVERITY_NOTIFICATION: GALAXY_LOG(GALAXY_INFO, "[OpenGL] - {0}", message); break;
								};
							},
							nullptr);
							// clang-format on

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

						// Create Post Processor.
						m_post_processor = std::make_unique<graphics::PostProcessor>();
					}
				}
			}

			return result;
		}

		void Window::set_icon(std::string_view icon)
		{
			const auto path = SL_HANDLE.vfs()->absolute(icon);
			if (path == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find window icon: {0}.", icon);
			}
			else
			{
				// Fill glfw-compatible struct.
				stbi_set_flip_vertically_on_load(true);
				GLFWimage img = {};
				img.pixels    = stbi_load(path.value().c_str(), &img.width, &img.height, nullptr, STBI_rgb_alpha);

				if (!img.pixels)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load image: {0}.", path.value());
				}
				else
				{
					// Copies data so safe to destroy.
					glfwSetWindowIcon(m_window, 1, &img);
				}

				stbi_image_free(img.pixels);
			}
		}

		void Window::set_icon(std::span<unsigned char> buffer)
		{
			stbi_set_flip_vertically_on_load(true);

			// Fill glfw-compatible struct.
			GLFWimage img = {};
			img.pixels    = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &img.width, &img.height, nullptr, STBI_rgb_alpha);

			if (!img.pixels)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load image for window icon from memory.");
			}
			else
			{
				// Copies data so safe to destroy.
				glfwSetWindowIcon(m_window, 1, &img);
			}

			stbi_image_free(img.pixels);
		}

		void Window::set_title(std::string_view title)
		{
			glfwSetWindowTitle(m_window, static_cast<std::string>(title).c_str());
		}

		void Window::set_cursor_visibility(const bool visible) noexcept
		{
			if (visible)
			{
				glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				// Raw mouse input.
				if (glfwRawMouseMotionSupported())
				{
					glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
				}

				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}

		void Window::set_cursor_icon(std::string_view icon)
		{
			const auto path = SL_HANDLE.vfs()->absolute(icon);
			if (path == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find cursor icon: {0}.", icon);
			}
			else
			{
				// Fill glfw-compatible struct.
				stbi_set_flip_vertically_on_load(false);
				GLFWimage img = {};
				img.pixels    = stbi_load(path.value().c_str(), &img.width, &img.height, nullptr, STBI_rgb_alpha);

				if (!img.pixels)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load image: {0}.", path.value());
				}
				else
				{
					// Copies data so safe to destroy.
					m_cursor.m_data = glfwCreateCursor(&img, 0, 0);
					glfwSetCursor(m_window, m_cursor.m_data);
				}

				m_cursor.m_cursor_size = {img.width, img.height};

				stbi_image_free(img.pixels);
			}
		}

		void Window::set_cursor_icon(std::span<unsigned char> buffer)
		{
			stbi_set_flip_vertically_on_load(false);

			// Fill glfw-compatible struct.
			GLFWimage img = {};
			img.pixels    = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &img.width, &img.height, nullptr, STBI_rgb_alpha);

			if (!img.pixels)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load image for cursor icon from memory.");
			}
			else
			{
				// Copies data so safe to destroy.
				m_cursor.m_data = glfwCreateCursor(&img, 0, 0);
				glfwSetCursor(m_window, m_cursor.m_data);
			}

			m_cursor.m_cursor_size = {img.width, img.height};

			stbi_image_free(img.pixels);
		}

		void Window::destroy()
		{
			end_text_input();

			// Clean up window data, checking to make sure its not already been destroyed.
			if (m_window != nullptr)
			{
				glfwDestroyWindow(m_window);
				m_window = nullptr;
			}

			// Cursor.
			m_cursor.destroy();

			glfwTerminate();
		}

		const bool Window::is_open() const noexcept
		{
			return (!glfwWindowShouldClose(m_window));
		}

		void Window::close() noexcept
		{
			glfwSetWindowShouldClose(m_window, true);
		}

		void Window::resize(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			m_post_processor->resize(m_width, m_height);
			glfwSetWindowSize(m_window, m_width, m_height);
		}

		void Window::request_attention() noexcept
		{
			glfwRequestWindowAttention(m_window);
		}

		void Window::allow_native_closing() noexcept
		{
			glfwSetWindowCloseCallback(m_window, nullptr);
		}

		void Window::prevent_native_closing() noexcept
		{
			// clang-format off
			glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
				glfwSetWindowShouldClose(window, GLFW_FALSE);
			});
			// clang-format on
		}

		void Window::begin()
		{
			RENDERER_2D().prepare();
			m_post_processor->bind();
		}

		void Window::end()
		{
			RENDERER_2D().draw();
			m_post_processor->render_effects();

			// Final Output.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_width, m_height);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_post_processor->render_output();

			glfwSwapBuffers(m_window);
		}

		void Window::poll_events() noexcept
		{
			while (!m_event_queue.empty())
			{
				m_event_queue.pop();
			}

			glfwPollEvents();
		}

		const bool Window::key_down(input::Keys key) noexcept
		{
			return glfwGetKey(m_window, m_keyboard.m_keymap[key]) == GLFW_PRESS;
		}

		const bool Window::key_pressed(input::Keys key) noexcept
		{
			bool res        = false;
			const int state = glfwGetKey(m_window, m_keyboard.m_keymap[key]);
			if (m_keyboard.m_prev_key_states[key] == GLFW_RELEASE && state == GLFW_PRESS)
			{
				res = true;
			}

			m_keyboard.m_prev_key_states[key] = state;
			return res;
		}

		std::string* Window::begin_text_input() noexcept
		{
			m_keyboard.m_text_input     = "";
			m_keyboard.m_inputting_text = true;

			return &m_keyboard.m_text_input;
		}

		void Window::end_text_input() noexcept
		{
			m_keyboard.m_inputting_text = false;
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

		void Window::trigger_queued_events(events::Dispatcher& dispatcher)
		{
			while (!m_event_queue.empty())
			{
				// clang-format off
				std::visit([&](auto&& event)
				{
					dispatcher.trigger<std::decay<decltype(event)>::type>(event);
				}, m_event_queue.front());
				// clang-format on

				m_event_queue.pop();
			}
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

		EventQueue& Window::queued_events() noexcept
		{
			return m_event_queue;
		}

		const bool Window::is_focused() noexcept
		{
			return glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
		}

		const int Window::get_width() const noexcept
		{
			return m_width;
		}

		const int Window::get_height() const noexcept
		{
			return m_height;
		}

		const glm::vec2& Window::cursor_size() const noexcept
		{
			return m_cursor.m_cursor_size;
		}

		GLFWwindow* Window::gl_window() noexcept
		{
			return m_window;
		}
	} // namespace core
} // namespace galaxy