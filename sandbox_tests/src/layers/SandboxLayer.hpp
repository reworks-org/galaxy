///
/// SandboxLayer.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_SANDBOXLAYER_HPP_
#define SANDBOXTESTS_SANDBOXLAYER_HPP_

#include <galaxy/core/World.hpp>
#include <galaxy/core/Layer.hpp>
#include <galaxy/graphics/Camera.hpp>
#include <galaxy/graphics/shapes/Point.hpp>
#include <galaxy/graphics/shapes/Circle.hpp>
#include <galaxy/graphics/particle/ParticleGenerator.hpp>
#include <galaxy/graphics/post/Simple.hpp>
#include <galaxy/graphics/Window.hpp>
#include "galaxy/physics/body/KineticBody.hpp"

namespace sb
{
	class SandboxLayer final : public galaxy::core::Layer
	{
	public:
		SandboxLayer();
		virtual ~SandboxLayer();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		galaxy::core::World* m_world;
		galaxy::graphics::Window* m_window;

		galaxy::graphics::ParticleGenerator m_particle_gen;
		galaxy::graphics::Point m_point;
		galaxy::graphics::Circle m_circle;
		galaxy::posteffect::Simple m_simple;
		galaxy::graphics::Camera m_camera;
		galaxy::physics::KineticBody* m_test_body;
	};
} // namespace sb

#endif