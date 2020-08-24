///
/// LuaEntity.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LUAENTITY_HPP_
#define GALAXY_LUAENTITY_HPP_

#include <solar/Config.hpp>

#include "galaxy/components/All.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// RAII class encapsulating entity functionality for Lua.
	///
	class LuaEntity final
	{
	public:
		///
		/// Create a brand new entity.
		///
		LuaEntity();

		///
		/// Create entity with a debug name.
		///
		LuaEntity(std::string_view debug_name);

		///
		/// Create from existing entity.
		///
		LuaEntity(sr::Entity entity);

		///
		/// Copy constructor.
		///
		LuaEntity(const LuaEntity&) noexcept = delete;

		///
		/// Move constructor.
		///
		LuaEntity(LuaEntity&&) noexcept;

		///
		/// Copy assignment operator.
		///
		LuaEntity& operator=(const LuaEntity&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		LuaEntity& operator=(LuaEntity&&) noexcept;

		///
		/// Destructor.
		///
		~LuaEntity() noexcept = default;

		///
		/// Enable the entity.
		///
		void enable();

		///
		/// Disable the entity.
		///
		void disable();

		///
		/// Add AnimationComponent to lua.
		///
		[[nodiscard]] AnimationComponent* add_animation();

		///
		/// Add MusicComponent to lua.
		///
		[[nodiscard]] MusicComponent* add_music();

		///
		/// Add ShaderComponent to lua.
		///
		[[nodiscard]] ShaderComponent* add_shader();

		///
		/// Add SoundComponent to lua.
		///
		[[nodiscard]] SoundComponent* add_sound();

		///
		/// Add SpriteBatchComponent to lua.
		///
		[[nodiscard]] SpriteBatchComponent* add_spritebatch();

		///
		/// Add SpriteComponent to lua.
		///
		[[nodiscard]] SpriteComponent* add_sprite();

		///
		/// Retrieve entity currrently stored.
		///
		/// \return Const sr::Entity.
		///
		[[nodiscard]] const sr::Entity retrieve() const noexcept;

	private:
		///
		/// Entity to encapsulate for Lua.
		///
		sr::Entity m_entity;
	};
} // namespace galaxy

#endif