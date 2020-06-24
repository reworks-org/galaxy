///
/// LuaEntity.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/World.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "galaxy/flags/EnabledFlag.hpp"

#include "LuaEntity.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	LuaEntity::LuaEntity() noexcept
	{
		auto* world = SL_HANDLE.world();
		m_entity = world->create();
	}

	LuaEntity::LuaEntity(const std::string& debugName) noexcept
	{
		auto* world = SL_HANDLE.world();
		m_entity = world->create(debugName);
	}

	LuaEntity::LuaEntity(sr::Entity entity) noexcept
	{
		m_entity = entity;
	}

	LuaEntity::~LuaEntity() noexcept
	{
		m_entity = NULL;
	}

	void LuaEntity::enable() noexcept
	{
		auto* world = SL_HANDLE.world();
		world->add<galaxy::EnabledFlag>(m_entity);
	}

	void LuaEntity::disable() noexcept
	{
		auto* world = SL_HANDLE.world();
		world->remove<galaxy::EnabledFlag>(m_entity);
	}

	SpriteComponent* LuaEntity::addSpriteComponent() noexcept
	{
		auto* world = SL_HANDLE.world();
		return world->add<galaxy::SpriteComponent>(m_entity);
	}

	TransformComponent* LuaEntity::addTransformComponent() noexcept
	{
		auto* world = SL_HANDLE.world();
		return world->add<galaxy::TransformComponent>(m_entity);
	}

	ShaderComponent* LuaEntity::addShaderComponent() noexcept
	{
		auto* world = SL_HANDLE.world();
		return world->add<galaxy::ShaderComponent>(m_entity);
	}
	
	const sr::Entity LuaEntity::retrieve() const noexcept
	{
		return m_entity;
	}
}