///
/// Sandbox.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_LAYERS_SANDBOX_HPP_
#define SANDBOXTESTS_LAYERS_SANDBOX_HPP_

#include <galaxy/core/Window.hpp>
#include <galaxy/core/Layer.hpp>
#include <galaxy/graphics/Camera.hpp>

namespace sb
{
	class Sandbox final : public galaxy::core::Layer
	{
	public:
		Sandbox();
		virtual ~Sandbox();

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		galaxy::core::Window* m_window;

		//galaxy::graphics::ParticleGenerator m_particle_gen;
		//galaxy::graphics::Point m_point;
		//galaxy::graphics::Circle m_circle;
		//galaxy::physics::KineticBody* m_test_body;
		//galaxy::audio::Sound m_boop;
	};
} // namespace sb

#endif