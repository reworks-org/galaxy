///
/// SandboxLayer.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_SANDBOXLAYER_HPP_
#define SUPERCLUSTER_SANDBOXLAYER_HPP_

#include <qs/core/Window.hpp>
#include <galaxy/core/World.hpp>
#include <galaxy/layer/Layer.hpp>

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
		qs::ParticleGenerator<qs::CircularGen> m_particle_gen;
	};
} // namespace sb

#endif