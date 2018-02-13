///
/// AnimationComponent.cpp
/// starlight
///
/// Created by reworks on 16/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "tmx/tmx.h"
#include "sol2/sol.hpp"
#include "loguru/loguru.hpp"
#include "sl/graphics/TextureAtlas.hpp"

#include "AnimationComponent.hpp"

namespace sl
{
	AnimationComponent::AnimationComponent(entt::Entity entity, const sol::table& table)
		:m_currentFrameTime(0.0)
	{
		m_isPaused = table.get<bool>("paused");
		m_activeAnimation = table.get<std::string_view>("defaultAnim");

		sol::table animTable = table.get<sol::table>("Animations");
		if (!animTable.empty())
		{
			animTable.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				m_animations.emplace(pair.first.as<std::string_view>(), pair.second.as<sol::table>());
			});
		}
		else
		{
			LOG_S(WARNING) << "Tried to load AnimationComponent with no Animations!";
		}
	}

	AnimationComponent::AnimationComponent(tmx_map* map, tmx_tile* tile, int x, int y, int tileWidth, int tileHeight, const std::string& layerName)
		:m_currentFrameTime(0.0), m_isPaused(false)
	{
		std::vector<std::string_view> frames;
		frames.clear(); // ensure empty, no junk data allowed
		frames.reserve(tile->animation_len); // helps prevent too many reallocations

		for (unsigned int i = 0; i < tile->animation_len; ++i)
		{
			int subX = x + map->tiles[tile->animation[i].tile_id]->ul_x;
			int subY = y + map->tiles[tile->animation[i].tile_id]->ul_y;

			std::string id = layerName + "AnimatedTileInternal" + std::to_string(i);
			TextureAtlas::inst()->addRectToAtlas(id, { subX, subY, tileWidth, tileHeight });
			frames.push_back(id.c_str());

			m_animations.emplace(std::make_pair<std::string_view, Animation>(id.c_str(), { true, 1.0f, static_cast<std::uint32_t>(tile->animation[i].duration), tile->animation_len, 0, frames }));
		}

		frames.shrink_to_fit(); // ensure no extra elements
		m_activeAnimation = frames[0];
	}

	AnimationComponent::~AnimationComponent()
	{
		m_animations.clear();
	}

	void AnimationComponent::changeAnimation(std::string_view animation)
	{
		m_animations[m_activeAnimation].m_currentFrame = 0;
		m_currentFrameTime = 0.0;
		m_activeAnimation = animation;
	}

	void AnimationComponent::play()
	{
		m_isPaused = false;
	}

	void AnimationComponent::play(std::string_view animation)
	{
		if (m_activeAnimation != animation)
		{
			changeAnimation(animation);
		}
			
		play();
	}

	void AnimationComponent::pause()
	{
		m_isPaused = true;
	}

	void AnimationComponent::stop()
	{
		m_isPaused = true;
		m_animations[m_activeAnimation].m_currentFrame = 0;
		m_currentFrameTime = 0.0;
	}

	AnimationComponent& AnimationComponent::operator=(const AnimationComponent&)
	{
		return *this;
	}
}