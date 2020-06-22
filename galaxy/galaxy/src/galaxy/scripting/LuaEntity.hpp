///
/// LuaEntity.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LUAENTITY_HPP_
#define GALAXY_LUAENTITY_HPP_

#include <solar/Config.hpp>

#include "galaxy/components/SpriteComponent.hpp"
#include "galaxy/components/ShaderComponent.hpp"
#include "galaxy/components/TransformComponent.hpp"

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
		LuaEntity() noexcept;
		LuaEntity(const std::string& debugName) noexcept;
		LuaEntity(sr::Entity entity) noexcept;
		~LuaEntity() noexcept;

		void enable() noexcept;
		void disable() noexcept;

		SpriteComponent* addSpriteComponent() noexcept;
		TransformComponent* addTransformComponent() noexcept;
		ShaderComponent* addShaderComponent() noexcept;

		const sr::Entity retrieve() const noexcept;

	private:
		///
		/// Entity to encapsulate for Lua.
		///
		sr::Entity m_entity;
	};
}

#endif