///
/// Font.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/utils/Globals.hpp"

#include "Font.hpp"

namespace galaxy
{
	namespace graphics
	{
		Font::Font()
			: m_font {nullptr}
			, m_face {nullptr}
		{
		}

		Font::Font(Font&& f)
		{
			if (this->m_font != nullptr)
			{
				msdfgl_destroy_font(this->m_font);
			}

			this->m_font = f.m_font;
			this->m_face = f.m_face;

			f.m_font = nullptr;
			f.m_face = nullptr;
		}

		Font& Font::operator=(Font&& f)
		{
			if (this != &f)
			{
				if (this->m_font != nullptr)
				{
					msdfgl_destroy_font(this->m_font);
				}

				this->m_font = f.m_font;
				this->m_face = f.m_face;

				f.m_font = nullptr;
				f.m_face = nullptr;
			}

			return *this;
		}

		Font::~Font()
		{
			if (m_font != nullptr)
			{
				msdfgl_destroy_font(m_font);

				m_face = nullptr;
			}
		}

		bool Font::load(const std::string& file)
		{
			auto  result = false;
			auto& fs     = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto data = fs.read_binary(file);
			if (!data.empty())
			{
				auto& fc = core::ServiceLocator<FontContext>::ref();
				m_face   = msdfgl_load_font_mem(fc.context(), data.data(), data.size());

				result = true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read font '{0}'.", file);
			}

			return result;
		}

		bool Font::load(unsigned char* buffer, const unsigned int size)
		{
			auto result = false;

			if (buffer && size > 0)
			{
				auto& fc = core::ServiceLocator<FontContext>::ref();
				m_face   = msdfgl_load_font_mem(fc.context(), buffer, size);

				result = true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load font from memory due to empty or null buffer");
			}

			return result;
		}

		void Font::build()
		{
			auto& fc = core::ServiceLocator<FontContext>::ref();

			m_font = _msdfgl_init_font_internal(fc.context(), &m_face, GALAXY_FONT_MSDF_RANGE, GALAXY_FONT_MSDF_SCALE, nullptr);
			if (!m_font)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to build font atlas for {0}.", m_face->family_name);
			}
			else
			{
				msdfgl_generate_ascii_ext(m_font);
			}

			// NOW OWNED BY MSDFGL
			m_face = nullptr;
		}

		float Font::vertical_advance(const float size) const
		{
			return msdfgl_vertical_advance(m_font, size);
		}

		glm::vec2 Font::get_text_size(const std::string& text, const float size)
		{
			glm::vec2 vec;

			std::size_t start = 0;
			std::size_t end   = text.find('\n');

			while (end != std::string::npos)
			{
				vec.y += msdfgl_vertical_advance(m_font, size);

				const auto block = text.substr(start, end);

				float x = 0.0f, y = 0.0f;
				msdfgl_geometry(&x, &y, m_font, size, static_cast<msdfgl_printf_flags>(MSDFGL_UTF8 | MSDFGL_KERNING), block.c_str());

				if (x > vec.x)
				{
					vec.x = x;
				}

				start = end + 1;
				end   = text.find('\n', start);
			}

			const auto last_block = text.substr(start, end);

			float x = 0.0f, y = 0.0f;
			msdfgl_geometry(&x, &y, m_font, size, static_cast<msdfgl_printf_flags>(MSDFGL_UTF8 | MSDFGL_KERNING), last_block.c_str());

			if (x > vec.x)
			{
				vec.x = x;
			}

			vec.y += msdfgl_vertical_advance(m_font, size);

			return vec;
		}

		msdfgl_font_t Font::handle()
		{
			return m_font;
		}
	} // namespace graphics
} // namespace galaxy
