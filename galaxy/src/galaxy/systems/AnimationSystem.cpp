///
/// AnimationSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/components/Animated.hpp"
#include "galaxy/components/Flag.hpp"
#include "galaxy/components/Map.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/flags/Enabled.hpp"
#include "galaxy/resource/TextureAtlas.hpp"
#include "galaxy/scene/Scene.hpp"
#include "galaxy/utils/Globals.hpp"

#include "AnimationSystem.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26462)
#endif

namespace galaxy
{
	namespace systems
	{
		AnimationSystem::AnimationSystem() noexcept
		{
		}

		AnimationSystem::~AnimationSystem() noexcept
		{
		}

		void AnimationSystem::update(scene::Scene* scene)
		{
			// Sprites.
			const auto view = scene->m_world.m_registry.view<components::Animated, components::Flag>();
			for (auto&& [entity, animated, flag] : view.each())
			{
				if (flag.is_flag_set<flags::Enabled>() && !animated.is_paused())
				{
					const auto active_anim = animated.active();
					if (active_anim != nullptr)
					{
						animated.m_time_spent_on_frame += (GALAXY_DT * active_anim->m_speed);

						const auto current = active_anim->current_frame();
						if (current != nullptr)
						{
							if (animated.m_time_spent_on_frame >= current->m_time_per_frame)
							{
								animated.m_time_spent_on_frame = 0;

								const auto next = active_anim->next_frame();
								if (next != nullptr)
								{
									auto sprite = scene->m_world.m_registry.try_get<components::Sprite>(entity);
									if (sprite != nullptr)
									{
										sprite->update(next->m_texture_id);
									}
								}
							}
						}
					}
				}
			}

			auto& atlas = core::ServiceLocator<resource::TextureAtlas>::ref();

			// Tiles.
			const auto view2 = scene->m_world.m_registry.view<components::Map, components::Flag>();
			for (auto&& [entity, map, flag] : view2.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					for (auto& [id, anim] : map.m_animations)
					{
						const auto orig = anim.m_anim->getCurrentFrameNumber();
						anim.m_anim->update(GALAXY_DT * 1000);

						if (orig != anim.m_anim->getCurrentFrameNumber())
						{
							const auto animated_tile = anim.m_tileset->getTile(anim.m_anim->getCurrentTileId());

							if (animated_tile)
							{
								auto& texture_rect = animated_tile->getDrawingRect();
								auto info_opt      = atlas.query(anim.m_tileset->getImage().stem().string());

								if (info_opt.has_value())
								{
									const auto& info = info_opt.value().get();

									const auto off_x = info.m_region.m_x + texture_rect.x;
									const auto off_y = info.m_region.m_y + texture_rect.y;

									for (auto& [index, vertices] : anim.m_tiles_to_animate)
									{
										vertices[0].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
										vertices[0].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

										vertices[1].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + texture_rect.width, info.m_sheet_width);
										vertices[1].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

										vertices[2].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + texture_rect.width, info.m_sheet_width);
										vertices[2].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + texture_rect.width, info.m_sheet_height);

										vertices[3].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
										vertices[3].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + texture_rect.height, info.m_sheet_height);

										map.m_spritebatch.sub_buffer(index, vertices);
									}
								}
							}
						}
					}
				}
			}
		}
	} // namespace systems
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif