///
/// Cursor.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Cursor.hpp"

namespace galaxy
{
	namespace input
	{
		Cursor::Cursor()
			: InputDevice {}
			, m_custom {nullptr}
			, m_pointer {nullptr}
			, m_cross {nullptr}
			, m_hand {nullptr}
			, m_text {nullptr}
		{
		}

		Cursor::~Cursor()
		{
			destroy();
		}

		void Cursor::init()
		{
			m_pointer = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
			m_cross   = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
			m_hand    = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
			m_text    = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		}

		void Cursor::toggle(const bool visible)
		{
			if (visible)
			{
				if (glfwRawMouseMotionSupported())
				{
					glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
				}

				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

				if (glfwRawMouseMotionSupported())
				{
					glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
				}
			}
		}

		void Cursor::load_custom(std::string_view file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto info = fs.find(file);
			if (info.code == fs::FileCode::FOUND)
			{
				stbi_set_flip_vertically_on_load(true);

				// Fill glfw-compatible struct.

				GLFWimage img = {};
				img.pixels    = stbi_load(info.string.c_str(), &img.width, &img.height, nullptr, STBI_rgb_alpha);

				if (img.pixels)
				{
					// Copies data so safe to destroy.
					m_custom = glfwCreateCursor(&img, 0, 0);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load image '{0}' with stb returning '{1}'.", file, stbi_failure_reason());
				}

				stbi_image_free(img.pixels);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find '{0}' to use as a cursor, because '{1}'.", file, magic_enum::enum_name(info.code));
			}
		}

		void Cursor::load_custom_mem(std::span<unsigned char> buffer)
		{
			stbi_set_flip_vertically_on_load(true);

			// Fill glfw-compatible struct.

			GLFWimage img = {};
			img.pixels    = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &img.width, &img.height, nullptr, STBI_rgb_alpha);

			if (img.pixels)
			{
				// Copies data so safe to destroy.
				m_custom = glfwCreateCursor(&img, 0, 0);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load buffer with stb returning '{0}'.", stbi_failure_reason());
			}

			stbi_image_free(img.pixels);
		}

		void Cursor::use_custom()
		{
			if (m_custom)
			{
				glfwSetCursor(m_window, m_custom);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to use custom cursor without loading data.");
			}
		}

		void Cursor::use_pointer()
		{
			glfwSetCursor(m_window, m_pointer);
		}

		void Cursor::use_hand()
		{
			glfwSetCursor(m_window, m_hand);
		}

		void Cursor::use_cross()
		{
			glfwSetCursor(m_window, m_cross);
		}

		void Cursor::use_text()
		{
			glfwSetCursor(m_window, m_text);
		}

		void Cursor::use_custom_else_pointer()
		{
			if (m_custom)
			{
				glfwSetCursor(m_window, m_custom);
			}
			else
			{
				glfwSetCursor(m_window, m_pointer);
			}
		}

		bool Cursor::within_window() const
		{
			return glfwGetWindowAttrib(m_window, GLFW_HOVERED);
		}

		void Cursor::destroy()
		{
			if (m_custom != nullptr)
			{
				glfwDestroyCursor(m_custom);
				m_custom = nullptr;
			}
		}

		void Cursor::destroy_system_cursors()
		{
			glfwDestroyCursor(m_pointer);
			glfwDestroyCursor(m_cross);
			glfwDestroyCursor(m_hand);
			glfwDestroyCursor(m_text);
		}
	} // namespace input
} // namespace galaxy