///
/// PhysicsScene.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_LAYERS_SCENES_PHYSICSSCENE_HPP_
#define SANDBOXTESTS_LAYERS_SCENES_PHYSICSSCENE_HPP_

#include <galaxy/core/Scene.hpp>
#include <galaxy/ecs/Entity.hpp>
#include <galaxy/events/dispatcher/Dispatcher.hpp>

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
		galaxy::ecs::Entity m_cube;
		galaxy::ecs::Entity m_floor;
	};
} // namespace sb

#endif