///
/// RenderTexture.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image_write.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "RenderTexture.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 26493)
#endif

namespace galaxy
{
	namespace graphics
	{
		RenderTexture::RenderTexture()
			: m_width {0}
			, m_height {0}
		{
		}

		RenderTexture::RenderTexture(RenderTexture&& rt)
		{
			this->m_width       = rt.m_width;
			this->m_height      = rt.m_height;
			this->m_framebuffer = std::move(rt.m_framebuffer);
		}

		RenderTexture& RenderTexture::operator=(RenderTexture&& rt)
		{
			if (this != &rt)
			{
				this->m_width       = rt.m_width;
				this->m_height      = rt.m_height;
				this->m_framebuffer = std::move(rt.m_framebuffer);
			}

			return *this;
		}

		RenderTexture::~RenderTexture()
		{
		}

		void RenderTexture::create(const int width, const int height)
		{
			m_width  = std::max(1, width);
			m_height = std::max(1, height);

			// m_projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

			m_framebuffer.add_colour_attachment(width, height);
			m_framebuffer.add_depth_stencil_renderbuffer();
			m_framebuffer.create();
			m_proj = glm::ortho(0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f, -1.0f, 1.0f);
		}

		void RenderTexture::recreate(const int width, const int height)
		{
			m_framebuffer.reset();

			if (width > 0 || height > 0)
			{
				m_width  = width;
				m_height = height;
			}

			create(m_width, m_height);
		}

		void RenderTexture::save(std::string_view file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto path = std::filesystem::path(file);
			if (!path.is_absolute())
			{
				path = GALAXY_ROOT_DIR / path;
			}

			if (!path.has_extension())
			{
				path.replace_extension(".png");
			}

			if (!std::filesystem::exists(path.parent_path()))
			{
				std::filesystem::create_directories(path.parent_path());
			}

			meta::vector<unsigned int> pixels(m_width * m_height * 4, 0);

			glGetTextureImage(m_framebuffer.texture(), 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLsizei>(pixels.size()), pixels.data());

			stbi_flip_vertically_on_write(true);

			int            len = 0;
			unsigned char* png = stbi_write_png_to_mem((const unsigned char*)pixels.data(), m_width * 4, m_width, m_height, 4, &len);

			if (!fs.write_raw(png, len, path.string()))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to write '{0}' to disk.", path.string());
			}

			mi_free(png);
		}

		void RenderTexture::bind(bool clear)
		{
			m_framebuffer.begin(clear);
		}

		void RenderTexture::unbind()
		{
			m_framebuffer.end();
		}

		void RenderTexture::clear()
		{
			m_framebuffer.clear();
		}

		int RenderTexture::width() const
		{
			return m_width;
		}

		int RenderTexture::height() const
		{
			return m_height;
		}

		unsigned int RenderTexture::texture() const
		{
			return m_framebuffer.texture();
		}

		const glm::mat4& RenderTexture::get_proj()
		{
			return m_proj;
		}

		Framebuffer& RenderTexture::fbo()
		{
			return m_framebuffer;
		}
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
