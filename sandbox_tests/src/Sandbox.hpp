///
/// Sandbox.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_SANDBOX_HPP_
#define SANDBOXTESTS_SANDBOX_HPP_

#include <galaxy/async/Timer.hpp>
#include <galaxy/core/Instance.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/graphics/Camera.hpp>
#include <galaxy/map/Map.hpp>

using namespace galaxy;

namespace sb
{
	class Sandbox final : public core::Instance
	{
	public:
		Sandbox();
		virtual ~Sandbox();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		void create_sandbox_scene();
		void create_physics_scene();
		void create_map_scene();

	private:
		core::Window* m_window;
		async::Timer m_timer;

		ecs::Entity m_cube;
		map::Map m_map;
	};
} // namespace sb

#endif