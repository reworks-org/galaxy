///
/// Window.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/meta/Globals.hpp"

#include "Window.hpp"

namespace galaxy
{
	namespace graphics
	{
		Window::Window() noexcept
			: m_window {nullptr}
		{
		}

		Window::Window(const WindowSettings& settings)
			: m_window {nullptr}
		{
			if (!open(settings))
			{
				GALAXY_LOG(GALAXY_FATAL, "GLFW window failed to open.");
			}
		}

		Window::~Window()
		{
			// Call again to ensure everything is cleaned up.
			// Has checks to ensure no null data is destroyed.
			destroy();
		}

		bool Window::open(const WindowSettings& settings)
		{
			glfwSetErrorCallback([](int error, const char* description) {
				GALAXY_LOG(GALAXY_ERROR, "[GLFW] Code: {0}, Desc: {1}.", error, description);
			});

			if (!glfwInit())
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to initialize glfw!");
			}
			else
			{
				// Set the OpenGL flags.
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

				// Set debug when compiling for debug mode.
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings.m_debug);

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
				m_window = glfwCreateWindow(settings.m_width, settings.m_height, settings.m_title.c_str(), nullptr, nullptr);
				if (!m_window)
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create window.");
				}
				else
				{
					// Set window context and aspect ratio.
					glfwMakeContextCurrent(m_window);

					glfwSetWindowAspectRatio(m_window, settings.m_aspect_ratio_x, settings.m_aspect_ratio_y);
					glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));
					glfwGetWindowSize(m_window, &m_width, &m_height);

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
										case GL_DEBUG_SEVERITY_NOTIFICATION:
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
					}
				}
			}
		}

		void Window::set_title(const char* title)
		{
			glfwSetWindowTitle(m_window, title);
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
			// Clean up window data, checking to make sure its not already been destroyed.
			if (m_window != nullptr)
			{
				glfwDestroyWindow(m_window);
				m_window = nullptr;
			}

			glfwTerminate();
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
			glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
				glfwSetWindowShouldClose(window, GLFW_FALSE);
			});
		}

		bool Window::is_focused() noexcept
		{
			return glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
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
	} // namespace graphics
} // namespace galaxy