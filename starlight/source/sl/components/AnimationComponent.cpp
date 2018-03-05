///
/// AnimationComponent.cpp
/// starlight
///
/// Created by reworks on 16/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/tmx/tmx.h"
#include "sl/libs/sol2/sol.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/graphics/TextureAtlas.hpp"

#include "AnimationComponent.hpp"

namespace sl
{
	AnimationComponent::AnimationComponent(entt::Entity entity, const sol::table& table)
		:m_currentFrameTime(0.0)
	{
		m_isPaused = table.get<bool>("paused");
		m_activeAnimation = table.get<std::string>("defaultAnim");

		sol::table animTable = table.get<sol::table>("Animations");
		if (!animTable.empty())
		{
			animTable.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				m_animations.emplace(pair.first.as<std::string>(), new Animation{ pair.second.as<sol::table>() });
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
		std::vector<std::string> frames;
		frames.reserve(tile->animation_len); // helps prevent too many reallocations

		for (unsigned int i = 0; i < tile->animation_len; ++i)
		{
			int subX = x + map->tiles[tile->animation[i].tile_id + 1]->ul_x;
			int subY = y + map->tiles[tile->animation[i].tile_id + 1]->ul_y;

			std::string id = layerName + "AnimatedTileInternal" + std::to_string(i);
			Locator::m_textureAtlas->addRectToAtlas(id, { subX, subY, tileWidth, tileHeight });
			frames.push_back(id);

			Animation* anim = new Animation(true, 1.0f, static_cast<std::uint32_t>(tile->animation[i].duration), tile->animation_len, static_cast<unsigned int>(0), frames);
			m_animations.emplace(id, anim);
		}

		frames.shrink_to_fit(); // ensure no extra elements
		m_activeAnimation = frames[0];
	}

	AnimationComponent::~AnimationComponent()
	{
		for (auto& pair : m_animations)
		{
			delete pair.second;
		}

		m_animations.clear();
	}

	void AnimationComponent::changeAnimation(const std::string& animation)
	{
		m_animations.at(m_activeAnimation)->m_currentFrame = 0;
		m_currentFrameTime = 0.0;
		m_activeAnimation = animation;
	}

	void AnimationComponent::play()
	{
		m_isPaused = false;
	}

	void AnimationComponent::play(const std::string& animation)
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
		m_animations.at(m_activeAnimation)->m_currentFrame = 0;
		m_currentFrameTime = 0.0;
	}

	AnimationComponent& AnimationComponent::operator=(const AnimationComponent&)
	{
		return *this;
	}
}