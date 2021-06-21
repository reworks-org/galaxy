///
/// Text.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/text/Font.hpp"
#include "galaxy/resource/FontBook.hpp"

#include "Text.hpp"

namespace galaxy
{
	namespace components
	{
		Text::Text() noexcept
		    : Serializable {this}, m_depth {0}, m_width {0}, m_height {0}, m_fontmap_width {0}, m_fontmap_height {0}, m_colour {255, 255, 255, 255}
		{
		}

		Text::Text(const nlohmann::json& json)
		    : Serializable {this}, m_depth {0}, m_width {0}, m_height {0}, m_fontmap_width {0}, m_fontmap_height {0}, m_colour {255, 255, 255, 255}
		{
			deserialize(json);
		}

		Text::Text(Text&& t) noexcept
		    : Serializable {this}
		{
			this->m_batch          = std::move(t.m_batch);
			this->m_batch_data     = std::move(t.m_batch_data);
			this->m_colour         = std::move(t.m_colour);
			this->m_height         = t.m_height;
			this->m_width          = t.m_width;
			this->m_fontmap_height = t.m_fontmap_height;
			this->m_fontmap_width  = t.m_fontmap_width;
			this->m_depth          = t.m_depth;
		}

		Text& Text::operator=(Text&& t) noexcept
		{
			if (this != &t)
			{
				this->m_batch          = std::move(t.m_batch);
				this->m_batch_data     = std::move(t.m_batch_data);
				this->m_colour         = std::move(t.m_colour);
				this->m_height         = t.m_height;
				this->m_width          = t.m_width;
				this->m_fontmap_height = t.m_fontmap_height;
				this->m_fontmap_width  = t.m_fontmap_width;
				this->m_depth          = t.m_depth;
			}

			return *this;
		}

		Text::~Text() noexcept
		{
			m_batch.clear();
			m_batch_data.clear();
		}

		void Text::load(std::string_view font, const graphics::Colour& col)
		{
			m_batch.clear();
			m_batch_data.clear();

			set_font(font);
			m_colour = col;
		}

		void Text::create(std::string_view text, const int depth)
		{
			m_depth = depth;

			m_batch.clear();
			m_batch_data.clear();

			auto* font_ptr   = SL_HANDLE.fontbook()->get(m_font_str);
			m_fontmap_width  = font_ptr->get_rendertexture()->get_width();
			m_fontmap_height = font_ptr->get_rendertexture()->get_height();
			m_batch.add_texture(font_ptr->get_fontmap());

			m_text_str = text;
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
						y_offset += font_ptr->get_height();
					}
					else
					{
						auto* c_obj = font_ptr->get_char(c);

						if (c_obj != nullptr)
						{
							m_batch_data.emplace(counter, CharacterBatch {});

							auto* data = &m_batch_data[counter];
							data->m_sprite.create(c_obj->m_region, depth);
							data->m_transform.set_pos(x_offset + c_obj->m_bearing.x, y_offset);
							x_offset += (c_obj->m_advance >> 6);

							m_batch.add(&data->m_sprite, &data->m_transform);
						}
					}

					counter++;
				}

				m_width  = font_ptr->get_width(text);
				m_height = font_ptr->get_height();

				m_batch.buffer_data();
			}
			else
			{
				m_width  = 1;
				m_height = 1;
			}
		}

		void Text::update(std::string_view text)
		{
			create(text, m_depth);
		}

		void Text::bind() noexcept
		{
			m_batch.bind();
		}

		void Text::unbind() noexcept
		{
			m_batch.unbind();
		}

		void Text::change_colour(const graphics::Colour& col)
		{
			m_colour = col;
		}

		void Text::set_font(std::string_view font)
		{
			m_font_str = font;
		}

		graphics::Colour& Text::get_colour() noexcept
		{
			return m_colour;
		}

		const int Text::get_width() const noexcept
		{
			return m_width;
		}

		const int Text::get_height() const noexcept
		{
			return m_height;
		}

		const int Text::get_batch_width() const noexcept
		{
			return m_fontmap_width;
		}

		const int Text::get_batch_height() const noexcept
		{
			return m_fontmap_height;
		}

		const int Text::get_depth() const noexcept
		{
			return m_depth;
		}

		const unsigned int Text::count() const noexcept
		{
			return m_batch.count();
		}

		const std::string& Text::get_text() const noexcept
		{
			return m_text_str;
		}

		const std::string& Text::get_font_id() const noexcept
		{
			return m_font_str;
		}

		nlohmann::json Text::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["font"]        = m_font_str;
			json["text"]        = m_text_str;
			json["depth"]       = m_depth;

			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			return json;
		}

		void Text::deserialize(const nlohmann::json& json)
		{
			const auto colour = json.at("colour");
			load(json.at("font"), {colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")});
			create(json.at("text"), json.at("depth"));
		}
	} // namespace components
} // namespace galaxy