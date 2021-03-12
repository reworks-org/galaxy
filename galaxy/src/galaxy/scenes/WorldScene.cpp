///
/// WorldScene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/systems/AnimationSystem.hpp>
#include <galaxy/systems/CollisionSystem.hpp>
#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/systems/TransformSystem.hpp>

#include "WorldScene.hpp"

namespace galaxy
{
	namespace scenes
	{
		WorldScene::WorldScene(std::string_view name) noexcept
		    : Scene {name, Types::WORLD}
		{
			m_camera.create(0.0f, 1024, 1024, 0.0f);
			m_camera.set_speed(100.0f);

			m_world.create_system<systems::AnimationSystem>();
			m_world.create_system<systems::TransformSystem>();
			m_world.create_system<systems::RenderSystem>();
			m_world.create_system<systems::CollisionSystem>(nullptr);
		}

		WorldScene::~WorldScene() noexcept
		{
		}

		void WorldScene::on_push()
		{
		}

		void WorldScene::on_pop()
		{
		}

		void WorldScene::events()
		{
		}

		void WorldScene::update(const double dt)
		{
			m_camera.update(dt);
			m_world.update(dt);
		}

		void WorldScene::pre_render()
		{
		}

		void WorldScene::render()
		{
			m_world.get_system<systems::RenderSystem>()->render(m_world, m_camera);
		}

		nlohmann::json WorldScene::sub_serialize()
		{
			nlohmann::json json = "{}"_json;

			return json;
		}

		void WorldScene::sub_deserialize(const nlohmann::json& json)
		{
		}
	} // namespace scenes
} // namespace galaxy