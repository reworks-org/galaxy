///
/// SandboxLayer.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_SANDBOXLAYER_HPP_
#define SANDBOXTESTS_SANDBOXLAYER_HPP_

#include <qs/core/Window.hpp>
#include <galaxy/core/World.hpp>
#include <galaxy/layer/Layer.hpp>
#include <qs/shapes/Point.hpp>
#include <qs/shapes/Circle.hpp>
#include <qs/post/PointLight.hpp>
#include <qs/post/Simple.hpp>
#include <qs/graphics/Camera.hpp>

#include <qs/graphics/ParticleGenerator.hpp>

namespace sb
{
	class SandboxLayer final : public galaxy::Layer
	{
	public:
		SandboxLayer();
		virtual ~SandboxLayer();

		void events() override;
		void update(const double dt) override;
		void render() override;

	private:
		galaxy::World* m_world;
		qs::Window* m_window;

		qs::ParticleGenerator m_particle_gen;
		qs::Point m_point;
		qs::Circle m_circle;
		qs::effect::PointLight m_pl;
		qs::effect::Simple m_simple;
		qs::Camera m_camera;
	};
} // namespace sb

#endif