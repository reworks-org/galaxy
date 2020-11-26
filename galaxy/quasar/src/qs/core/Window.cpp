///
/// Window.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <pulsar/Log.hpp>
#include <stb_image.h>
#include <stb_image_write.h>

#include "qs/core/Renderer.hpp"
#include "qs/core/WindowSettings.hpp"
#include "qs/post/PostEffect.hpp"

#include "Window.hpp"

///
/// Core namespace.
///
namespace qs
{
	Window::Window()
	    : m_window {nullptr}, m_cursor {nullptr}, m_width {0}, m_height {0}, m_colour {1.0f, 1.0f, 1.0f, 1.0f}, m_text_input {""}, m_inputting_text {false}, m_framebuffer {nullptr}, m_fb_sprite {nullptr}
	{
		m_prev_mouse_btn_states = {GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE};
	}

	Window::Window(qs::WindowSettings& settings)
	    : m_window {nullptr}, m_cursor {nullptr}, m_width {0}, m_height {0}, m_colour {1.0f, 1.0f, 1.0f, 1.0f}, m_text_input {""}, m_inputting_text {false}, m_framebuffer {nullptr}, m_fb_sprite {nullptr}
	{
		m_prev_mouse_btn_states = {GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE, GLFW_RELEASE};

		if (!create(settings))
		{
			PL_LOG(PL_FATAL, "GLFW window creation failed.");
		}
	}

	Window::~Window()
	{
		// Call again to ensure everything is cleaned up.
		// Has checks to ensure no null data is destroyed.
		destroy();
	}

	bool Window::create(qs::WindowSettings& settings)
	{
		// Function result.
		bool result = true;

		// Window w/h.
		m_width  = settings.m_width;
		m_height = settings.m_height;

		// Error callbacks.
		glfwSetErrorCallback([](int error, const char* description) {
			PL_LOG(PL_ERROR, "[GLFW] Code: {0}. Desc: {1}.", error, description);
		});

		// Init glfw.
		if (!glfwInit())
		{
			PL_LOG(PL_FATAL, "Failed to initialize glfw!");
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
			glfwWindowHint(GLFW_SRGB_CAPABLE, true);

			// MSAA
			glfwWindowHint(GLFW_SAMPLES, settings.m_anti_aliasing);

			// Create the window from input, ensuring it is centered in the screen.
			m_window = glfwCreateWindow(m_width, m_height, settings.m_title.c_str(), nullptr, nullptr);

			// Then if the window failed to create:
			if (!m_window)
			{
				PL_LOG(PL_FATAL, "Failed to create window.");
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
					PL_LOG(PL_FATAL, "Failed to init glad.");
					result = false;
				}
				else
				{
					// Set internal pointer references.
					glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));

					// Set resize callback.
					glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
						reinterpret_cast<qs::Window*>(glfwGetWindowUserPointer(window))->resize(w, h);
					});

					// Set vsync.
					glfwSwapInterval(settings.m_vsync);

					// Raw mouse input.
					if (glfwRawMouseMotionSupported())
					{
						glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, settings.m_raw_mouse_input);
					}

					glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
						qs::Window* qs_win = reinterpret_cast<qs::Window*>(glfwGetWindowUserPointer(window));
						if (qs_win->m_inputting_text)
						{
							qs_win->m_text_input += static_cast<char>(codepoint);
						}
					});

					if (settings.m_gl_debug)
					{
						glEnable(GL_DEBUG_OUTPUT);
						glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) -> void {
							PL_LOG(PL_WARNING, "[OpenGL] - [ {0}, {1}, {2}, {3}, {4} ] - {5}.", source, type, id, severity, length, message);
						},
								       nullptr);
					}

					// Enable MSAA.
					glEnable(GL_MULTISAMPLE);

					// Set up blending.
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					// Allow for changing vertex point size.
					glEnable(GL_PROGRAM_POINT_SIZE);

					// sRGB colours for rendering.
					if (settings.m_srgb)
					{
						glEnable(GL_FRAMEBUFFER_SRGB);
					}

					// Set custom line width.
					glLineWidth(settings.m_line_thickness);

					m_keymap.reserve(102);
					m_keymap.emplace(pr::Keys::A, GLFW_KEY_A);
					m_keymap.emplace(pr::Keys::B, GLFW_KEY_B);
					m_keymap.emplace(pr::Keys::C, GLFW_KEY_C);
					m_keymap.emplace(pr::Keys::D, GLFW_KEY_D);
					m_keymap.emplace(pr::Keys::E, GLFW_KEY_E);
					m_keymap.emplace(pr::Keys::F, GLFW_KEY_F);
					m_keymap.emplace(pr::Keys::G, GLFW_KEY_G);
					m_keymap.emplace(pr::Keys::H, GLFW_KEY_H);
					m_keymap.emplace(pr::Keys::I, GLFW_KEY_I);
					m_keymap.emplace(pr::Keys::J, GLFW_KEY_J);
					m_keymap.emplace(pr::Keys::K, GLFW_KEY_K);
					m_keymap.emplace(pr::Keys::L, GLFW_KEY_L);
					m_keymap.emplace(pr::Keys::M, GLFW_KEY_M);
					m_keymap.emplace(pr::Keys::N, GLFW_KEY_N);
					m_keymap.emplace(pr::Keys::O, GLFW_KEY_O);
					m_keymap.emplace(pr::Keys::P, GLFW_KEY_P);
					m_keymap.emplace(pr::Keys::Q, GLFW_KEY_Q);
					m_keymap.emplace(pr::Keys::R, GLFW_KEY_R);
					m_keymap.emplace(pr::Keys::S, GLFW_KEY_S);
					m_keymap.emplace(pr::Keys::T, GLFW_KEY_T);
					m_keymap.emplace(pr::Keys::U, GLFW_KEY_U);
					m_keymap.emplace(pr::Keys::V, GLFW_KEY_V);
					m_keymap.emplace(pr::Keys::W, GLFW_KEY_W);
					m_keymap.emplace(pr::Keys::X, GLFW_KEY_X);
					m_keymap.emplace(pr::Keys::Y, GLFW_KEY_Y);
					m_keymap.emplace(pr::Keys::Z, GLFW_KEY_Z);
					m_keymap.emplace(pr::Keys::NUM_1, GLFW_KEY_1);
					m_keymap.emplace(pr::Keys::NUM_2, GLFW_KEY_2);
					m_keymap.emplace(pr::Keys::NUM_3, GLFW_KEY_3);
					m_keymap.emplace(pr::Keys::NUM_4, GLFW_KEY_4);
					m_keymap.emplace(pr::Keys::NUM_5, GLFW_KEY_5);
					m_keymap.emplace(pr::Keys::NUM_6, GLFW_KEY_6);
					m_keymap.emplace(pr::Keys::NUM_7, GLFW_KEY_7);
					m_keymap.emplace(pr::Keys::NUM_8, GLFW_KEY_8);
					m_keymap.emplace(pr::Keys::NUM_9, GLFW_KEY_9);
					m_keymap.emplace(pr::Keys::NUM_0, GLFW_KEY_0);
					m_keymap.emplace(pr::Keys::MINUS, GLFW_KEY_MINUS);
					m_keymap.emplace(pr::Keys::EQUALS, GLFW_KEY_EQUAL);
					m_keymap.emplace(pr::Keys::BACKSPACE, GLFW_KEY_BACKSPACE);
					m_keymap.emplace(pr::Keys::GRAVE, GLFW_KEY_GRAVE_ACCENT);
					m_keymap.emplace(pr::Keys::TAB, GLFW_KEY_TAB);
					m_keymap.emplace(pr::Keys::CAPS, GLFW_KEY_CAPS_LOCK);
					m_keymap.emplace(pr::Keys::LSHIFT, GLFW_KEY_LEFT_SHIFT);
					m_keymap.emplace(pr::Keys::LCNTRL, GLFW_KEY_LEFT_CONTROL);
					m_keymap.emplace(pr::Keys::LSTART, GLFW_KEY_LEFT_SUPER);
					m_keymap.emplace(pr::Keys::LALT, GLFW_KEY_LEFT_ALT);
					m_keymap.emplace(pr::Keys::SPACE, GLFW_KEY_SPACE);
					m_keymap.emplace(pr::Keys::RALT, GLFW_KEY_RIGHT_ALT);
					m_keymap.emplace(pr::Keys::RSTART, GLFW_KEY_RIGHT_SUPER);
					m_keymap.emplace(pr::Keys::MENU, GLFW_KEY_MENU);
					m_keymap.emplace(pr::Keys::RCNTRL, GLFW_KEY_RIGHT_CONTROL);
					m_keymap.emplace(pr::Keys::RSHIFT, GLFW_KEY_RIGHT_SHIFT);
					m_keymap.emplace(pr::Keys::ENTER, GLFW_KEY_ENTER);
					m_keymap.emplace(pr::Keys::SEMICOLON, GLFW_KEY_SEMICOLON);
					m_keymap.emplace(pr::Keys::APOSTROPHE, GLFW_KEY_APOSTROPHE);
					m_keymap.emplace(pr::Keys::SLASH, GLFW_KEY_SLASH);
					m_keymap.emplace(pr::Keys::PERIOD, GLFW_KEY_PERIOD);
					m_keymap.emplace(pr::Keys::COMMA, GLFW_KEY_COMMA);
					m_keymap.emplace(pr::Keys::LBRACKET, GLFW_KEY_LEFT_BRACKET);
					m_keymap.emplace(pr::Keys::RBRACKET, GLFW_KEY_RIGHT_BRACKET);
					m_keymap.emplace(pr::Keys::BACKSLASH, GLFW_KEY_BACKSLASH);
					m_keymap.emplace(pr::Keys::ESC, GLFW_KEY_ESCAPE);
					m_keymap.emplace(pr::Keys::F1, GLFW_KEY_F1);
					m_keymap.emplace(pr::Keys::F2, GLFW_KEY_F2);
					m_keymap.emplace(pr::Keys::F3, GLFW_KEY_F3);
					m_keymap.emplace(pr::Keys::F4, GLFW_KEY_F4);
					m_keymap.emplace(pr::Keys::F5, GLFW_KEY_F5);
					m_keymap.emplace(pr::Keys::F6, GLFW_KEY_F6);
					m_keymap.emplace(pr::Keys::F7, GLFW_KEY_F7);
					m_keymap.emplace(pr::Keys::F8, GLFW_KEY_F8);
					m_keymap.emplace(pr::Keys::F9, GLFW_KEY_F9);
					m_keymap.emplace(pr::Keys::F10, GLFW_KEY_F10);
					m_keymap.emplace(pr::Keys::F11, GLFW_KEY_F11);
					m_keymap.emplace(pr::Keys::F12, GLFW_KEY_F12);
					m_keymap.emplace(pr::Keys::PRINTSCREEN, GLFW_KEY_PRINT_SCREEN);
					m_keymap.emplace(pr::Keys::SCROLL_LOCK, GLFW_KEY_SCROLL_LOCK);
					m_keymap.emplace(pr::Keys::PAUSE, GLFW_KEY_PAUSE);
					m_keymap.emplace(pr::Keys::INSERT, GLFW_KEY_INSERT);
					m_keymap.emplace(pr::Keys::HOME, GLFW_KEY_HOME);
					m_keymap.emplace(pr::Keys::PAGEUP, GLFW_KEY_PAGE_UP);
					m_keymap.emplace(pr::Keys::PAGEDOWN, GLFW_KEY_PAGE_DOWN);
					m_keymap.emplace(pr::Keys::END, GLFW_KEY_END);
					m_keymap.emplace(pr::Keys::DELETE, GLFW_KEY_DELETE);
					m_keymap.emplace(pr::Keys::UP, GLFW_KEY_UP);
					m_keymap.emplace(pr::Keys::DOWN, GLFW_KEY_DOWN);
					m_keymap.emplace(pr::Keys::LEFT, GLFW_KEY_LEFT);
					m_keymap.emplace(pr::Keys::RIGHT, GLFW_KEY_RIGHT);
					m_keymap.emplace(pr::Keys::NUMLOCK, GLFW_KEY_NUM_LOCK);
					m_keymap.emplace(pr::Keys::NUMPAD_DIVIDE, GLFW_KEY_KP_DIVIDE);
					m_keymap.emplace(pr::Keys::NUMPAD_MULTIPLY, GLFW_KEY_KP_MULTIPLY);
					m_keymap.emplace(pr::Keys::NUMPAD_ADD, GLFW_KEY_KP_ADD);
					m_keymap.emplace(pr::Keys::NUMPAD_ENTER, GLFW_KEY_KP_ENTER);
					m_keymap.emplace(pr::Keys::NUMPAD_PERIOD, GLFW_KEY_KP_DECIMAL);
					m_keymap.emplace(pr::Keys::NUMPAD_0, GLFW_KEY_KP_0);
					m_keymap.emplace(pr::Keys::NUMPAD_1, GLFW_KEY_KP_1);
					m_keymap.emplace(pr::Keys::NUMPAD_2, GLFW_KEY_KP_2);
					m_keymap.emplace(pr::Keys::NUMPAD_3, GLFW_KEY_KP_3);
					m_keymap.emplace(pr::Keys::NUMPAD_4, GLFW_KEY_KP_4);
					m_keymap.emplace(pr::Keys::NUMPAD_5, GLFW_KEY_KP_5);
					m_keymap.emplace(pr::Keys::NUMPAD_6, GLFW_KEY_KP_6);
					m_keymap.emplace(pr::Keys::NUMPAD_7, GLFW_KEY_KP_7);
					m_keymap.emplace(pr::Keys::NUMPAD_8, GLFW_KEY_KP_8);
					m_keymap.emplace(pr::Keys::NUMPAD_9, GLFW_KEY_KP_9);

					m_prev_key_states.reserve(102);
					m_prev_key_states.emplace(pr::Keys::A, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::B, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::C, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::D, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::E, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::G, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::H, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::I, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::J, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::K, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::L, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::M, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::N, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::O, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::P, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::Q, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::R, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::S, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::T, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::U, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::V, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::W, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::X, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::Y, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::Z, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUM_1, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUM_2, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUM_3, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUM_4, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUM_5, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUM_6, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUM_7, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUM_8, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUM_9, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUM_0, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::MINUS, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::EQUALS, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::BACKSPACE, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::GRAVE, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::TAB, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::CAPS, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::LSHIFT, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::LCNTRL, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::LSTART, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::LALT, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::SPACE, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::RALT, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::RSTART, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::MENU, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::RCNTRL, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::RSHIFT, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::ENTER, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::SEMICOLON, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::APOSTROPHE, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::SLASH, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::PERIOD, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::COMMA, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::LBRACKET, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::RBRACKET, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::BACKSLASH, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::ESC, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F1, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F2, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F3, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F4, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F5, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F6, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F7, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F8, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F9, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F10, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F11, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::F12, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::PRINTSCREEN, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::SCROLL_LOCK, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::PAUSE, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::INSERT, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::HOME, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::PAGEUP, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::PAGEDOWN, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::END, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::DELETE, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::UP, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::DOWN, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::LEFT, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::RIGHT, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMLOCK, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_MULTIPLY, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_DIVIDE, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_ADD, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_ENTER, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_PERIOD, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_0, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_1, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_2, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_3, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_4, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_5, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_6, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_7, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_8, GLFW_RELEASE);
					m_prev_key_states.emplace(pr::Keys::NUMPAD_9, GLFW_RELEASE);

					// Setup framebuffer.
					m_framebuffer = std::make_unique<qs::RenderTexture>();
					m_framebuffer->create(m_width, m_height);

					m_fb_sprite = std::make_unique<qs::Sprite>();
					m_fb_sprite->load(m_framebuffer->gl_texture(), m_width, m_height);
					m_fb_sprite->create<qs::BufferStatic>();
				}
			}
		}

		return result;
	}

	void Window::set_window_background(pr::Colour& col)
	{
		m_colour = col.get_normalized();
	}

	void Window::set_icon(std::string_view icon)
	{
		stbi_set_flip_vertically_on_load(true);

		// Always convert to proper path before loading.
		auto path = std::filesystem::path {icon};

		// Fill glfw-compatible struct.
		GLFWimage img;
		img.pixels = stbi_load(path.string().c_str(), &img.width, &img.height, nullptr, STBI_rgb_alpha);
		if (!img.pixels)
		{
			PL_LOG(PL_ERROR, "Failed to load image: {0}.", path.string());
		}
		else
		{
			// Copies data so safe to destroy.
			glfwSetWindowIcon(m_window, 1, &img);
		}

		stbi_image_free(img.pixels);
	}

	void Window::set_icon(std::span<unsigned char> buffer)
	{
		stbi_set_flip_vertically_on_load(true);

		// Fill glfw-compatible struct.
		GLFWimage img;
		img.pixels = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &img.width, &img.height, nullptr, STBI_rgb_alpha);

		if (!img.pixels)
		{
			PL_LOG(PL_ERROR, "Failed to load image for window icon from memory.");
		}
		else
		{
			// Copies data so safe to destroy.
			glfwSetWindowIcon(m_window, 1, &img);
		}

		stbi_image_free(img.pixels);
	}

	void Window::set_cursor_visibility(const bool visible)
	{
		visible == true ? glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL)
				: glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void Window::remove_cursor()
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Window::set_cursor_icon(std::string_view icon)
	{
		stbi_set_flip_vertically_on_load(false);

		// Always convert to proper path before loading.
		auto path = std::filesystem::path {icon};

		// Fill glfw-compatible struct.
		GLFWimage img;
		img.pixels = stbi_load(path.string().c_str(), &img.width, &img.height, nullptr, STBI_rgb_alpha);

		if (!img.pixels)
		{
			PL_LOG(PL_ERROR, "Failed to load image: {0}.", path.string());
		}
		else
		{
			// Copies data so safe to destroy.
			m_cursor.m_glfw = glfwCreateCursor(&img, 0, 0);
			glfwSetCursor(m_window, m_cursor.m_glfw);
		}

		stbi_image_free(img.pixels);
	}

	void Window::set_cursor_icon(std::span<unsigned char> buffer)
	{
		stbi_set_flip_vertically_on_load(false);

		// Fill glfw-compatible struct.
		GLFWimage img;
		img.pixels = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &img.width, &img.height, nullptr, STBI_rgb_alpha);

		if (!img.pixels)
		{
			PL_LOG(PL_ERROR, "Failed to load image for cursor icon from memory.");
		}
		else
		{
			// Copies data so safe to destroy.
			m_cursor.m_glfw = glfwCreateCursor(&img, 0, 0);
			glfwSetCursor(m_window, m_cursor.m_glfw);
		}

		stbi_image_free(img.pixels);
	}

	void Window::set_scrolling_callback(GLFWscrollfun func)
	{
		glfwSetScrollCallback(m_window, func);
	}

	void Window::destroy()
	{
		end_text_input();

		// Clean up window data, checking to make sure its not already been destroyed.
		if (m_window != nullptr)
		{
			m_fb_sprite.reset();
			m_framebuffer.reset();
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}

		// Cursor
		if (m_cursor.m_glfw != nullptr)
		{
			glfwDestroyCursor(m_cursor.m_glfw);
		}

		glfwTerminate();
	}

	bool Window::is_open() const
	{
		return (!glfwWindowShouldClose(m_window));
	}

	void Window::close()
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	void Window::resize(const int width, const int height)
	{
		m_width  = width;
		m_height = height;

		m_framebuffer->change_size(width, height);
		m_fb_sprite->load(m_framebuffer->gl_texture(), m_width, m_height);
		m_fb_sprite->create<qs::BufferStatic>();

		glfwSetWindowSize(m_window, width, height);
	}

	void Window::request_attention()
	{
		glfwRequestWindowAttention(m_window);
	}

	void Window::begin()
	{
		m_framebuffer->bind();
	}

	void Window::end(qs::Renderer* renderer)
	{
		// clang-format off
		m_framebuffer->unbind();

		glViewport(0, 0, m_width, m_height);
		glClearColor(m_colour[0], m_colour[1], m_colour[2], m_colour[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		#ifdef _DEBUG
			if (renderer->get_post_effects().empty())
			{
				PL_LOG(PL_FATAL, "Failed to set a post effect. At least 1 required.");
			}
		#endif
		// clang-format on

		m_fb_sprite->bind();
		for (qs::PostEffect* effect : renderer->get_post_effects())
		{
			effect->m_shader.bind();
			effect->m_shader.set_uniform("u_projection", m_framebuffer->get_proj());
			effect->m_shader.set_uniform("u_transform", m_fb_sprite->get_transform());
			effect->m_shader.set_uniform("u_width", static_cast<float>(m_fb_sprite->get_width()));
			effect->m_shader.set_uniform("u_height", static_cast<float>(m_fb_sprite->get_height()));
			effect->apply_uniforms();

			glDrawElements(GL_TRIANGLES, m_fb_sprite->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		glfwSwapBuffers(m_window);
	}

	void Window::poll_events()
	{
		glfwPollEvents();
	}

	bool Window::mouse_button_pressed(int mouse_button)
	{
		bool res = false;

		if (!(mouse_button < 0 || mouse_button > 7))
		{
			int state = glfwGetMouseButton(m_window, mouse_button);
			if (state == GLFW_PRESS && m_prev_mouse_btn_states[mouse_button] == GLFW_RELEASE)
			{
				res = true;
			}

			m_prev_mouse_btn_states[mouse_button] = state;
		}

		return res;
	}

	bool Window::mouse_button_released(int mouse_button)
	{
		bool res = false;

		if (!(mouse_button < 0 || mouse_button > 7))
		{
			int state = glfwGetMouseButton(m_window, mouse_button);
			if (state == GLFW_RELEASE && m_prev_mouse_btn_states[mouse_button] == GLFW_PRESS)
			{
				res = true;
			}

			m_prev_mouse_btn_states[mouse_button] = state;
		}

		return res;
	}

	bool Window::key_down(pr::Keys key)
	{
		return glfwGetKey(m_window, m_keymap[key]) == GLFW_PRESS;
	}

	bool Window::key_pressed(pr::Keys key)
	{
		bool res = false;

		int state = glfwGetKey(m_window, m_keymap[key]);
		if (m_prev_key_states[key] == GLFW_RELEASE && state == GLFW_PRESS)
		{
			res = true;
		}

		m_prev_key_states[key] = state;
		return res;
	}

	std::string* Window::begin_text_input()
	{
		m_text_input     = "";
		m_inputting_text = true;

		return &m_text_input;
	}

	void Window::end_text_input()
	{
		m_inputting_text = false;
	}

	std::tuple<bool, glm::vec2> Window::get_cursor_pos()
	{
		glfwGetCursorPos(m_window, &m_cursor.m_pos.x, &m_cursor.m_pos.y);

		if (m_cursor.m_pos != m_cursor.m_prev_pos)
		{
			m_cursor.m_prev_pos = m_cursor.m_pos;
			return std::make_tuple(true, m_cursor.m_pos);
		}

		return std::make_tuple(false, m_cursor.m_pos);
	}

	GLFWwindow* Window::gl_window()
	{
		return m_window;
	}

	const int Window::get_width() const
	{
		return m_width;
	}

	const int Window::get_height() const
	{
		return m_height;
	}
} // namespace qs