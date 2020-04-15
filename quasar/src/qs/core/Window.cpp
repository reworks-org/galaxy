///
/// Window.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <stb_image.h>
#include <stb_image_write.h>

#include "qs/utils/Error.hpp"
#include "qs/utils/Utility.hpp"
#include "qs/core/WindowSettings.hpp"

#include "Window.hpp"

///
/// Core namespace.
///
namespace qs
{
	Window::Window() noexcept
		:m_window(nullptr), m_cursor(nullptr), m_width(0), m_height(0)
	{
	}

	Window::Window(const std::string& title, int w, int h)
		:m_window(nullptr), m_cursor(nullptr), m_width(0), m_height(0)
	{
		if (!create(title, w, h))
		{
			qs::Error::handle().callback("Window.cpp", 37, "Window creation failed!");
		}
	}

	Window::~Window() noexcept
	{
		// Call again to ensure everything is cleaned up.
		// Has checks to ensure no null data is destroyed.
		destroy();
	}

	bool Window::create(const std::string& title, int w, int h)
	{
		// Function result.
		bool result = true;

		// Window w/h.
		m_width = w;
		m_height = h;

		// Error callbacks.
		glfwSetErrorCallback([](int error, const char* description)
		{
			std::string msg = "Error Code: " + std::to_string(error) + ". Desc: " + description + ".";
			qs::Error::handle().callback("GLFW ERROR CALLBACK", 0, msg);
		});

		// Init glfw.
		if (!glfwInit())
		{
			qs::Error::handle().callback("Window.cpp", 67, "Failed to initialize glfw!");
		}
		else
		{
			// Set the OpenGL flags.
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

			// Set debug when compiling for debug mode.
			#ifdef _DEBUG
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
			#endif

			// Window related hints.
			glfwWindowHint(GLFW_RESIZABLE, true);
			glfwWindowHint(GLFW_VISIBLE, true);
			glfwWindowHint(GLFW_DECORATED, true);
			glfwWindowHint(GLFW_FOCUSED, true);

			// Framebuffer stuff
			glfwWindowHint(GLFW_RED_BITS, 8);
			glfwWindowHint(GLFW_GREEN_BITS, 8);
			glfwWindowHint(GLFW_BLUE_BITS, 8);
			glfwWindowHint(GLFW_ALPHA_BITS, 8);
			glfwWindowHint(GLFW_DEPTH_BITS, 24);
			glfwWindowHint(GLFW_STENCIL_BITS, 8);
			glfwWindowHint(GLFW_DOUBLEBUFFER, true);

			// sRGB.
			glfwWindowHint(GLFW_SRGB_CAPABLE, qs::WindowSettings::s_srgb);

			// MSAA
			glfwWindowHint(GLFW_SAMPLES, qs::WindowSettings::s_msaa);

			// Create the window from input, ensuring it is centered in the screen.
			m_window = glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);

			// Then if the window failed to create:
			if (!m_window)
			{
				qs::Error::handle().callback("Window.cpp", 109, "Failed to create window!");
				result = false;
			}
			else
			{
				// Set window context and aspect ratio.
				glfwMakeContextCurrent(m_window);
				
				if (!(qs::WindowSettings::s_aspectRatioX == -1 || qs::WindowSettings::s_aspectRatioY == -1))
				{
					glfwSetWindowAspectRatio(m_window, qs::WindowSettings::s_aspectRatioX, qs::WindowSettings::s_aspectRatioY);
				}

				// Set up glad gl loader with SDL2.
				if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
				{
					std::string msg = "Failed to init glad.";

					qs::Error::handle().callback("Window.cpp", 123, msg);
					result = false;
				}
				else
				{
					// Set internal pointer references.
					glfwSetWindowUserPointer(m_window, static_cast<void*>(this));

					// Set resize callback.
					glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int w, int h)
						{
							static_cast<qs::Window*>(glfwGetWindowUserPointer(window))->resize(w, h);
						});

					// Set vsync.
					glfwSwapInterval(qs::WindowSettings::s_vsync);

					// Raw mouse input.
					if (glfwRawMouseMotionSupported())
					{
						glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, qs::WindowSettings::s_rawMouseInput);
					}

					// Enable MSAA.
					glEnable(GL_MULTISAMPLE);

					// Depth testing.
					glEnable(GL_DEPTH_TEST);

					// Set up blending.
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					// Allow for chaning vertex point size.
					glEnable(GL_PROGRAM_POINT_SIZE);

					// Print OpenGL version.
					std::string msg = "OpenGL v" + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
					qs::Error::handle().callback("Window.cpp", 155, msg);
				}
			}
		}

		return result;
	}

	void Window::setIcon(const std::string& icon) noexcept
	{
		// Always convert to proper path before loading.
		auto path = std::filesystem::path(icon);
		int w = 0, h = 0;
		GLFWimage img;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load(path.string().c_str(), &w, &h, nullptr, STBI_rgb_alpha);

		// Fill glfw-compatible struct.
		img.height = h;
		img.width = w;
		img.pixels = pixels;

		// Copies data so safe to destroy.
		glfwSetWindowIcon(m_window, 1, &img);

		// Make sure is all cleaned up.
		stbi_image_free(pixels);
	}

	void Window::setIcon(const unsigned char* mem, const int size) noexcept
	{
		// Setup variables with default data.
		int w = 0, h = 0;
		GLFWimage img;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load_from_memory(mem, size, &w, &h, nullptr, STBI_rgb_alpha);

		// Fill glfw-compatible struct.
		img.height = h;
		img.width = w;
		img.pixels = pixels;

		// Copies data so safe to destroy.
		glfwSetWindowIcon(m_window, 1, &img);

		// Make sure is all cleaned up.
		stbi_image_free(pixels);
	}

	void Window::setCursorVisibility(const bool visible) noexcept
	{
		if (visible)
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
	}

	void Window::removeCursor() noexcept
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Window::setCursorIcon(const std::string& icon) noexcept
	{
		// Always convert to proper path before loading.
		auto path = std::filesystem::path(icon);
		int w = 0, h = 0;
		GLFWimage img;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load(path.string().c_str(), &w, &h, nullptr, STBI_rgb_alpha);

		// Fill glfw-compatible struct.
		img.height = h;
		img.width = w;
		img.pixels = pixels;

		// Copies data so safe to destroy.
		m_cursor = glfwCreateCursor(&img, 0, 0);

		stbi_image_free(pixels);
		glfwSetCursor(m_window, m_cursor);
	}

	void Window::setCursorIcon(const unsigned char* mem, const int size) noexcept
	{
		// Setup variables with default data.
		int w = 0, h = 0;
		GLFWimage img;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load_from_memory(mem, size, &w, &h, nullptr, STBI_rgb_alpha);

		// Fill glfw-compatible struct.
		img.height = h;
		img.width = w;
		img.pixels = pixels;

		// Copies data so safe to destroy.
		m_cursor = glfwCreateCursor(&img, 0, 0);

		stbi_image_free(pixels);
		glfwSetCursor(m_window, m_cursor);
	}

	void Window::destroy() noexcept
	{
		// Clean up window data, checking to make sure its not already been destroyed.
		if (m_window != nullptr)
		{
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}

		// Cursor
		if (m_cursor != nullptr)
		{
			glfwDestroyCursor(m_cursor);
		}

		glfwTerminate();
	}

	bool Window::isOpen() const noexcept
	{
		return (!glfwWindowShouldClose(m_window));
	}

	void Window::close() noexcept
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	void Window::resize(int w, int h) noexcept
	{
		m_width = w;
		m_height = h;

		glfwSetWindowSize(m_window, w, h);
		glViewport(0, 0, m_width, m_height);
	}

	void Window::makeCurrent() noexcept
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_width, m_height);
	}

	void Window::requestAttention() noexcept
	{
		glfwRequestWindowAttention(m_window);
	}

	void Window::begin(const protostar::Colour& colour) noexcept
	{
		glClearColor(qs::Utils::uint8ToFloat(colour.m_red), qs::Utils::uint8ToFloat(colour.m_green), qs::Utils::uint8ToFloat(colour.m_blue), qs::Utils::uint8ToFloat(colour.m_alpha));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::end() noexcept
	{
		glfwSwapBuffers(m_window);
	}

	GLFWwindow* Window::getWindow() noexcept
	{
		return m_window;
	}

	const int Window::getWidth() const noexcept
	{
		return m_width;
	}

	const int Window::getHeight() const noexcept
	{
		return m_height;
	}
}