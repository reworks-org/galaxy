///
/// SandboxLayer.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_SANDBOXLAYER_HPP_
#define SANDBOXTESTS_SANDBOXLAYER_HPP_

#include <galaxy/graphicsWindow.hpp>
#include <galaxy/core/World.hpp>
#include <galaxy/state/Layer.hpp>
#include <galaxy/graphics/shapes/Point.hpp>
#include <galaxy/graphics/shapes/Circle.hpp>
#include <galaxy/graphics/post/PointLight.hpp>
#include <galaxy/graphics/post/Simple.hpp>
#include <qs/graphics/Camera.hpp>

#include <qs/graphics/ParticleGenerator.hpp>

namespace sb
{
	class SandboxLayer final : public pr::Layer
	{
	public:
		SandboxLayer();
		virtual ~SandboxLayer();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		galaxy::World* m_world;
		Window* m_window;

		ParticleGenerator m_particle_gen;
		Point m_point;
		Circle m_circle;
		effect::PointLight m_pl;
		effect::Simple m_simple;
		graphics::Camera m_camera;
		rs::KineticBody* m_test_body;
	};
} // namespace sb

#endif