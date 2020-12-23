///
/// Text.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "Text.hpp"

///
/// Core namespace.
///
namespace qs
{
	Text::Text()
	    : m_width {0}, m_height {0}, m_colour {255, 255, 255, 255}, m_font {nullptr}, m_batch(1000)
	{
	}

	Text::~Text()
	{
		m_batch.clear();
		m_batch_data.clear();
	}

	void Text::load(qs::Font* font, const pr::Colour& col)
	{
		m_font   = font;
		m_colour = col;
	}

	void Text::create(std::string_view text)
	{
		m_batch.set_texture(m_font->get_fontmap());
		if (!text.empty())
		{
			float x_offset       = 0.0f;
			float y_offset       = 0.0f;
			unsigned int counter = 0;
			for (const char c : text)
			{
				if (c == '\n')
				{
					x_offset = 0.0f;
					y_offset += m_font->get_height();
				}
				else
				{
					auto* c_obj = m_font->get_char(c);

					if (c_obj != nullptr)
					{
						m_batch_data.emplace(counter, qs::BatchedSprite {});

						auto* spr = &m_batch_data[counter];
						spr->create(c_obj->m_region, 1);
						spr->set_pos(x_offset + c_obj->m_bearing.x, y_offset);
						x_offset += (c_obj->m_advance >> 6);

						m_batch.add(spr);
					}
				}

				counter++;
			}

			m_width  = m_font->get_width(text);
			m_height = m_font->get_height();
		}
		else
		{
			m_width  = 1;
			m_height = 1;
		}

		set_rotation_origin(m_width * 0.5f, m_height * 0.5f);
		m_dirty = true;
	}

	void Text::update_text(std::string_view text)
	{
		m_batch.clear();
		m_batch_data.clear();
		create(text);
	}

	void Text::bind()
	{
		m_batch.update(this);
		m_batch.bind();
	}

	void Text::unbind()
	{
		m_batch.unbind();
	}

	std::array<float, 4> Text::get_colour()
	{
		return m_colour.get_normalized();
	}

	const int Text::get_width() const
	{
		return m_width;
	}

	const int Text::get_height() const
	{
		return m_height;
	}

	const int Text::get_batch_width() const
	{
		return m_batch.get_width();
	}

	const int Text::get_batch_height() const
	{
		return m_batch.get_height();
	}

	const unsigned int Text::index_count() const
	{
		return m_batch.get_used_index_count();
	}
} // namespace qs