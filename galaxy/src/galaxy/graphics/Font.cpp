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
		Font::Font() noexcept
			: m_font {nullptr}
			, m_face {nullptr}
		{
		}

		Font::Font(const std::string& file)
			: m_font {nullptr}
			, m_face {nullptr}
		{
			if (!load(file))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load font file '{0}'.", file);
			}
		}

		Font::Font(Font&& f) noexcept
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

		Font& Font::operator=(Font&& f) noexcept
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

		Font::~Font() noexcept
		{
			if (m_font != nullptr)
			{
				msdfgl_destroy_font(m_font);

				m_face = nullptr;
			}
		}

		bool Font::load(const std::string& file)
		{
			auto result = false;
			auto& fs    = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto info = fs.find(file);
			if (info.code == fs::FileCode::FOUND)
			{
				auto& fc = core::ServiceLocator<FontContext>::ref();
				m_face   = msdfgl_load_font(fc.context(), info.string.c_str());

				result = true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find font '{0}' because '{1}'.", file, magic_enum::enum_name(info.code));
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

			/* NOW OWNED BY MSDFGL */
			m_face = nullptr;
		}

		float Font::vertical_advance(const float size) const noexcept
		{
			return msdfgl_vertical_advance(m_font, size);
		}

		glm::vec2 Font::get_text_size(const std::string& text, const float size) noexcept
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

		msdfgl_font_t Font::handle() noexcept
		{
			return m_font;
		}
	} // namespace graphics
} // namespace galaxy