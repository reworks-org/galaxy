///
/// Text.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Text.hpp"

namespace galaxy
{
	namespace components
	{
		Text::Text()
			: Serializable {}
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
			this->m_text = std::move(t.m_text);
		}

		Text& Text::operator=(Text&& t)
		{
			if (this != &t)
			{
				this->m_text = std::move(t.m_text);
			}

			return *this;
		}

		Text::~Text()
		{
		}

		nlohmann::json Text::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["text"]      = m_text.get_text();
			json["size"]      = m_text.get_size();
			json["font"]      = m_text.get_font();
			json["alignment"] = static_cast<int>(m_text.get_alignment());

			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_text.m_colour.r<std::uint8_t>();
			json["colour"]["g"] = m_text.m_colour.g<float>();
			json["colour"]["b"] = m_text.m_colour.b<float>();
			json["colour"]["a"] = m_text.m_colour.a<float>();

			return json;
		}

		void Text::deserialize(const nlohmann::json& json)
		{
			graphics::Colour colour;
			const auto&      colson = json.at("colour");

			m_text.m_colour.set_r(colson.at("r").get<std::uint8_t>());
			m_text.m_colour.set_g(colson.at("g").get<std::uint8_t>());
			m_text.m_colour.set_b(colson.at("b").get<std::uint8_t>());
			m_text.m_colour.set_a(colson.at("a").get<std::uint8_t>());

			const int alignment = json.at("alignment");
			m_text.create(json.at("text"), json.at("size"), json.at("font"), colour, static_cast<graphics::Text::Alignment>(alignment));
		}
	} // namespace components
} // namespace galaxy
