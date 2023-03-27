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
			, m_data {nullptr}
		{
		}

		Cursor::~Cursor()
		{
			destroy();
		}

		void Cursor::toggle(const bool visible)
		{
			if (visible)
			{
				glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

				// Raw mouse input.
				if (glfwRawMouseMotionSupported())
				{
					glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
				}
			}
		}

		void Cursor::set_cursor_icon(std::string_view icon)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto info = fs.find(icon);
			if (info.code == fs::FileCode::FOUND)
			{
				stbi_set_flip_vertically_on_load(true);

				// Fill glfw-compatible struct.

				GLFWimage img = {};
				img.pixels    = stbi_load(info.string.c_str(), &img.width, &img.height, nullptr, STBI_rgb_alpha);

				if (img.pixels)
				{
					// Copies data so safe to destroy.
					m_data = glfwCreateCursor(&img, 0, 0);
					glfwSetCursor(m_window, m_data);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load image '{0}' with stb returning '{1}'.", icon, stbi_failure_reason());
				}

				stbi_image_free(img.pixels);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find '{0}' to use as a cursor, because '{1}'.", icon, magic_enum::enum_name(info.code));
			}
		}

		void Cursor::set_cursor_icon(std::span<unsigned char> buffer)
		{
			stbi_set_flip_vertically_on_load(true);

			// Fill glfw-compatible struct.

			GLFWimage img = {};
			img.pixels    = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &img.width, &img.height, nullptr, STBI_rgb_alpha);

			if (img.pixels)
			{
				// Copies data so safe to destroy.
				m_data = glfwCreateCursor(&img, 0, 0);
				glfwSetCursor(m_window, m_data);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load buffer with stb returning '{0}'.", stbi_failure_reason());
			}

			stbi_image_free(img.pixels);
		}

		bool Cursor::within_window() const
		{
			return glfwGetWindowAttrib(m_window, GLFW_HOVERED);
		}

		void Cursor::destroy()
		{
			if (m_data != nullptr)
			{
				glfwDestroyCursor(m_data);
				m_data = nullptr;
			}
		}
	} // namespace input
} // namespace galaxy