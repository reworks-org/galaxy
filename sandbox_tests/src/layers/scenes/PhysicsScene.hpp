///
/// PhysicsScene.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_LAYERS_SCENES_PHYSICSSCENE_HPP_
#define SANDBOXTESTS_LAYERS_SCENES_PHYSICSSCENE_HPP_

#include <galaxy/core/Scene.hpp>
#include <galaxy/events/dispatcher/Dispatcher.hpp>
#include <galaxy/physics/Box2DIntegration.hpp>

namespace sb
{
	class PhysicsScene final : public galaxy::core::Scene
	{
	public:
		PhysicsScene();
		virtual ~PhysicsScene();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		galaxy::events::Dispatcher m_dispatcher;
		galaxy::physics::GalaxyContactListener m_contact_listener;
	};
} // namespace sb

#endif