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

#include <qs/core/Shader.hpp>
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
		void render(qs::Camera& camera) override;

	private:
		galaxy::World* m_world;
		qs::Window* m_window;

		qs::Shader m_shader;
		qs::ParticleGenerator m_particle_gen;
		qs::Point m_point;
		qs::Circle m_circle;
	};
} // namespace sb

#endif