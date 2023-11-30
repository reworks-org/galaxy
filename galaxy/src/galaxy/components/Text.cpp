///
/// Text.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Primitives.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/utils/StringUtils.hpp"

#include "Text.hpp"

namespace galaxy
{
	namespace components
	{
		Text::Text()
			: Serializable {}
			, m_font {nullptr}
			, m_width {0.0f}
			, m_height {0.0f}
			, m_size {0.0f}
			, m_alignment {Alignment::LEFT}
			, m_tex_id {0}
			, m_layer {0}
		{
		}

		Text::Text(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Text::Text(Text&& t)
			: Serializable {}
		{
			this->m_colour    = std::move(t.m_colour);
			this->m_vao       = std::move(t.m_vao);
			this->m_rt        = std::move(t.m_rt);
			this->m_font_name = t.m_font_name;
			this->m_font      = t.m_font;
			this->m_width     = t.m_width;
			this->m_height    = t.m_height;
			this->m_text      = std::move(t.m_text);
			this->m_size      = t.m_size;
			this->m_alignment = t.m_alignment;
			this->m_tex_id    = t.m_tex_id;
			this->m_layer     = t.m_layer;

			t.m_font   = nullptr;
			t.m_tex_id = 0;
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
				this->m_width     = t.m_width;
				this->m_height    = t.m_height;
				this->m_text      = std::move(t.m_text);
				this->m_size      = t.m_size;
				this->m_alignment = t.m_alignment;
				this->m_tex_id    = t.m_tex_id;
				this->m_layer     = t.m_layer;

				t.m_font   = nullptr;
				t.m_tex_id = 0;
			}

			return *this;
		}

		Text::~Text()
		{
			m_font = nullptr;
		}

		void
		Text::create(std::string_view text, const float size, const std::string& font, const graphics::Colour& colour, const int layer, Alignment alignment)
		{
			m_colour    = colour;
			m_font_name = font;
			m_text      = text;
			m_size      = size;
			m_alignment = alignment;
			m_layer     = layer;

			strutils::replace_all(m_text, "\t", "    "); // Handle tabs.

			if (!font.empty())
			{
				auto& fonts = core::ServiceLocator<resource::Fonts>::ref();
				m_font      = fonts.get(m_font_name);

				const auto vec = m_font->get_text_size(m_text, m_size);
				m_width        = vec.x;
				m_height       = vec.y;

				m_rt.create(static_cast<int>(m_width), static_cast<int>(m_height));
				m_tex_id = m_rt.get_texture();

				m_rt.bind(true);

				std::size_t start = 0;
				std::size_t end   = m_text.find('\n');

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
						glm::value_ptr(m_rt.get_proj()),
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
					glm::value_ptr(m_rt.get_proj()),
					static_cast<msdfgl_printf_flags>(MSDFGL_UTF8 | MSDFGL_KERNING),
					last_block.c_str());

				m_rt.unbind();

				auto vertices = graphics::Vertex::gen_quad_vertices(static_cast<int>(m_width), static_cast<int>(m_height));
				m_vao.create(vertices, graphics::StorageFlag::DYNAMIC_DRAW, graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);
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

			if (!m_font_name.empty())
			{
				auto& fonts = core::ServiceLocator<resource::Fonts>::ref();
				m_font      = fonts.get(m_font_name);

				const auto vec = m_font->get_text_size(m_text, m_size);
				m_width        = vec.x;
				m_height       = vec.y;

				m_rt.resize(static_cast<int>(m_width), static_cast<int>(m_height));
				m_rt.bind(true);

				std::size_t start = 0;
				std::size_t end   = m_text.find('\n');

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
						glm::value_ptr(m_rt.get_proj()),
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
					glm::value_ptr(m_rt.get_proj()),
					static_cast<msdfgl_printf_flags>(MSDFGL_UTF8 | MSDFGL_KERNING),
					last_block.c_str());

				m_rt.unbind();

				auto vertices = graphics::Vertex::gen_quad_vertices(static_cast<int>(m_width), static_cast<int>(m_height));
				m_vao.sub_buffer(0, vertices);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to update text without a font.");
			}
		}

		void Text::update(std::string_view text, const float size)
		{
			m_size = size;

			update(text);
		}

		void Text::update(std::string_view text, const graphics::Colour& colour)
		{
			m_colour = colour;

			update(text);
		}

		void Text::update(std::string_view text, const float size, const graphics::Colour& colour)
		{
			m_size   = size;
			m_colour = colour;

			update(text);
		}

		void Text::update(std::string_view text, Alignment alignment)
		{
			m_alignment = alignment;

			update(text);
		}

		void Text::update(std::string_view text, const float size, Alignment alignment)
		{
			m_size      = size;
			m_alignment = alignment;

			update(text);
		}

		void Text::update(std::string_view text, const graphics::Colour& colour, Alignment alignment)
		{
			m_colour    = colour;
			m_alignment = alignment;

			update(text);
		}

		void Text::update(std::string_view text, const float size, const graphics::Colour& colour, Alignment alignment)
		{
			m_size      = size;
			m_colour    = colour;
			m_alignment = alignment;

			update(text);
		}

		float Text::get_width() const
		{
			return m_width;
		}

		float Text::get_height() const
		{
			return m_height;
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

		int Text::get_instances() const
		{
			return 1;
		}

		unsigned int Text::get_mode() const
		{
			return graphics::primitive_to_gl(graphics::Primitives::TRIANGLE);
		}

		unsigned int Text::get_vao() const
		{
			return m_vao.id();
		}

		unsigned int Text::get_texture() const
		{
			return m_tex_id;
		}

		unsigned int Text::get_count() const
		{
			return m_vao.index_count();
		}

		int Text::get_layer() const
		{
			return m_layer;
		}

		nlohmann::json Text::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["text"]      = m_text;
			json["size"]      = m_size;
			json["font"]      = m_font_name;
			json["layer"]     = m_layer;
			json["alignment"] = static_cast<int>(m_alignment);

			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			return json;
		}

		void Text::deserialize(const nlohmann::json& json)
		{
			graphics::Colour colour;
			const auto&      colson = json.at("colour");

			colour.m_red   = colson.at("r");
			colour.m_green = colson.at("g");
			colour.m_blue  = colson.at("b");
			colour.m_alpha = colson.at("a");

			int alignment = json.at("alignment");
			alignment     = std::clamp(alignment, 0, 2);

			create(json.at("text"), json.at("size"), json.at("font"), colour, json.at("layer"), static_cast<Alignment>(alignment));
		}
	} // namespace components
} // namespace galaxy
