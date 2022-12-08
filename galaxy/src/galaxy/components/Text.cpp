///
/// Text.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/utils/StringUtils.hpp"

#include "Text.hpp"

namespace galaxy
{
	namespace components
	{
		Text::Text() noexcept
			: Renderable {}
			, Serializable {}
			, m_font {nullptr}
			, m_width {0.0f}
			, m_height {0.0f}
			, m_size {0.0f}
		{
		}

		Text::Text(const nlohmann::json& json)
			: Renderable {}
			, Serializable {}
			, m_font {nullptr}
			, m_width {0.0f}
			, m_height {0.0f}
			, m_size {0.0f}
		{
			deserialize(json);
		}

		Text::Text(Text&& s) noexcept
			: Renderable {std::move(s)}
			, Serializable {}
			, m_font {nullptr}
			, m_width {0.0f}
			, m_height {0.0f}
			, m_size {0.0f}
		{
			this->m_colour  = std::move(s.m_colour);
			this->m_vao     = std::move(s.m_vao);
			this->m_rt      = std::move(s.m_rt);
			this->m_font_id = std::move(s.m_font_id);
			this->m_font    = s.m_font;
			this->m_width   = s.m_width;
			this->m_height  = s.m_height;
			this->m_text    = std::move(s.m_text);
			this->m_size    = s.m_size;

			s.m_font = nullptr;
		}

		Text& Text::operator=(Text&& s) noexcept
		{
			if (this != &s)
			{
				this->Renderable::operator=(std::move(s));

				this->m_colour  = std::move(s.m_colour);
				this->m_vao     = std::move(s.m_vao);
				this->m_rt      = std::move(s.m_rt);
				this->m_font_id = std::move(s.m_font_id);
				this->m_font    = s.m_font;
				this->m_width   = s.m_width;
				this->m_height  = s.m_height;
				this->m_text    = std::move(s.m_text);
				this->m_size    = s.m_size;

				s.m_font = nullptr;
			}

			return *this;
		}

		Text::~Text() noexcept
		{
			m_font = nullptr;
		}

		void Text::create(std::string_view text, const float size, const std::string& font, const graphics::Colour& colour, const int layer)
		{
			m_colour  = colour;
			m_font_id = font;
			m_size    = size;

			m_layer = layer;

			m_text = text;
			strutils::replace_all(m_text, "\t", "    "); // Handle tabs.

			if (!font.empty())
			{
				auto& fonts = core::ServiceLocator<resource::Fonts>::ref();
				m_font      = fonts.get(m_font_id);

				const auto vec = m_font->get_text_size(m_text, m_size);
				m_width        = vec.x;
				m_height       = vec.y;

				m_rt.create(static_cast<int>(m_width), static_cast<int>(m_height));
				m_texture_id = m_rt.get_texture();

				m_rt.bind(true);

				std::size_t start = 0;
				std::size_t end   = m_text.find('\n');

				auto y_off = m_font->vertical_advance(m_size);
				while (end != std::string::npos)
				{
					const auto block = m_text.substr(start, end);
					msdfgl_printf(0,
						y_off,
						m_font->handle(),
						m_size,
						0xffffffff,
						glm::value_ptr(m_rt.get_proj()),
						static_cast<msdfgl_printf_flags>(MSDFGL_UTF8 | MSDFGL_KERNING),
						block.c_str());

					y_off += m_font->vertical_advance(m_size);
					start = end + 1;
					end   = m_text.find('\n', start);
				}

				const auto last_block = m_text.substr(start, end);

				msdfgl_printf(0,
					y_off,
					m_font->handle(),
					m_size,
					0xffffffff,
					glm::value_ptr(m_rt.get_proj()),
					static_cast<msdfgl_printf_flags>(MSDFGL_UTF8 | MSDFGL_KERNING),
					last_block.c_str());

				m_rt.unbind();

				auto vertices = graphics::Vertex::gen_quad_vertices(static_cast<int>(m_width), static_cast<int>(m_height));
				m_vao.create(vertices, graphics::StorageFlag::DYNAMIC_DRAW, graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);

				configure();
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

			if (!m_font_id.empty())
			{
				auto& fonts = core::ServiceLocator<resource::Fonts>::ref();
				m_font      = fonts.get(m_font_id);

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
						m_font->handle(),
						m_size,
						0xffffffff,
						glm::value_ptr(m_rt.get_proj()),
						static_cast<msdfgl_printf_flags>(MSDFGL_UTF8 | MSDFGL_KERNING),
						block.c_str());

					y_off += m_font->vertical_advance(m_size);
					start = end + 1;
					end   = m_text.find('\n', start);
				}

				const auto last_block = m_text.substr(start, end);

				msdfgl_printf(0,
					y_off,
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

		float Text::get_width() const noexcept
		{
			return m_width;
		}

		float Text::get_height() const noexcept
		{
			return m_height;
		}

		const std::string& Text::get_text() const noexcept
		{
			return m_text;
		}

		float Text::get_size() const noexcept
		{
			return m_size;
		}

		const std::string& Text::get_font() const noexcept
		{
			return m_font_id;
		}

		void Text::configure() noexcept
		{
			m_vao_id      = m_vao.id();
			m_index_count = m_vao.index_count();
			m_instances   = 1;
			set_primitive_type(graphics::Primitives::TRIANGLE);
		}

		nlohmann::json Text::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["text"]  = m_text;
			json["size"]  = m_size;
			json["font"]  = m_font_id;
			json["layer"] = m_layer;

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
			const auto& colson = json.at("colour");

			colour.m_red   = colson.at("r");
			colour.m_green = colson.at("g");
			colour.m_blue  = colson.at("b");
			colour.m_alpha = colson.at("a");

			create(json.at("text"), json.at("size"), json.at("font"), colour, json.at("layer"));
		}
	} // namespace components
} // namespace galaxy