///
/// SandboxLayer.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/components/SpriteComponent.hpp>
#include <qs/core/Renderer.hpp>
#include <galaxy/components/ShaderComponent.hpp>
#include <galaxy/flags/EnabledFlag.hpp>
#include <galaxy/fs/FileSystem.hpp>

#include "SandboxLayer.hpp"

///
/// Core namespace.
///
namespace sb
{
	SandboxLayer::SandboxLayer()
	{
		set_name("sandbox_layer");
		m_window = SL_HANDLE.window();
		m_world  = SL_HANDLE.world();

		auto vs = galaxy::FileSystem::s_root + galaxy::FileSystem::s_shaders + "particle.vs";
		auto fs = galaxy::FileSystem::s_root + galaxy::FileSystem::s_shaders + "particle.fs";
		m_shader.load_path(vs, fs);

		// create and set texture
		auto tex = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "particle_demo.png";
		m_particle_gen.create(tex, &m_shader);
		m_particle_gen.define("default", {0, 0, 16, 16});
		m_particle_gen.configure("default", 100, 0, 50);
	}

	SandboxLayer::~SandboxLayer()
	{
		m_world  = nullptr;
		m_window = nullptr;
	}

	void SandboxLayer::events()
	{
		if (glfwGetKey(m_window->gl_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			m_window->close();
		}

		m_world->events();
	}

	void SandboxLayer::update(const double dt)
	{
		m_world->update(dt);
	}

	void SandboxLayer::render(qs::Camera& camera)
	{
		// set shader
		m_shader.bind();
		m_shader.set_uniform("u_opacity", 1.0f);
		m_shader.set_uniform<float>("u_width", 16.0f);
		m_shader.set_uniform<float>("u_height", 16.0f);
		m_shader.set_uniform("u_cameraProj", camera.get_proj());
		m_shader.set_uniform("u_cameraView", camera.get_transform());

		SL_HANDLE.renderer()->draw_particles(m_particle_gen);
		//m_world->get_system<galaxy::RenderSystem>()->render(camera);
	}
} // namespace sb