///
/// AnimationSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"
#include "galaxy/graphics/anim/AnimatedBatchSprite.hpp"

#include "AnimationSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		AnimationSystem::AnimationSystem()
		    : m_world {nullptr}
		{
			m_world = SL_HANDLE.world();
		}

		AnimationSystem::~AnimationSystem()
		{
			m_world = nullptr;
		}

		void AnimationSystem::events()
		{
		}

		void AnimationSystem::update(const double dt)
		{
			m_world->operate<graphics::AnimatedBatchSprite>([&](const ecs::Entity entity, graphics::AnimatedBatchSprite* ac) {
				ac->update(dt);
			});
		}

		void AnimationSystem::render(graphics::Camera& camera)
		{
		}
	} // namespace systems
} // namespace galaxy