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
	LuaEntity::LuaEntity()
	{
		m_entity = SL_HANDLE.world()->create();
	}

	LuaEntity::LuaEntity(std::string_view debug_name)
	{
		m_entity = SL_HANDLE.world()->create(debug_name);
	}

	LuaEntity::LuaEntity(sr::Entity entity)
	{
		m_entity = entity;
	}

	LuaEntity::LuaEntity(LuaEntity&& le) noexcept
	{
		this->m_entity = le.m_entity;
		le.m_entity    = 0;
	}

	LuaEntity& LuaEntity::operator=(LuaEntity&& le) noexcept
	{
		if (this != &le)
		{
			this->m_entity = le.m_entity;
			le.m_entity    = 0;
		}

		return *this;
	}

	void LuaEntity::enable()
	{
		SL_HANDLE.world()->create_component<galaxy::EnabledFlag>(m_entity);
	}

	void LuaEntity::disable()
	{
		SL_HANDLE.world()->remove<galaxy::EnabledFlag>(m_entity);
	}

	AnimationComponent* LuaEntity::add_animation()
	{
		return SL_HANDLE.world()->create_component<galaxy::AnimationComponent>(m_entity);
	}

	MusicComponent* LuaEntity::add_music()
	{
		return SL_HANDLE.world()->create_component<galaxy::MusicComponent>(m_entity);
	}

	ShaderComponent* LuaEntity::add_shader()
	{
		return SL_HANDLE.world()->create_component<galaxy::ShaderComponent>(m_entity);
	}

	SoundComponent* LuaEntity::add_sound()
	{
		return SL_HANDLE.world()->create_component<galaxy::SoundComponent>(m_entity);
	}

	SpriteBatchComponent* LuaEntity::add_spritebatch()
	{
		return SL_HANDLE.world()->create_component<galaxy::SpriteBatchComponent>(m_entity);
	}

	SpriteComponent* LuaEntity::add_sprite()
	{
		return SL_HANDLE.world()->create_component<galaxy::SpriteComponent>(m_entity);
	}

	const sr::Entity LuaEntity::retrieve() const noexcept
	{
		return m_entity;
	}
} // namespace galaxy