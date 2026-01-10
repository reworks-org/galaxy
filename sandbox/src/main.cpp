///
/// main.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include <format>

#include <mimalloc.h>
#include <mimalloc-new-delete.h>

#include <entt/signal/dispatcher.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <tinyfiledialogs.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/graphics/Renderer.hpp>
#include <galaxy/graphics/gl/Shader.hpp>
#include <galaxy/graphics/gl/Texture.hpp>
#include <galaxy/graphics/Enums.hpp>
#include <galaxy/scene/World.hpp>

#include "tests/LoggingTests.hpp"
#include "tests/PlatformTests.hpp"

#include "TestConfig.hpp"

const auto vert = R"(
	#version 460 core
	#extension GL_ARB_bindless_texture : require
	#extension GL_ARB_gpu_shader_int64 : enable

	struct UniformData
	{
		mat4 transform;
		vec4 colour;
		int entity;
		bool point;
		uint64_t handle;
	};

	layout(location = 0) in vec3 l_pos;
	layout(location = 1) in vec2 l_texels;
	layout(location = 2) in uint l_index;

	layout(std430, binding = 1) readonly buffer camera_data
	{
		mat4 u_camera_model_view;
		mat4 u_camera_proj;
	};

	layout(std430, binding = 2) readonly buffer uniform_data
	{
		UniformData u_uniforms[];
	};

	out vec2 io_texels;
	out flat uint64_t io_handle;
	
	void main()
	{
		UniformData data = u_uniforms[l_index];
		gl_Position = u_camera_proj * u_camera_model_view * data.transform * vec4(l_pos, 1.0);

		io_texels = l_texels;
		io_texels.y = 1.0 - io_texels.y;
		io_handle = data.handle;
		//io_handle = data.entity;
	}
)";
const auto frag = R"(
	#version 460 core
	#extension GL_ARB_bindless_texture : require
	#extension GL_ARB_gpu_shader_int64 : enable

	in vec2 io_texels;
	in flat uint64_t io_handle;

	out vec4 io_frag_colour;

	void main()
	{
		sampler2D tex = sampler2D(io_handle);
		io_frag_colour = texture(tex, io_texels);
		//io_frag_colour = vec4(io_handle, 1.0f, 1.0f, 1.0f);
	}
)";

int main(int argc, char* argv[])
{
	mi_version();

	GALAXY_UNUSED(argc);
	GALAXY_UNUSED(argv);

	try
	{
		auto app = galaxy::App();

		if (ENABLE_PLATFORM_TESTS)
		{
			platform_tests();
		}

		if (ENABLE_LOGGING_TESTS)
		{
			logging_tests();
		}

		auto&           w = entt::locator<galaxy::Window>::value();
		galaxy::Texture tex;
		tex.load("textures/test.png");

		galaxy::Texture tex2;
		tex2.load("textures/test2.png");

		galaxy::Texture tex3;
		tex3.load("textures/test3.png");

		auto verts  = galaxy::graphics::gen_quad_vertices(640, 480, 0.3f);
		auto verts2 = galaxy::graphics::gen_quad_vertices(1024, 1024, 0.1f);
		auto verts3 = galaxy::graphics::gen_quad_vertices(1024, 1024, 0.2f);

		galaxy::Shader shader;
		shader.parse(vert, frag);
		shader.compile();

		galaxy::RenderPass     pass;
		galaxy::RenderPipeline pipeline;
		galaxy::Renderer       renderer;
		galaxy::Camera         camera;

		app.set_update_func([&](entt::dispatcher& dispatcher, galaxy::Window& window, galaxy::World& world) {
			window.process_events(dispatcher);
		});

		pass.target      = 0;
		pass.viewport    = glm::vec4(0, 0, w.get_pixel_size().x, w.get_pixel_size().y);
		pass.clear_col   = {0, 0, 0, 0};
		pass.clear_depth = 1.0f;
		pass.scissor     = pass.viewport;

		pipeline.enabled.push_back(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		pipeline.enabled.push_back(GL_PROGRAM_POINT_SIZE);
		pipeline.enabled.push_back(GL_DEPTH_TEST);
		pipeline.enabled.push_back(GL_STENCIL_TEST);
		pipeline.enabled.push_back(GL_BLEND);
		pipeline.enabled.push_back(GL_DITHER);
		pipeline.enabled.push_back(GL_SCISSOR_TEST);

		pipeline.disabled.push_back(GL_CULL_FACE);
		pipeline.disabled.push_back(GL_FRAMEBUFFER_SRGB);
		pipeline.disabled.push_back(GL_MULTISAMPLE);

		pipeline.cull_face      = GL_BACK;
		pipeline.depth_func     = GL_LEQUAL;
		pipeline.blend_equation = GL_FUNC_ADD;
		pipeline.blend_function = GL_ONE_MINUS_SRC_ALPHA;
		pipeline.shader         = shader.id();

		camera.set_projection(0.0f, static_cast<float>(w.get_pixel_size().x), static_cast<float>(w.get_pixel_size().y), 0.0f);

		app.set_render_func([&](entt::dispatcher& dispatcher, galaxy::Window& window, galaxy::World& world) {
			renderer.prepare();
			renderer.begin_pass(pass);

			renderer.bind_pipeline(pipeline);

			auto cmd1           = galaxy::RenderCmd {.type = galaxy::GLRenderMode::QUADS, .vertices = verts};
			cmd1.data.entity    = 0;
			cmd1.data.transform = glm::mat4 {1.0f};
			cmd1.data.handle    = tex.handle();
			renderer.push_cmd(std::move(cmd1));

			auto cmd2           = galaxy::RenderCmd {.type = galaxy::GLRenderMode::QUADS, .vertices = verts2};
			cmd2.data.entity    = 1;
			cmd2.data.transform = glm::mat4 {1.0f};
			cmd2.data.handle    = tex2.handle();
			renderer.push_cmd(std::move(cmd2));

			auto cmd3           = galaxy::RenderCmd {.type = galaxy::GLRenderMode::QUADS, .vertices = verts3};
			cmd3.data.entity    = 2;
			cmd3.data.transform = glm::mat4 {1.0f};
			cmd3.data.handle    = tex3.handle();
			renderer.push_cmd(std::move(cmd3));

			renderer.end_pass(camera);
			window.swap();
		});

		app.run();
	}
	catch (const std::exception& e)
	{
		tinyfd_notifyPopup("Error", e.what(), "error");
	}

	return 0;
}
