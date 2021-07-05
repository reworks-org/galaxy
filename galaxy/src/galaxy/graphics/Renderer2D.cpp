///
/// Renderer2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform2D.hpp"
#include "galaxy/graphics/RenderTexture.hpp"

#include "Renderer2D.hpp"

///
/// Index of camera uniform buffer in shaders.
///
#define CAMERA_UBO_INDEX 0

///
/// Point vertex shader.
///
constexpr const char* const point_vert = R"(
	#version 450 core
	layout(location = 0) in vec2 l_pos;
	layout(location = 1) in vec2 l_texels;
	layout(location = 2) in vec4 l_colour;
	layout(location = 3) in int l_depth;

	layout(std140, binding = 0) uniform camera_data
	{
		mat4 u_camera_model_view;
		mat4 u_camera_proj;
	};
	
	uniform mat4 u_transform;
	uniform int u_point_size;

	void main()
	{
		gl_PointSize = u_point_size;
		gl_Position  = u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);
	}
)";

///
/// Point fragment shader.
///
constexpr const char* const point_frag = R"(
	#version 450 core

	out vec4 io_frag_colour;
	
	uniform vec4 u_colour;

	void main()
	{
		io_frag_colour = u_colour;
	}
)";

///
/// Line vertex shader.
///
constexpr const char* const line_vert = R"(
	#version 450 core
	layout(location = 0) in vec2 l_pos;
	layout(location = 1) in vec2 l_texels;
	layout(location = 2) in vec4 l_colour;
	layout(location = 3) in int l_depth;

	layout(std140, binding = 0) uniform camera_data
	{
		mat4 u_camera_model_view;
		mat4 u_camera_proj;
	};

	uniform mat4 u_transform;

	void main()
	{
		gl_Position = u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);
	}
)";

///
/// Line fragment shader.
///
constexpr const char* const line_frag = R"(
	#version 450 core

	out vec4 io_frag_colour;

	uniform vec4 u_colour;

	void main()
	{
		io_frag_colour = u_colour;
	}
)";

///
/// Text vertex shader.
///
constexpr const char* const text_vert = R"(
	#version 450 core
	layout(location = 0) in vec2 l_pos;
	layout(location = 1) in vec2 l_texels;
	layout(location = 2) in vec4 l_colour;
	layout(location = 3) in int l_depth;

	out vec2 io_texels;
	
	layout(std140, binding = 0) uniform camera_data
	{
		mat4 u_camera_model_view;
		mat4 u_camera_proj;
	};

	uniform float u_width;
	uniform float u_height;
	uniform mat4 u_transform;

	void main()
	{
		io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));

		gl_Position =  u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);
	}
)";

///
/// Text fragment shader.
///
constexpr const char* const text_frag = R"(
	#version 450 core

	in vec2 io_texels;
	out vec4 io_frag_colour;

	uniform vec4 u_colour;
	uniform sampler2D u_texture;

	void main()
	{
		io_frag_colour = texture(u_texture, io_texels) * u_colour;
	}
)";

///
/// Sprite vertex shader.
///
constexpr const char* const sprite_vert = R"(
	#version 450 core
	layout(location = 0) in vec2 l_pos;
	layout(location = 1) in vec2 l_texels;
	layout(location = 2) in vec4 l_colour;
	layout(location = 3) in int l_depth;

	out vec2 io_texels;
	
	layout(std140, binding = 0) uniform camera_data
	{
		mat4 u_camera_model_view;
		mat4 u_camera_proj;
	};

	uniform mat4 u_transform;
	uniform float u_width;
	uniform float u_height;
	
	void main()
	{
		io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));

		gl_Position =  u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);
	}
)";

///
/// Sprite fragment shader.
///
constexpr const char* const sprite_frag = R"(
	#version 450 core

	in vec2 io_texels;
	out vec4 io_frag_colour;

	uniform float u_opacity;
	uniform sampler2D u_texture;

	void main()
	{
		io_frag_colour = texture(u_texture, io_texels);
		io_frag_colour.a = u_opacity;
	}
)";

///
/// RenderToTexture vertex shader.
///
constexpr const char* const render_to_texture_vert = R"(
	#version 450 core
	layout(location = 0) in vec2 l_pos;
	layout(location = 1) in vec2 l_texels;
	layout(location = 2) in vec4 l_colour;
	layout(location = 3) in int l_depth;

	out vec2 io_texels;
	
	uniform mat4 u_projection;
	uniform mat4 u_transform;
	uniform float u_width;
	uniform float u_height;

	void main()
	{
		io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));

		gl_Position = u_projection * u_transform * vec4(l_pos, 0.0, 1.0);
	}
)";

///
/// RenderToTexture fragment shader.
///
constexpr const char* const render_to_texture_frag = R"(
	#version 450 core

	in vec2 io_texels;
	out vec4 io_frag_colour;

	uniform sampler2D u_texture;

	void main()
	{
		io_frag_colour = texture(u_texture, io_texels);
	}
)";

///
/// SpriteBatch vertex shader.
///
constexpr const char* const spritebatch_vert = R"(
	#version 450 core
	layout(location = 0) in vec2 l_pos;
	layout(location = 1) in vec2 l_texels;
	layout(location = 2) in vec4 l_colour;
	layout(location = 3) in int l_depth;

	out vec2 io_texels;
	out float io_opacity;
	
	layout(std140, binding = 0) uniform camera_data
	{
		mat4 u_camera_model_view;
		mat4 u_camera_proj;
	};

	uniform float u_width;
	uniform float u_height;

	void main()
	{
		io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));
		io_opacity = l_colour.a;		

		gl_Position =  u_camera_proj * u_camera_model_view * vec4(l_pos, 0.0, 1.0);
	}
)";

///
/// SpriteBatch fragment shader.
///
constexpr const char* const spritebatch_frag = R"(
	#version 450 core

	in vec2 io_texels;
	in float io_opacity;
	out vec4 io_frag_colour;

	uniform sampler2D u_texture;

	void main()
	{
		io_frag_colour = texture(u_texture, io_texels);
		io_frag_colour.a = io_opacity;
	}
)";

namespace galaxy
{
	namespace graphics
	{
		Renderer2D::Renderer2D() noexcept
		{
			m_point_shader.load_raw(point_vert, point_frag);
			m_line_shader.load_raw(line_vert, line_frag);
			m_text_shader.load_raw(text_vert, text_frag);
			m_sprite_shader.load_raw(sprite_vert, sprite_frag);
			m_rtt_shader.load_raw(render_to_texture_vert, render_to_texture_frag);
			m_spritebatch_shader.load_raw(spritebatch_vert, spritebatch_frag);

			m_camera_ubo.create(CAMERA_UBO_INDEX);
			m_camera_ubo.reserve(sizeof(Camera2D::Data));
		}

		Renderer2D& Renderer2D::inst() noexcept
		{
			static Renderer2D s_inst;
			return s_inst;
		}

		void Renderer2D::buffer_camera(Camera2D& camera)
		{
			m_camera_ubo.sub_buffer<Camera2D::Data>(0, 1, &camera.get_data());
		}

		void Renderer2D::bind_point() noexcept
		{
			m_point_shader.bind();
		}

		void Renderer2D::bind_line() noexcept
		{
			m_line_shader.bind();
		}

		void Renderer2D::bind_text() noexcept
		{
			m_text_shader.bind();
		}

		void Renderer2D::bind_sprite() noexcept
		{
			m_sprite_shader.bind();
		}

		void Renderer2D::bind_rtt() noexcept
		{
			m_rtt_shader.bind();
		}

		void Renderer2D::bind_sb_shader() noexcept
		{
			m_spritebatch_shader.bind();
		}

		void Renderer2D::draw_point(components::Primitive2D* data, components::Transform2D* transform)
		{
			data->bind();

			m_point_shader.set_uniform("u_colour", data->get_colour().normalized());
			m_point_shader.set_uniform("u_point_size", data->get_data().m_pointsize.value());
			m_point_shader.set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_POINTS, data->count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_line(components::Primitive2D* data, components::Transform2D* transform)
		{
			data->bind();

			m_line_shader.set_uniform("u_colour", data->get_colour().normalized());
			m_line_shader.set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_LINES, data->count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_lineloop(components::Primitive2D* data, components::Transform2D* transform)
		{
			data->bind();

			m_line_shader.set_uniform("u_colour", data->get_colour().normalized());
			m_line_shader.set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_LINE_LOOP, data->count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_text(components::Text* text, components::Transform2D* transform)
		{
			text->bind();

			m_text_shader.set_uniform("u_transform", transform->get_transform());
			m_text_shader.set_uniform("u_colour", text->get_colour().normalized());
			m_text_shader.set_uniform("u_width", static_cast<float>(text->get_batch_width()));
			m_text_shader.set_uniform("u_height", static_cast<float>(text->get_batch_height()));

			glDrawElements(GL_TRIANGLES, text->count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_sprite(components::Sprite* sprite, components::Transform2D* transform)
		{
			sprite->bind();

			m_sprite_shader.set_uniform("u_transform", transform->get_transform());
			m_sprite_shader.set_uniform("u_opacity", static_cast<float>(sprite->get_opacity()) / static_cast<float>(0xFF));
			m_sprite_shader.set_uniform("u_width", static_cast<float>(sprite->get_width()));
			m_sprite_shader.set_uniform("u_height", static_cast<float>(sprite->get_height()));

			glDrawElements(GL_TRIANGLES, sprite->count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_sprite_to_target(components::Sprite* sprite, components::Transform2D* transform, RenderTexture* target)
		{
			sprite->bind();

			m_rtt_shader.set_uniform("u_projection", target->get_proj());
			m_rtt_shader.set_uniform("u_transform", transform->get_transform());
			m_rtt_shader.set_uniform("u_width", static_cast<float>(sprite->get_width()));
			m_rtt_shader.set_uniform("u_height", static_cast<float>(sprite->get_height()));

			glDrawElements(GL_TRIANGLES, sprite->count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_spritebatch(SpriteBatch* batch)
		{
			batch->bind();

			m_spritebatch_shader.set_uniform("u_width", static_cast<float>(batch->get_width()));
			m_spritebatch_shader.set_uniform("u_height", static_cast<float>(batch->get_height()));

			glDrawElements(GL_TRIANGLES, batch->count(), GL_UNSIGNED_INT, nullptr);
		}
	} // namespace graphics
} // namespace galaxy