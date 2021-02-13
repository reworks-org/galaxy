///
/// Text.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/res/FontBook.hpp"

#include "Text.hpp"

namespace galaxy
{
	namespace components
	{
		Text::Text() noexcept
		    : Serializable {this}, m_width {0}, m_height {0}, m_colour {255, 255, 255, 255}, m_font {nullptr}, m_batch {100}
		{
		}

		Text::Text(const nlohmann::json& json)
		    : Serializable {this}, m_width {0}, m_height {0}, m_colour {255, 255, 255, 255}, m_font {nullptr}, m_batch {100}
		{
			deserialize(json);
		}

		Text::Text(Text&& t) noexcept
		    : Serializable {this}, m_batch {100}
		{
			this->m_batch      = std::move(t.m_batch);
			this->m_batch_data = std::move(t.m_batch_data);
			this->m_colour     = std::move(t.m_colour);
			this->m_font       = t.m_font;
			this->m_height     = t.m_height;
			this->m_width      = t.m_width;

			t.m_font = nullptr;
		}

		Text& Text::operator=(Text&& t) noexcept
		{
			if (this != &t)
			{
				this->m_batch      = std::move(t.m_batch);
				this->m_batch_data = std::move(t.m_batch_data);
				this->m_colour     = std::move(t.m_colour);
				this->m_font       = t.m_font;
				this->m_height     = t.m_height;
				this->m_width      = t.m_width;

				t.m_font = nullptr;
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
			m_font = SL_HANDLE.fontbook()->get(font);

			m_font_str = font;
			m_colour   = col;
		}

		void Text::create(std::string_view text)
		{
			m_text_str = text;
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
							m_batch_data.emplace(counter, CharacterBatch {});

							auto* data = &m_batch_data[counter];
							data->m_sprite.create(c_obj->m_region, 1);
							data->m_transform.set_pos(x_offset + c_obj->m_bearing.x, y_offset);
							x_offset += (c_obj->m_advance >> 6);

							m_batch.add(&data->m_sprite, &data->m_transform, 0);
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
		}

		void Text::update_text(std::string_view text)
		{
			m_batch.clear();
			m_batch_data.clear();

			m_text_str = text;
			create(text);
		}

		void Text::bind()
		{
			m_batch.bind();
		}

		void Text::unbind() noexcept
		{
			m_batch.unbind();
		}

		std::array<float, 4> Text::get_colour() noexcept
		{
			return m_colour.normalized();
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
			return m_batch.get_width();
		}

		const int Text::get_batch_height() const noexcept
		{
			return m_batch.get_height();
		}

		const unsigned int Text::index_count() const noexcept
		{
			return m_batch.get_used_index_count();
		}

		nlohmann::json Text::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["font"]        = m_font_str;
			json["text"]        = m_text_str;

			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			return json;
		}

		void Text::deserialize(const nlohmann::json& json)
		{
			m_batch.clear();
			m_batch_data.clear();

			const auto colour = json.at("colour");
			load(json.at("font"), {colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")});
			create(json.at("text"));
		}
	} // namespace components
} // namespace galaxy