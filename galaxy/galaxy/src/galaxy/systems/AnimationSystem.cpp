///
/// AnimationSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/All.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"

#include "AnimationSystem.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	AnimationSystem::AnimationSystem()
	    : m_world {nullptr}
	{
		m_world = SL_HANDLE.world();
	}

	AnimationSystem::AnimationSystem(const AnimationSystem& rs)
	{
		this->m_world = rs.m_world;
	}

	AnimationSystem::AnimationSystem(AnimationSystem&& rs)
	{
		this->m_world = rs.m_world;
		rs.m_world    = nullptr;
	}

	AnimationSystem& AnimationSystem::operator=(const AnimationSystem& rs)
	{
		this->m_world = rs.m_world;
		return *this;
	}

	AnimationSystem& AnimationSystem::operator=(AnimationSystem&& rs)
	{
		if (this != &rs)
		{
			this->m_world = rs.m_world;
			rs.m_world    = nullptr;
		}

		return *this;
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
		m_world->operate<AnimationComponent, EnabledComponent>([&](AnimationComponent* ac, EnabledComponent* ec) {
			ac->m_abs.update(dt);
		});
	}

	void AnimationSystem::render(qs::Camera& camera)
	{
	}
} // namespace galaxy