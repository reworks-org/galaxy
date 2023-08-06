///
/// Window.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <stb_image.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/graphics/FontContext.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/input/InputMods.hpp"
#include "galaxy/platform/Pragma.hpp"
#include "galaxy/utils/StringUtils.hpp"
#include "galaxy/ui/NuklearUI.hpp"

#include "Window.hpp"

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
		Window::Window()
			: m_window {nullptr}
			, m_width {1}
			, m_height {1}
		{
			m_glfw_allocator.allocate   = &glfw_alloc;
			m_glfw_allocator.reallocate = &glfw_realloc;
			m_glfw_allocator.deallocate = &glfw_dealloc;
		}

		Window::Window(const WindowSettings& settings)
			: m_window {nullptr}
			, m_width {1}
			, m_height {1}
		{
			m_glfw_allocator.allocate   = &glfw_alloc;
			m_glfw_allocator.reallocate = &glfw_realloc;
			m_glfw_allocator.deallocate = &glfw_dealloc;

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
				glfwWindowHint(GLFW_SCALE_TO_MONITOR, settings.scale_to_monitor);

				// Configure framebuffer setup.
				glfwWindowHint(GLFW_RED_BITS, 8);
				glfwWindowHint(GLFW_GREEN_BITS, 8);
				glfwWindowHint(GLFW_BLUE_BITS, 8);
				glfwWindowHint(GLFW_ALPHA_BITS, 8);
				glfwWindowHint(GLFW_DEPTH_BITS, 24);
				glfwWindowHint(GLFW_STENCIL_BITS, 8);
				glfwWindowHint(GLFW_STEREO, GLFW_FALSE);
				glfwWindowHint(GLFW_SAMPLES, 4);
				glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
				glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

				// Configure OpenGL context hints.
				glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings.debug);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

				// Window maximized check.
				if (settings.maximized)
				{
					glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
				}

				// Create the window from input, ensuring it is centered in the screen.
				m_window = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);
				if (!m_window)
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create window.");
				}
				else
				{
					// Center window.
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

					glfwSetInputMode(m_window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
					glfwShowWindow(m_window);

					// Set input devices.
					m_keyboard.set_window(m_window);
					m_mouse.set_window(m_window);
					m_cursor.set_window(m_window);
					m_clipboard.set_window(m_window);
					input::Input::s_window = m_window;

					// Set resize callback.
					glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

						// clang-format off
						events::WindowResized wr
						{
							.width = width,
							.height = height
						};
						// clang-format on

						win->m_event_queue.emplace_back(std::move(wr));
						win->resize(width, height);
					});

					// Content scale callback.
					glfwSetWindowContentScaleCallback(m_window, [](GLFWwindow* window, float xscale, float yscale) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

						// clang-format off
						events::ContentScale sc
						{
							.xscale = xscale,
							.yscale = yscale
						};
						// clang-format on

						win->m_event_queue.emplace_back(std::move(sc));
					});

					// Key input callback.
					glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
						if (!win->m_keyboard.is_text_input_enabled())
						{
							// clang-format off
							switch (action)
							{
								case GLFW_PRESS:
								{
									events::KeyDown kd
									{
										.keycode = input::int_to_key(key),
										.mod = input::int_to_keymod(mods),
										.scancode = scancode
									};
								
									win->m_event_queue.emplace_back<events::KeyDown>(std::move(kd));
								}
								break;

								case GLFW_REPEAT:
								{
									events::KeyRepeat kr
									{
										.keycode = input::int_to_key(key),
										.mod = input::int_to_keymod(mods),
										.scancode = scancode
									};
								
									win->m_event_queue.emplace_back<events::KeyRepeat>(std::move(kr));
								}
								break;

								case GLFW_RELEASE:
								{
									events::KeyUp ku
									{
										.keycode = input::int_to_key(key),
										.mod = input::int_to_keymod(mods),
										.scancode = scancode
									};
								
									win->m_event_queue.emplace_back<events::KeyUp>(std::move(ku));
								}
								break;

								default:
                                    GALAXY_LOG(GALAXY_FATAL, "Received invalid action in glfw key callback.");
								break;
							}
							// clang-format on
						}
					});

					// Text input callback.
					glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
						if (win->m_keyboard.is_text_input_enabled())
						{
							// clang-format off
							events::KeyChar kc
							{
								.codepoint = codepoint
							};
							// clang-format on

							ServiceLocator<ui::NuklearUI>::ref().on_key_char(window, kc);
							win->m_event_queue.emplace_back<events::KeyChar>(std::move(kc));
						}
					});

					// Mouse entered callback.
					glfwSetCursorEnterCallback(m_window, [](GLFWwindow* window, int entered) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

						// clang-format off
						events::MouseEnter me
						{
							.entered = static_cast<bool>(entered)
						};
						// clang-format on

						win->m_event_queue.emplace_back<events::MouseEnter>(std::move(me));
					});

					// Mouse movement callback.
					glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

						// clang-format off
						events::MouseMoved mm
						{
							.xpos = xpos,
							.ypos = ypos
						};
						// clang-format on

						win->m_event_queue.emplace_back<events::MouseMoved>(std::move(mm));
					});

					// Mouse button callback.
					glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
						auto* win      = static_cast<Window*>(glfwGetWindowUserPointer(window));
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
									.button = static_cast<input::MouseButtons>(button),
									.mod = input::int_to_keymod(mods)
								};
								
								ServiceLocator<ui::NuklearUI>::ref().on_mouse_pressed(window, mp);
								win->m_event_queue.emplace_back<events::MousePressed>(std::move(mp));
							}
							break;

							case GLFW_RELEASE:
							{
								events::MouseReleased mr
								{
									.xpos = pos.x,
									.ypos = pos.y,
									.button = static_cast<input::MouseButtons>(button),
									.mod = input::int_to_keymod(mods)
								};
								
								win->m_event_queue.emplace_back<events::MouseReleased>(std::move(mr));
							}
							break;

							default:
                                GALAXY_LOG(GALAXY_FATAL, "Received invalid action in glfw key callback.");
							break;
						}
						// clang-format on
					});

					// Set scroll wheel callback.
					glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
						auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

						// clang-format off
						events::MouseWheel mw
						{
							.xoff = xoffset,
							.yoff = yoffset
						};
						// clang-format on

						ServiceLocator<ui::NuklearUI>::ref().on_mouse_wheel(window, mw);
						win->m_event_queue.emplace_back<events::MouseWheel>(std::move(mw));
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

					// Set window scale callback.
					glfwSetWindowContentScaleCallback(m_window, [](GLFWwindow* window, float xscale, float yscale) {
						if (!ServiceLocator<graphics::FontContext>::empty())
						{
							auto& fc = ServiceLocator<graphics::FontContext>::ref();
							fc.set_dpi(xscale * 96.0f, yscale * 96.0f);
						}
					});

					// clang-format off
					#ifdef GALAXY_WIN_PLATFORM
					GALAXY_DISABLE_WARNING_POP
					#endif
					// clang-format on

					glfwSetWindowUserPointer(m_window, this);
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
						glDisable(GL_STENCIL_TEST);
						glDisable(GL_SCISSOR_TEST);

						glEnable(GL_MULTISAMPLE);
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
						m_postprocess.init(m_width, m_height);

						m_postprocess.add<graphics::ChromaticAberration>(m_width, m_height);
						m_postprocess.add<graphics::GammaCorrection>(m_width, m_height);
						m_postprocess.add<graphics::GaussianBlur>(m_width, m_height);
						m_postprocess.add<graphics::Sharpen>(m_width, m_height);
						m_postprocess.add<graphics::SMAA>(m_width, m_height);

						graphics::Renderer::init();
					}
				}
			}
		}

		void Window::set_title(const char* title)
		{
			glfwSetWindowTitle(m_window, title);
		}

		void Window::set_icon(std::string_view icon)
		{
			auto& fs = ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto info = fs.find(icon);
			if (info.code == fs::FileCode::FOUND)
			{
				// Fill glfw-compatible struct.

				stbi_set_flip_vertically_on_load(true);

				GLFWimage img = {};
				img.pixels    = stbi_load(info.string.c_str(), &img.width, &img.height, nullptr, STBI_rgb_alpha);

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
				GALAXY_LOG(GALAXY_ERROR, "Failed to find '{0}' to use as a window icon, because '{1}'.", icon, magic_enum::enum_name(info.code));
			}
		}

		void Window::set_icon(std::span<unsigned char> buffer)
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
				graphics::Renderer::destroy();
				m_postprocess.destroy();
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
			pop_queued_events();
			glfwPollEvents();
		}

		void Window::begin_postprocessing()
		{
			m_postprocess.bind();
		}

		void Window::end_postprocessing()
		{
			m_postprocess.render_effects();
		}

		void Window::prepare_screen_for_rendering()
		{
			// Final Output.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_width, m_height);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Window::render_postprocessing()
		{
			m_postprocess.render_output();

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}

		void Window::resize(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			m_postprocess.resize(width, height);
			glfwSetWindowSize(m_window, m_width, m_height);
		}

		void Window::trigger_queued_events(entt::dispatcher& dispatcher)
		{
			for (auto i = 0; i < m_event_queue.size(); i++)
			{
				// clang-format off
				std::visit([&](auto&& queued_event) {
                    dispatcher.trigger(queued_event);
                }, m_event_queue[i]);
				// clang-format on
			}
		}

		void Window::pop_queued_events()
		{
			m_event_queue.clear();
		}

		void Window::request_attention()
		{
			glfwRequestWindowAttention(m_window);
		}

		void Window::focus() const
		{
			glfwFocusWindow(m_window);
		}

		void Window::maximize() const
		{
			glfwMaximizeWindow(m_window);
		}

		void Window::restore() const
		{
			glfwRestoreWindow(m_window);
		}

		void Window::minimize() const
		{
			glfwIconifyWindow(m_window);
		}

		void Window::allow_native_closing()
		{
			glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			});
		}

		void Window::prevent_native_closing()
		{
			glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
				glfwSetWindowShouldClose(window, GLFW_FALSE);
			});
		}

		bool Window::is_focused() const
		{
			return glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
		}

		events::Queue& Window::queued_events()
		{
			return m_event_queue;
		}

		const meta::vector<std::string>& Window::get_drop_paths() const
		{
			return m_drop_paths;
		}

		int Window::get_width() const
		{
			return m_width;
		}

		int Window::get_height() const
		{
			return m_height;
		}

		float Window::get_widthf() const
		{
			return static_cast<float>(m_width);
		}

		float Window::get_heightf() const
		{
			return static_cast<float>(m_height);
		}

		glm::ivec2 Window::get_framebuffer_size()
		{
			glm::ivec2 size;
			glfwGetFramebufferSize(m_window, &size.x, &size.y);

			return size;
		}

		GLFWwindow* Window::handle()
		{
			return m_window;
		}
	} // namespace core
} // namespace galaxy