///
/// AnimationComponent.cpp
/// starlight
///
/// Created by reworks on 16/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/utils/Time.hpp"
#include "sl/libs/tmx/tmx.h"
#include "sl/core/ServiceLocator.hpp"
#include "sl/graphics/TextureAtlas.hpp"

#include "AnimationComponent.hpp"

namespace sl
{
	AnimationComponent::AnimationComponent()
		:m_currentFrameTime(0.0), m_isPaused(true), m_activeAnimation("")
	{
	}

	AnimationComponent::AnimationComponent(const sol::table& table)
		:m_currentFrameTime(0.0), m_isPaused(table.get<bool>("paused")), m_activeAnimation(table.get<std::string>("defaultAnim"))
	{
		// Set up component based on lua table data.
		sol::table animTable = table.get<sol::table>("Animations");
		if (!animTable.empty())
		{
			// Then create the animations.
			animTable.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				sol::table input = pair.second.as<sol::table>();
				m_animations.emplace(pair.first.as<std::string>(), Animation{ input });
			});
		}
		else
		{
			// Does not need to be a fatal error. Game will still work with no animations.
			LOG_S(ERROR) << "Could not create animations for defaultAnim: " << m_activeAnimation << ". Animation table empty.";
		}
	}

	AnimationComponent::AnimationComponent(tmx_map* map, tmx_tile* tile, int x, int y, int tileWidth, int tileHeight, const std::string& layerName)
		:m_currentFrameTime(0.0), m_isPaused(false)
	{
		// Construct animation from raw data from a tmx tile map.
		std::string aID = "AnimatedTile" + std::to_string(Time::getTimeSinceEpoch());
		m_animations.emplace(aID, Animation { true, 1.0f, tile->animation_len, static_cast<unsigned int>(0), std::vector<AnimationFrame>() });
		m_animations[aID].m_frames.clear(); // ensure empty with default values

		// Construct animations for the tiles.
		for (unsigned int i = 0; i < tile->animation_len; ++i)
		{
			int subX = x + map->tiles[tile->animation[i].tile_id + 1]->ul_x;
			int subY = y + map->tiles[tile->animation[i].tile_id + 1]->ul_y;

			std::string iaID = layerName + "AnimatedTileInternal" + std::to_string(Time::getTimeSinceEpoch());
			Locator::textureAtlas->addRectToAtlas(iaID, { subX, subY, tileWidth, tileHeight });
			m_animations[aID].m_frames.emplace_back(AnimationFrame { static_cast<std::uint32_t>(tile->animation[i].duration), iaID });
		}
		
		m_activeAnimation = aID;
	}

	AnimationComponent::~AnimationComponent()
	{
		m_animations.clear();
	}

	void AnimationComponent::changeAnimation(const std::string& animation)
	{
		// Simply reset the time and change the active animation.
		m_animations[m_activeAnimation].m_currentFrame = 0;
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
		// Resets the animation to the beginning.
		m_isPaused = true;
		m_animations[m_activeAnimation].m_currentFrame = 0;
		m_currentFrameTime = 0.0;
	}
}