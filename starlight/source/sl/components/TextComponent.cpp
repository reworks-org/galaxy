///
/// TextComponent.cpp
/// starlight
///
/// Created by reworks on 19/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "sl/libs/sol2/sol.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/graphics/TextureAtlas.hpp"

#include "TextComponent.hpp"

namespace sl
{
	TextComponent::TextComponent(entt::Entity entity, const sol::table& table)
	{
		m_id = table.get<std::string>("id");
		m_text = table.get<std::string>("text");
		m_font = Locator::m_fontBook->get(entt::HashedString{ table.get<const char*>("font") });

		sol::table colour = table.get<sol::table>("colour");
		m_colour = al_map_rgba(colour.get<unsigned char>("r"), colour.get<unsigned char>("g"), colour.get<unsigned char>("b"), colour.get<unsigned char>("a"));

		Locator::m_textureAtlas->addTextToAtlas(m_id.c_str(), m_text, m_font, m_colour);
	}

	TextComponent& TextComponent::operator=(const TextComponent&)
	{
		return *this;
	}
}