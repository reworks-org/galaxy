///
/// AnimationSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ANIMATIONSYSTEM_HPP_
#define GALAXY_ANIMATIONSYSTEM_HPP_

#include <qs/graphics/Camera.hpp>
#include <galaxy/system/System.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// System that handles rendering of entities with a SpriteComponent, TransformComponent, etc.
	///
	class AnimationSystem final : public sr::System
	{
	public:
		///
		/// Constructor.
		///
		AnimationSystem();

		///
		/// Copy constructor.
		///
		AnimationSystem(const AnimationSystem&);

		///
		/// Move constructor.
		///
		AnimationSystem(AnimationSystem&&);

		///
		/// Copy assignment operator.
		///
		AnimationSystem& operator=(const AnimationSystem&);

		///
		/// Move assignment operator.
		///
		AnimationSystem& operator=(AnimationSystem&&);

		///
		/// Destructor.
		///
		virtual ~AnimationSystem();

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

	private:
		///
		/// Pointer to entities.
		///
		World* m_world;
	};
} // namespace galaxy

#endif