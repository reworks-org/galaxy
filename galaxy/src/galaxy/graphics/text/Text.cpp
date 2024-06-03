///
/// Text.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/type_ptr.hpp>
#include <msdfgl.h>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/utils/StringUtils.hpp"

#include "Text.hpp"

namespace galaxy
{
	namespace graphics
	{
		Text::Text()
			: m_font {nullptr}
			, m_size {0.0f}
			, m_alignment {Alignment::LEFT}
		{
			auto indicies = graphics::gen_default_indices();
			m_vao.buffer(4, indicies);

			m_rt.create(1, 1);
		}

		Text::Text(Text&& t)
		{
			this->m_colour    = std::move(t.m_colour);
			this->m_vao       = std::move(t.m_vao);
			this->m_rt        = std::move(t.m_rt);
			this->m_font_name = t.m_font_name;
			this->m_font      = t.m_font;
			this->m_text      = std::move(t.m_text);
			this->m_size      = t.m_size;
			this->m_alignment = t.m_alignment;

			t.m_font = nullptr;
		}

		Text& Text::operator=(Text&& t)
		{
			if (this != &t)
			{
				this->m_vao       = std::move(t.m_vao);
				this->m_colour    = std::move(t.m_colour);
				this->m_rt        = std::move(t.m_rt);
				this->m_font_name = t.m_font_name;
				this->m_font      = t.m_font;
				this->m_text      = std::move(t.m_text);
				this->m_size      = t.m_size;
				this->m_alignment = t.m_alignment;

				t.m_font = nullptr;
			}

			return *this;
		}

		Text::~Text()
		{
			m_font = nullptr;
		}

		void Text::create(std::string_view text, const float size, const std::string& font, const graphics::Colour& colour, Alignment alignment)
		{
			m_colour    = colour;
			m_font_name = font;
			m_text      = text;
			m_size      = size;
			m_alignment = alignment;

			strutils::replace_all(m_text, "\t", "    "); // Handle tabs.

			update();
		}

		void Text::set_font(std::string_view font)
		{
			m_font_name = font;
		}

		void Text::update()
		{
			if (!m_font_name.empty())
			{
				auto& fonts = core::ServiceLocator<resource::Fonts>::ref();
				m_font      = fonts.get(m_font_name);

				const auto vec    = m_font->get_text_size(m_text, m_size);
				const auto width  = vec.x;
				const auto height = vec.y;

				if (width > m_rt.width() || height > m_rt.height())
				{
					m_rt.recreate(static_cast<int>(width), static_cast<int>(height));
				}

				m_rt.bind();

				std::size_t start = 0;
				std::size_t end   = m_text.find('\n');

				auto proj  = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
				auto y_off = m_font->vertical_advance(m_size);

				while (end != std::string::npos)
				{
					const auto block = m_text.substr(start, end);
					msdfgl_printf(0,
						y_off,
						static_cast<int>(m_alignment),
						m_font->handle(),
						m_size,
						0xffffffff,
						glm::value_ptr(proj),
						static_cast<msdfgl_printf_flags>(MSDFGL_UTF8 | MSDFGL_KERNING),
						block.c_str());

					y_off += m_font->vertical_advance(m_size);
					start  = end + 1;
					end    = m_text.find('\n', start);
				}

				const auto last_block = m_text.substr(start, end);

				msdfgl_printf(0,
					y_off,
					static_cast<int>(m_alignment),
					m_font->handle(),
					m_size,
					0xffffffff,
					glm::value_ptr(proj),
					static_cast<msdfgl_printf_flags>(MSDFGL_UTF8 | MSDFGL_KERNING),
					last_block.c_str());

				m_rt.unbind();

				auto vertices = graphics::gen_quad_vertices(width, height);
				m_vao.sub_buffer(0, vertices);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to create text without a font.");
			}
		}

		void Text::update(std::string_view text)
		{
			m_text = text;
			strutils::replace_all(m_text, "\t", "    "); // Handle tabs.

			update();
		}

		void Text::update(const float size)
		{
			m_size = size;

			update();
		}

		void Text::update(Alignment alignment)
		{
			m_alignment = alignment;

			update();
		}

		float Text::width() const
		{
			return static_cast<float>(m_rt.width());
		}

		float Text::height() const
		{
			return static_cast<float>(m_rt.height());
		}

		const std::string& Text::get_text() const
		{
			return m_text;
		}

		float Text::get_size() const
		{
			return m_size;
		}

		Text::Alignment Text::get_alignment() const
		{
			return m_alignment;
		}

		const std::string& Text::get_font() const
		{
			return m_font_name;
		}

		graphics::VertexArray& Text::vao()
		{
			return m_vao;
		}

		graphics::RenderTexture& Text::render_texture()
		{
			return m_rt;
		}
	} // namespace graphics
} // namespace galaxy
