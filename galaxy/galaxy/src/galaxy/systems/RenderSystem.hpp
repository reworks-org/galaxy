///
/// RenderSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RENDERSYSTEM_HPP_
#define GALAXY_RENDERSYSTEM_HPP_

#include <qs/core/Shader.hpp>
#include <solar/system/System.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	class World;

	///
	/// System that handles rendering of entities with a SpriteComponent, TransformComponent, etc.
	///
	class RenderSystem final : public sr::System
	{
	public:
		///
		/// Constructor.
		///
		RenderSystem() noexcept;

		///
		/// Destructor.
		///
		~RenderSystem() noexcept override;

		///
		/// Abstract implementation for processing events.
		///
		void events() noexcept override;

		///
		/// Abstract implementation for updating the system. Use the manager to retreive your components.
		///
		/// \param time DeltaTime from gameloop.
		/// \param manager Entity manager.
		///
		void update(protostar::ProtectedDouble* deltaTime, sr::Manager& manager) noexcept override;

		///
		/// Render sprites / textures to screen.
		///
		/// \param world Galaxy gameworld.
		/// \param shader Shader to apply when drawing.
		/// 
		void render(galaxy::World* world, qs::Shader& shader);
	};
}

#endif