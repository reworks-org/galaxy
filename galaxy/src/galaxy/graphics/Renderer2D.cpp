///
/// Renderer2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <execution>

#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform2D.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/resource/TextureBook.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

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

	layout(std140, binding = 0) uniform camera_data
	{
		mat4 u_camera_model_view;
		mat4 u_camera_proj;
	};
	
	uniform mat4 u_transform;

	void main()
	{
		gl_PointSize = 4;
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
		io_texels.x = (((l_texels.x - 0.0) * (1.0 - 0.0)) / (u_width - 0.0)) + 0.0;
		io_texels.y = 1.0 - (((l_texels.y - 0.0) * (1.0 - 0.0)) / (u_height - 0.0)) + 0.0;

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
		io_texels.x = (((l_texels.x - 0.0) * (1.0 - 0.0)) / (u_width - 0.0)) + 0.0;
		io_texels.y = 1.0 - (((l_texels.y - 0.0) * (1.0 - 0.0)) / (u_height - 0.0)) + 0.0;

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

	uniform int u_opacity;
	uniform sampler2D u_texture;

	void main()
	{
		io_frag_colour = texture(u_texture, io_texels);
		io_frag_colour.a *= (float(u_opacity) / 255.0);
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

	out vec2 io_texels;
	
	uniform mat4 u_projection;
	uniform mat4 u_transform;
	uniform float u_width;
	uniform float u_height;

	void main()
	{
		io_texels.x = (((l_texels.x - 0.0) * (1.0 - 0.0)) / (u_width - 0.0)) + 0.0;
		io_texels.y = 1.0 - (((l_texels.y - 0.0) * (1.0 - 0.0)) / (u_height - 0.0)) + 0.0;

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
		io_texels.x = (((l_texels.x - 0.0) * (1.0 - 0.0)) / (u_width - 0.0)) + 0.0;
		io_texels.y = 1.0 - (((l_texels.y - 0.0) * (1.0 - 0.0)) / (u_height - 0.0)) + 0.0;
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
		io_frag_colour.a *= io_opacity;
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

		Renderer2D::~Renderer2D() noexcept
		{
			clear();
		}

		Renderer2D& Renderer2D::inst() noexcept
		{
			static Renderer2D s_inst;
			return s_inst;
		}

		void Renderer2D::init(std::string_view layers)
		{
			const auto opt = json::parse_from_disk(layers);
			if (opt != std::nullopt)
			{
				const auto& root = opt.value().at("layers");
				for (const auto& [name, layer] : root.items())
				{
					m_layer_data.emplace(name, layer.get<int>());
				}

				// Set pointers to layer data.
				for (auto& [name, layer] : m_layer_data)
				{
					for (const auto& [key, atlas] : SL_HANDLE.texturebook()->get_all())
					{
						layer.m_batches.try_emplace(key);
						layer.m_batches[key].add_texture(atlas.gl_texture());
					}

					m_layers.push_back(&layer);
				}

				// Make sure order is correct.
				std::sort(std::execution::par, m_layers.begin(), m_layers.end(), [&](auto* left, auto* right) {
					return left->get_layer() < right->get_layer();
				});
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to create renderlayers from path: {0}.", layers);
			}
		}

		void Renderer2D::clear() noexcept
		{
			m_layer_data.clear();
			m_layers.clear();
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

		void Renderer2D::submit(components::Primitive2D* data, components::Transform2D* transform)
		{
			// clang-format off
			Renderable renderable = {
				.m_vao = data->vao(),
				.m_texture = 0,
				.m_index_count = data->count(),
				.m_configure_shader = [this, data, transform]()
				{
					this->m_point_shader.bind();
					this->m_point_shader.set_uniform("u_colour", data->get_colour().normalized());
					this->m_point_shader.set_uniform("u_transform", transform->get_transform());
			    }
			};
			// clang-format on

			switch (data->get_type())
			{
				case Primitives::CIRCLE:
				case Primitives::ELLIPSE:
				case Primitives::POLYGON:
				case Primitives::POLYLINE:
					renderable.m_type = GL_LINE_LOOP;
					break;

				case Primitives::LINE:
					renderable.m_type = GL_LINES;
					break;

				case Primitives::POINT:
					renderable.m_type = GL_POINTS;
					break;
			}

			m_layer_data.at(data->get_layer()).submit(renderable);
		}

		void Renderer2D::submit(components::Text* text, components::Transform2D* transform)
		{
			// clang-format off
			Renderable renderable = {
				.m_vao = text->vao(),
				.m_texture = text->gl_texture(),
				.m_index_count = text->count(),
				.m_type = GL_TRIANGLES,
				.m_configure_shader = [this, text, transform]()
				{
					this->m_text_shader.bind();
					this->m_text_shader.set_uniform("u_transform", transform->get_transform());
					this->m_text_shader.set_uniform("u_colour", text->get_colour().normalized());
					this->m_text_shader.set_uniform<float>("u_width", text->get_batch_width());
					this->m_text_shader.set_uniform<float>("u_height", text->get_batch_height());
				}
			};
			// clang-format on

			m_layer_data.at(text->get_layer()).submit(renderable);
		}

		void Renderer2D::submit(components::Sprite* sprite, components::Transform2D* transform)
		{
			// clang-format off
			Renderable renderable = {
				.m_vao = sprite->vao(),
				.m_texture = sprite->gl_texture(),
				.m_index_count = sprite->count(),
				.m_type =  GL_TRIANGLES,
				.m_configure_shader = [this, sprite, transform]()
				{
					this->m_sprite_shader.bind();
					this->m_sprite_shader.set_uniform("u_transform", transform->get_transform());
					this->m_sprite_shader.set_uniform<int>("u_opacity", sprite->get_opacity());
					this->m_sprite_shader.set_uniform<float>("u_width", sprite->get_width());
					this->m_sprite_shader.set_uniform<float>("u_height", sprite->get_height());
				}
			};
			// clang-format on

			m_layer_data.at(sprite->get_layer()).submit(renderable);
		}

		void Renderer2D::submit(components::BatchSprite* batch, components::Transform2D* transform)
		{
			m_layer_data.at(batch->get_layer()).m_batches[batch->get_atlas_index()].add(batch, transform);
		}

		void Renderer2D::prepare()
		{
			for (auto* layer : m_layers)
			{
				layer->clear();
			}
		}

		void Renderer2D::draw()
		{
			for (auto* layer : m_layers)
			{
				layer->submit_batched_sprites(m_spritebatch_shader);
				layer->draw();
			}
		}

		void Renderer2D::draw_sprite_to_target(components::Sprite* sprite, components::Transform2D* transform, RenderTexture* target)
		{
			sprite->bind();

			m_rtt_shader.set_uniform("u_projection", target->get_proj());
			m_rtt_shader.set_uniform("u_transform", transform->get_transform());
			m_rtt_shader.set_uniform<float>("u_width", sprite->get_width());
			m_rtt_shader.set_uniform<float>("u_height", sprite->get_height());

			glDrawElements(GL_TRIANGLES, sprite->count(), GL_UNSIGNED_INT, nullptr);
		}
	} // namespace graphics
} // namespace galaxy