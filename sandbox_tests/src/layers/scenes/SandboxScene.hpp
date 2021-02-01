///
/// Sandbox.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_LAYERS_SCENES_SANDBOXSCENE_HPP_
#define SANDBOXTESTS_LAYERS_SCENES_SANDBOXSCENE_HPP_

#include <iostream>
#include <thread>
#include <chrono>

#include <galaxy/async/Timer.hpp>
#include <galaxy/core/Scene.hpp>
#include <galaxy/graphics/particle/ParticleGenerator.hpp>
#include <galaxy/ui/GUI.hpp>

namespace sb
{
	class SandboxScene final : public galaxy::core::Scene
	{
	public:
		SandboxScene();
		virtual ~SandboxScene();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		galaxy::async::Timer m_timer;
		galaxy::graphics::ParticleGenerator m_particle_gen;
		galaxy::ui::GUI m_gui;
		galaxy::ui::Theme m_theme;
	};
} // namespace sb

#endif