///
/// RenderSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RENDERSYSTEM_HPP_
#define GALAXY_RENDERSYSTEM_HPP_

#include <qs/core/Shader.hpp>
#include <qs/graphics/Camera.hpp>
#include <solar/system/System.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// System that handles rendering of entities with a SpriteComponent, TransformComponent, etc.
	///
	class RenderSystem final : public sr::System
	{
	public:
		///
		/// Constructor.
		///
		RenderSystem();

		///
		/// Copy constructor.
		///
		RenderSystem(const RenderSystem&) noexcept = default;

		///
		/// Move constructor.
		///
		RenderSystem(RenderSystem&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		RenderSystem& operator=(const RenderSystem&) noexcept = default;

		///
		/// Move assignment operator.
		///
		RenderSystem& operator=(RenderSystem&&) noexcept = default;

		///
		/// Destructor.
		///
		virtual ~RenderSystem();

		///
		/// Abstract implementation for processing events.
		///
		void events() override;

		///
		/// Abstract implementation for updating the system. Use the manager to retreive your components.
		///
		/// \param dt DeltaTime from gameloop.
		///
		void update(const double dt) override;

		///
		/// Render sprites / textures to screen.
		///
		/// \param camera Camera used for rendering.
		///
		void render(qs::Camera& camera);
	};
} // namespace galaxy

#endif