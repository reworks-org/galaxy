///
/// TextComponent.cpp
/// starlight
///
/// Created by reworks on 19/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "sol2/sol.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/graphics/TextureAtlas.hpp"

#include "TextComponent.hpp"

namespace sl
{
	TextComponent::TextComponent(entt::Entity entity, const sol::table& table)
	{
		m_id = table.get<std::string>("id");
		m_text = table.get<std::string>("text");
		m_font = FontBook::inst()->get(entt::HashedString{ table.get<std::string>("font").c_str() });

		sol::table colour = table.get<sol::table>("colour");
		m_colour = al_map_rgba(colour.get<unsigned char>("r"), colour.get<unsigned char>("g"), colour.get<unsigned char>("b"), colour.get<unsigned char>("a"));

		TextureAtlas::inst()->addTextToAtlas(m_id.c_str(), m_text, m_font, m_colour);
	}

	TextComponent& TextComponent::operator=(const TextComponent&)
	{
		return *this;
	}
}