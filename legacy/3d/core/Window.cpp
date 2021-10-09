///
/// Window.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/events/KeyRepeat.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/graphics/Renderer3D.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "Window.hpp"

namespace galaxy
{
	namespace core
	{
		Window::Cursor::Cursor() noexcept
		    : m_glfw {nullptr}, m_pos {0.0, 0.0}, m_prev_pos {0.0, 0.0}
		{
		}

		Window::Window() noexcept
		    : m_window {nullptr}, m_width {800}, m_height {600}, m_colour {1.0f, 1.0f, 1.0f, 1.0f}, m_text_input {""}, m_inputting_text {false}, m_scene_dispatcher {nullptr}, m_cursor_size {0.0, 0.0}, m_scroll_delta {0.0}, m_post_processor {nullptr}
		{
		}

		Window::Window(const WindowSettings& settings)
		    : m_window {nullptr}, m_width {800}, m_height {600}, m_colour {1.0f, 1.0f, 1.0f, 1.0f}, m_text_input {""}, m_inputting_text {false}, m_scene_dispatcher {nullptr}, m_cursor_size {0.0, 0.0}, m_scroll_delta {0.0}, m_post_processor {nullptr}
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
			glfwSetErrorCallback([](int error, const char* description) {
				GALAXY_LOG(GALAXY_ERROR, "[GLFW] Code: {0}. Desc: {1}.", error, description);
			});

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
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

				// Set debug when compiling for debug mode.
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings.m_gl_debug);

				// Window related hints.
				glfwWindowHint(GLFW_RESIZABLE, true);
				glfwWindowHint(GLFW_VISIBLE, true);
				glfwWindowHint(GLFW_DECORATED, true);
				glfwWindowHint(GLFW_FOCUSED, true);

				// Framebuffer stuff.
				glfwWindowHint(GLFW_RED_BITS, 8);
				glfwWindowHint(GLFW_GREEN_BITS, 8);
				glfwWindowHint(GLFW_BLUE_BITS, 8);
				glfwWindowHint(GLFW_ALPHA_BITS, 8);
				glfwWindowHint(GLFW_DEPTH_BITS, 24);
				glfwWindowHint(GLFW_STENCIL_BITS, 8);
				glfwWindowHint(GLFW_SAMPLES, 0);
				glfwWindowHint(GLFW_DOUBLEBUFFER, true);
				glfwWindowHint(GLFW_SRGB_CAPABLE, true);

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
						glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
							this_win->resize(width, height);
						});

						// Set vsync.
						glfwSwapInterval(settings.m_vsync);

						// Raw mouse input.
						if (glfwRawMouseMotionSupported())
						{
							if (settings.m_raw_mouse_input)
							{
								remove_cursor();
							}
						}

						// Key input callback.
						glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							if (this_win->m_scene_dispatcher)
							{
								switch (action)
								{
									case GLFW_PRESS:
										this_win->m_scene_dispatcher->trigger<events::KeyDown>(this_win->m_reverse_keymap[key]);
										break;

									case GLFW_REPEAT:
										this_win->m_scene_dispatcher->trigger<events::KeyRepeat>(this_win->m_reverse_keymap[key]);
										break;

									case GLFW_RELEASE:
										this_win->m_scene_dispatcher->trigger<events::KeyUp>(this_win->m_reverse_keymap[key]);
										break;
								}
							}
						});

						// Text input callback.
						glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							if (this_win->m_inputting_text)
							{
								this_win->m_text_input += static_cast<char>(codepoint);
							}
						});

						// Mouse movement callback.
						glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							if (this_win->m_scene_dispatcher)
							{
								this_win->m_scene_dispatcher->trigger<events::MouseMoved>(xpos, ypos);
							}
						});

						// Mouse button callback.
						glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							if (this_win->m_scene_dispatcher)
							{
								const auto pos = this_win->get_cursor_pos();
								switch (action)
								{
									case GLFW_PRESS:
										this_win->m_scene_dispatcher->trigger<events::MousePressed>(pos.x, pos.y, this_win->m_reverse_mouse_map[button]);
										break;

									case GLFW_RELEASE:
										this_win->m_scene_dispatcher->trigger<events::MouseReleased>(pos.x, pos.y, this_win->m_reverse_mouse_map[button]);
										break;
								}
							}
						});

						// Set scroll wheel callback.
						glfwSetScrollCallback(m_window, [](GLFWwindow* window, double x, double y) {
							Window* this_win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							if (this_win->m_scene_dispatcher)
							{
								this_win->m_scene_dispatcher->trigger<events::MouseWheel>(x, y);
							}

							this_win->m_scroll_delta = y;
						});

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
						glDisable(GL_FRAMEBUFFER_SRGB);
						glDisable(GL_MULTISAMPLE);
						glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
						glEnable(GL_PROGRAM_POINT_SIZE);
						glEnable(GL_DEPTH_TEST);
						glDepthFunc(GL_LEQUAL);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

						// Create Post Processor.
						m_post_processor = std::make_unique<graphics::PostProcessor>();

						m_mouse_map.reserve(12);
						m_mouse_map.emplace(input::MouseButton::BUTTON_1, GLFW_MOUSE_BUTTON_1);
						m_mouse_map.emplace(input::MouseButton::BUTTON_2, GLFW_MOUSE_BUTTON_2);
						m_mouse_map.emplace(input::MouseButton::BUTTON_3, GLFW_MOUSE_BUTTON_3);
						m_mouse_map.emplace(input::MouseButton::BUTTON_4, GLFW_MOUSE_BUTTON_4);
						m_mouse_map.emplace(input::MouseButton::BUTTON_5, GLFW_MOUSE_BUTTON_5);
						m_mouse_map.emplace(input::MouseButton::BUTTON_6, GLFW_MOUSE_BUTTON_6);
						m_mouse_map.emplace(input::MouseButton::BUTTON_7, GLFW_MOUSE_BUTTON_7);
						m_mouse_map.emplace(input::MouseButton::BUTTON_8, GLFW_MOUSE_BUTTON_8);
						m_mouse_map.emplace(input::MouseButton::BUTTON_LAST, GLFW_MOUSE_BUTTON_LAST);
						m_mouse_map.emplace(input::MouseButton::BUTTON_LEFT, GLFW_MOUSE_BUTTON_LEFT);
						m_mouse_map.emplace(input::MouseButton::BUTTON_RIGHT, GLFW_MOUSE_BUTTON_RIGHT);
						m_mouse_map.emplace(input::MouseButton::BUTTON_MIDDLE, GLFW_MOUSE_BUTTON_MIDDLE);

						m_reverse_mouse_map.reserve(12);
						for (const auto& [key, value] : m_mouse_map)
						{
							m_reverse_mouse_map.emplace(value, key);
						}

						m_keymap.reserve(102);
						m_keymap.emplace(input::Keys::A, GLFW_KEY_A);
						m_keymap.emplace(input::Keys::B, GLFW_KEY_B);
						m_keymap.emplace(input::Keys::C, GLFW_KEY_C);
						m_keymap.emplace(input::Keys::D, GLFW_KEY_D);
						m_keymap.emplace(input::Keys::E, GLFW_KEY_E);
						m_keymap.emplace(input::Keys::F, GLFW_KEY_F);
						m_keymap.emplace(input::Keys::G, GLFW_KEY_G);
						m_keymap.emplace(input::Keys::H, GLFW_KEY_H);
						m_keymap.emplace(input::Keys::I, GLFW_KEY_I);
						m_keymap.emplace(input::Keys::J, GLFW_KEY_J);
						m_keymap.emplace(input::Keys::K, GLFW_KEY_K);
						m_keymap.emplace(input::Keys::L, GLFW_KEY_L);
						m_keymap.emplace(input::Keys::M, GLFW_KEY_M);
						m_keymap.emplace(input::Keys::N, GLFW_KEY_N);
						m_keymap.emplace(input::Keys::O, GLFW_KEY_O);
						m_keymap.emplace(input::Keys::P, GLFW_KEY_P);
						m_keymap.emplace(input::Keys::Q, GLFW_KEY_Q);
						m_keymap.emplace(input::Keys::R, GLFW_KEY_R);
						m_keymap.emplace(input::Keys::S, GLFW_KEY_S);
						m_keymap.emplace(input::Keys::T, GLFW_KEY_T);
						m_keymap.emplace(input::Keys::U, GLFW_KEY_U);
						m_keymap.emplace(input::Keys::V, GLFW_KEY_V);
						m_keymap.emplace(input::Keys::W, GLFW_KEY_W);
						m_keymap.emplace(input::Keys::X, GLFW_KEY_X);
						m_keymap.emplace(input::Keys::Y, GLFW_KEY_Y);
						m_keymap.emplace(input::Keys::Z, GLFW_KEY_Z);
						m_keymap.emplace(input::Keys::NUM_1, GLFW_KEY_1);
						m_keymap.emplace(input::Keys::NUM_2, GLFW_KEY_2);
						m_keymap.emplace(input::Keys::NUM_3, GLFW_KEY_3);
						m_keymap.emplace(input::Keys::NUM_4, GLFW_KEY_4);
						m_keymap.emplace(input::Keys::NUM_5, GLFW_KEY_5);
						m_keymap.emplace(input::Keys::NUM_6, GLFW_KEY_6);
						m_keymap.emplace(input::Keys::NUM_7, GLFW_KEY_7);
						m_keymap.emplace(input::Keys::NUM_8, GLFW_KEY_8);
						m_keymap.emplace(input::Keys::NUM_9, GLFW_KEY_9);
						m_keymap.emplace(input::Keys::NUM_0, GLFW_KEY_0);
						m_keymap.emplace(input::Keys::MINUS, GLFW_KEY_MINUS);
						m_keymap.emplace(input::Keys::EQUALS, GLFW_KEY_EQUAL);
						m_keymap.emplace(input::Keys::BACKSPACE, GLFW_KEY_BACKSPACE);
						m_keymap.emplace(input::Keys::GRAVE, GLFW_KEY_GRAVE_ACCENT);
						m_keymap.emplace(input::Keys::TAB, GLFW_KEY_TAB);
						m_keymap.emplace(input::Keys::CAPS, GLFW_KEY_CAPS_LOCK);
						m_keymap.emplace(input::Keys::LSHIFT, GLFW_KEY_LEFT_SHIFT);
						m_keymap.emplace(input::Keys::LCNTRL, GLFW_KEY_LEFT_CONTROL);
						m_keymap.emplace(input::Keys::LSTART, GLFW_KEY_LEFT_SUPER);
						m_keymap.emplace(input::Keys::LALT, GLFW_KEY_LEFT_ALT);
						m_keymap.emplace(input::Keys::SPACE, GLFW_KEY_SPACE);
						m_keymap.emplace(input::Keys::RALT, GLFW_KEY_RIGHT_ALT);
						m_keymap.emplace(input::Keys::RSTART, GLFW_KEY_RIGHT_SUPER);
						m_keymap.emplace(input::Keys::MENU, GLFW_KEY_MENU);
						m_keymap.emplace(input::Keys::RCNTRL, GLFW_KEY_RIGHT_CONTROL);
						m_keymap.emplace(input::Keys::RSHIFT, GLFW_KEY_RIGHT_SHIFT);
						m_keymap.emplace(input::Keys::ENTER, GLFW_KEY_ENTER);
						m_keymap.emplace(input::Keys::SEMICOLON, GLFW_KEY_SEMICOLON);
						m_keymap.emplace(input::Keys::APOSTROPHE, GLFW_KEY_APOSTROPHE);
						m_keymap.emplace(input::Keys::SLASH, GLFW_KEY_SLASH);
						m_keymap.emplace(input::Keys::PERIOD, GLFW_KEY_PERIOD);
						m_keymap.emplace(input::Keys::COMMA, GLFW_KEY_COMMA);
						m_keymap.emplace(input::Keys::LBRACKET, GLFW_KEY_LEFT_BRACKET);
						m_keymap.emplace(input::Keys::RBRACKET, GLFW_KEY_RIGHT_BRACKET);
						m_keymap.emplace(input::Keys::BACKSLASH, GLFW_KEY_BACKSLASH);
						m_keymap.emplace(input::Keys::ESC, GLFW_KEY_ESCAPE);
						m_keymap.emplace(input::Keys::F1, GLFW_KEY_F1);
						m_keymap.emplace(input::Keys::F2, GLFW_KEY_F2);
						m_keymap.emplace(input::Keys::F3, GLFW_KEY_F3);
						m_keymap.emplace(input::Keys::F4, GLFW_KEY_F4);
						m_keymap.emplace(input::Keys::F5, GLFW_KEY_F5);
						m_keymap.emplace(input::Keys::F6, GLFW_KEY_F6);
						m_keymap.emplace(input::Keys::F7, GLFW_KEY_F7);
						m_keymap.emplace(input::Keys::F8, GLFW_KEY_F8);
						m_keymap.emplace(input::Keys::F9, GLFW_KEY_F9);
						m_keymap.emplace(input::Keys::F10, GLFW_KEY_F10);
						m_keymap.emplace(input::Keys::F11, GLFW_KEY_F11);
						m_keymap.emplace(input::Keys::F12, GLFW_KEY_F12);
						m_keymap.emplace(input::Keys::PRINTSCREEN, GLFW_KEY_PRINT_SCREEN);
						m_keymap.emplace(input::Keys::SCROLL_LOCK, GLFW_KEY_SCROLL_LOCK);
						m_keymap.emplace(input::Keys::PAUSE, GLFW_KEY_PAUSE);
						m_keymap.emplace(input::Keys::INSERT, GLFW_KEY_INSERT);
						m_keymap.emplace(input::Keys::HOME, GLFW_KEY_HOME);
						m_keymap.emplace(input::Keys::PAGEUP, GLFW_KEY_PAGE_UP);
						m_keymap.emplace(input::Keys::PAGEDOWN, GLFW_KEY_PAGE_DOWN);
						m_keymap.emplace(input::Keys::END, GLFW_KEY_END);
						m_keymap.emplace(input::Keys::DEL, GLFW_KEY_DELETE);
						m_keymap.emplace(input::Keys::UP, GLFW_KEY_UP);
						m_keymap.emplace(input::Keys::DOWN, GLFW_KEY_DOWN);
						m_keymap.emplace(input::Keys::LEFT, GLFW_KEY_LEFT);
						m_keymap.emplace(input::Keys::RIGHT, GLFW_KEY_RIGHT);
						m_keymap.emplace(input::Keys::NUMLOCK, GLFW_KEY_NUM_LOCK);
						m_keymap.emplace(input::Keys::NUMPAD_DIVIDE, GLFW_KEY_KP_DIVIDE);
						m_keymap.emplace(input::Keys::NUMPAD_MULTIPLY, GLFW_KEY_KP_MULTIPLY);
						m_keymap.emplace(input::Keys::NUMPAD_ADD, GLFW_KEY_KP_ADD);
						m_keymap.emplace(input::Keys::NUMPAD_ENTER, GLFW_KEY_KP_ENTER);
						m_keymap.emplace(input::Keys::NUMPAD_PERIOD, GLFW_KEY_KP_DECIMAL);
						m_keymap.emplace(input::Keys::NUMPAD_0, GLFW_KEY_KP_0);
						m_keymap.emplace(input::Keys::NUMPAD_1, GLFW_KEY_KP_1);
						m_keymap.emplace(input::Keys::NUMPAD_2, GLFW_KEY_KP_2);
						m_keymap.emplace(input::Keys::NUMPAD_3, GLFW_KEY_KP_3);
						m_keymap.emplace(input::Keys::NUMPAD_4, GLFW_KEY_KP_4);
						m_keymap.emplace(input::Keys::NUMPAD_5, GLFW_KEY_KP_5);
						m_keymap.emplace(input::Keys::NUMPAD_6, GLFW_KEY_KP_6);
						m_keymap.emplace(input::Keys::NUMPAD_7, GLFW_KEY_KP_7);
						m_keymap.emplace(input::Keys::NUMPAD_8, GLFW_KEY_KP_8);
						m_keymap.emplace(input::Keys::NUMPAD_9, GLFW_KEY_KP_9);

						m_reverse_keymap.reserve(102);
						for (const auto& [key, value] : m_keymap)
						{
							m_reverse_keymap.emplace(value, key);
						}

						m_prev_key_states.reserve(102);
						m_prev_key_states.emplace(input::Keys::A, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::B, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::C, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::D, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::E, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::G, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::H, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::I, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::J, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::K, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::L, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::M, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::N, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::O, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::P, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::Q, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::R, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::S, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::T, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::U, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::V, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::W, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::X, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::Y, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::Z, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUM_1, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUM_2, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUM_3, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUM_4, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUM_5, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUM_6, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUM_7, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUM_8, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUM_9, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUM_0, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::MINUS, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::EQUALS, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::BACKSPACE, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::GRAVE, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::TAB, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::CAPS, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::LSHIFT, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::LCNTRL, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::LSTART, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::LALT, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::SPACE, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::RALT, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::RSTART, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::MENU, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::RCNTRL, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::RSHIFT, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::ENTER, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::SEMICOLON, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::APOSTROPHE, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::SLASH, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::PERIOD, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::COMMA, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::LBRACKET, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::RBRACKET, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::BACKSLASH, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::ESC, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F1, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F2, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F3, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F4, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F5, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F6, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F7, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F8, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F9, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F10, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F11, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::F12, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::PRINTSCREEN, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::SCROLL_LOCK, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::PAUSE, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::INSERT, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::HOME, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::PAGEUP, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::PAGEDOWN, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::END, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::DEL, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::UP, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::DOWN, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::LEFT, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::RIGHT, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMLOCK, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_MULTIPLY, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_DIVIDE, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_ADD, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_ENTER, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_PERIOD, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_0, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_1, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_2, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_3, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_4, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_5, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_6, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_7, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_8, GLFW_RELEASE);
						m_prev_key_states.emplace(input::Keys::NUMPAD_9, GLFW_RELEASE);

						m_prev_mouse_btn_states[0] = GLFW_RELEASE;
						m_prev_mouse_btn_states[1] = GLFW_RELEASE;
						m_prev_mouse_btn_states[2] = GLFW_RELEASE;
						m_prev_mouse_btn_states[3] = GLFW_RELEASE;
						m_prev_mouse_btn_states[4] = GLFW_RELEASE;
						m_prev_mouse_btn_states[5] = GLFW_RELEASE;
						m_prev_mouse_btn_states[6] = GLFW_RELEASE;
						m_prev_mouse_btn_states[7] = GLFW_RELEASE;
					}
				}
			}

			return result;
		}

		void Window::set_window_background(const graphics::Colour& col) noexcept
		{
			m_colour = const_cast<graphics::Colour&>(col).normalized();
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
			glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
			visible == true ? glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL)
					: glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		void Window::remove_cursor() noexcept
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
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
					m_cursor.m_glfw = glfwCreateCursor(&img, 0, 0);
					glfwSetCursor(m_window, m_cursor.m_glfw);
				}

				m_cursor_size = {img.width, img.height};

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
				m_cursor.m_glfw = glfwCreateCursor(&img, 0, 0);
				glfwSetCursor(m_window, m_cursor.m_glfw);
			}

			m_cursor_size = {img.width, img.height};

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
			if (m_cursor.m_glfw != nullptr)
			{
				glfwDestroyCursor(m_cursor.m_glfw);
			}

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

			glfwSetWindowSize(m_window, m_width, m_height);

			RENDERER_2D().resize(m_width, m_height);
			RENDERER_3D().resize(m_width, m_height);
			m_post_processor->resize(m_width, m_height);

			if (m_scene_dispatcher)
			{
				m_scene_dispatcher->trigger<events::WindowResized>(m_width, m_height);
			}
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

		void Window::enable_front_cull() noexcept
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
		}

		void Window::enable_back_cull() noexcept
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		void Window::enable_fandb_cull() noexcept
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT_AND_BACK);
		}

		void Window::disable_culling() noexcept
		{
			glDisable(GL_CULL_FACE);
		}

		void Window::begin()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			RENDERER_2D().prepare();
			RENDERER_3D().prepare();
		}

		void Window::end()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_width, m_height);

			m_post_processor->bind();

			glDisable(GL_BLEND);
			RENDERER_3D().render();

			glEnable(GL_BLEND);
			RENDERER_3D().do_forward_render();
			m_post_processor->render();
			RENDERER_2D().render();

			glfwSwapBuffers(m_window);
		}

		void Window::poll_events() noexcept
		{
			glfwPollEvents();
		}

		const bool Window::key_down(input::Keys key) noexcept
		{
			return glfwGetKey(m_window, m_keymap[key]) == GLFW_PRESS;
		}

		const bool Window::key_pressed(input::Keys key) noexcept
		{
			bool res  = false;
			int state = glfwGetKey(m_window, m_keymap[key]);
			if (m_prev_key_states[key] == GLFW_RELEASE && state == GLFW_PRESS)
			{
				res = true;
			}

			m_prev_key_states[key] = state;
			return res;
		}

		const bool Window::mouse_button_pressed(input::MouseButton mouse_button) noexcept
		{
			bool res = false;

			int state = glfwGetMouseButton(m_window, m_mouse_map[mouse_button]);
			if (state == GLFW_PRESS && m_prev_mouse_btn_states[m_mouse_map[mouse_button]] == GLFW_RELEASE)
			{
				res = true;
			}

			m_prev_mouse_btn_states[m_mouse_map[mouse_button]] = state;

			return res;
		}

		const bool Window::mouse_button_released(input::MouseButton mouse_button) noexcept
		{
			if (glfwGetMouseButton(m_window, m_mouse_map[mouse_button]) == GLFW_RELEASE)
			{
				m_prev_mouse_btn_states[m_mouse_map[mouse_button]] = GLFW_RELEASE;
				return true;
			}

			return false;
		}

		const double Window::get_scroll_delta() noexcept
		{
			double old_delta = m_scroll_delta;
			m_scroll_delta   = 0.0;

			return old_delta;
		}

		void Window::set_scene_dispatcher(events::Dispatcher* dispatcher) noexcept
		{
			m_scene_dispatcher = dispatcher;
		}

		std::string* Window::begin_text_input() noexcept
		{
			m_text_input     = "";
			m_inputting_text = true;

			return &m_text_input;
		}

		void Window::end_text_input() noexcept
		{
			m_inputting_text = false;
		}

		glm::vec2 Window::get_cursor_pos() noexcept
		{
			glfwGetCursorPos(m_window, &m_cursor.m_pos.x, &m_cursor.m_pos.y);
			return m_cursor.m_pos;
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
			return m_cursor_size;
		}

		GLFWwindow* Window::gl_window() noexcept
		{
			return m_window;
		}
	} // namespace core
} // namespace galaxy