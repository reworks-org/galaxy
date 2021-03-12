///
/// PhysicsScene.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_LAYERS_SCENES_PHYSICSSCENE_HPP_
#define SANDBOXTESTS_LAYERS_SCENES_PHYSICSSCENE_HPP_

#include <galaxy/scenes/Scene.hpp>
#include <galaxy/ecs/Entity.hpp>
#include <galaxy/events/dispatcher/Dispatcher.hpp>

namespace sb
{
	class PhysicsScene final : public galaxy::scenes::Scene
	{
	public:
		PhysicsScene(std::string_view name);
		virtual ~PhysicsScene();

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

		[[nodiscard]] nlohmann::json sub_serialize() override;
		void sub_deserialize(const nlohmann::json& json) override;

	private:
		galaxy::events::Dispatcher m_dispatcher;
		galaxy::ecs::Entity m_cube;
		galaxy::ecs::Entity m_floor;
	};
} // namespace sb

#endif