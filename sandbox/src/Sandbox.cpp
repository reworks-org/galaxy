///
/// Sandbox.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include "Sandbox.hpp"

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

Sandbox::Sandbox(const std::string& name)
	: Scene {name}
{
	auto& w = entt::locator<galaxy::Window>::value();

	tex.load("textures/test.png");
	tex2.load("textures/test2.png");
	tex3.load("textures/test3.png");

	verts  = galaxy::graphics::gen_quad_vertices(640, 480, 0.3f);
	verts2 = galaxy::graphics::gen_quad_vertices(1024, 1024, 0.1f);
	verts3 = galaxy::graphics::gen_quad_vertices(1024, 1024, 0.2f);

	shader.parse(vert, frag);
	shader.compile();

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

	cmd1 = galaxy::RenderCmd {.type = galaxy::GLRenderMode::QUADS, .vertices = verts};
	cmd2 = galaxy::RenderCmd {.type = galaxy::GLRenderMode::QUADS, .vertices = verts2};
	cmd3 = galaxy::RenderCmd {.type = galaxy::GLRenderMode::QUADS, .vertices = verts3};

	cmd1.data.entity    = 0;
	cmd1.data.transform = glm::mat4 {1.0f};
	cmd1.data.handle    = tex.handle();
	cmd2.data.entity    = 1;
	cmd2.data.transform = glm::mat4 {1.0f};
	cmd2.data.handle    = tex2.handle();
	cmd3.data.entity    = 2;
	cmd3.data.transform = glm::mat4 {1.0f};
	cmd3.data.handle    = tex3.handle();
}

Sandbox::~Sandbox()
{
}

void Sandbox::on_push()
{
}

void Sandbox::on_pop()
{
}

void Sandbox::on_event(SDL_Event& event)
{
}

void Sandbox::update(galaxy::EntityManager& em)
{
}

void Sandbox::render()
{
	renderer.prepare();
	renderer.begin_pass(pass);

	renderer.bind_pipeline(pipeline);

	renderer.push_cmd(cmd1);
	renderer.push_cmd(cmd2);
	renderer.push_cmd(cmd3);

	renderer.end_pass(camera);
}
