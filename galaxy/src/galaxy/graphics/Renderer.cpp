///
/// Renderer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Point.hpp"
#include "galaxy/components/Line.hpp"
#include "galaxy/components/Circle.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Renderables.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/shaders/DefaultFramebuffer.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "Renderer.hpp"

namespace galaxy
{
	namespace graphics
	{
		void Renderer::init(const unsigned int max_quads, std::string_view batch_shader)
		{
			auto* handle = SL_HANDLE.shaderbook()->create("DefaultFramebuffer");
			handle->load_raw(shaders::default_framebuffer_vert, shaders::default_framebuffer_frag);
			Renderer::m_post_shaders.push_back(handle);

			m_batch        = std::make_unique<graphics::SpriteBatch>(max_quads);
			m_batch_shader = SL_HANDLE.shaderbook()->get(batch_shader);
		}

		void Renderer::submit_point(components::Point* point, components::Transform* transform, Shader* shader)
		{
			point->bind();
			shader->set_uniform("u_point_size", point->get_size());
			shader->set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_POINTS, point->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::submit_line(components::Line* line, components::Transform* transform, Shader* shader)
		{
			line->bind();
			shader->set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_LINES, line->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::submit_circle(components::Circle* circle, components::Transform* transform, Shader* shader)
		{
			circle->bind();
			shader->set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_LINE_LOOP, circle->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::submit_sprite(components::Sprite* sprite, components::Transform* transform, Shader* shader)
		{
			sprite->bind();

			shader->set_uniform("u_transform", transform->get_transform());
			shader->set_uniform("u_opacity", sprite->opacity());
			shader->set_uniform("u_width", static_cast<float>(sprite->get_width()));
			shader->set_uniform("u_height", static_cast<float>(sprite->get_height()));

			glDrawElements(GL_TRIANGLES, sprite->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::submit_text(components::Text* text, components::Transform* transform, Shader* shader)
		{
			text->m_batch.calculate(transform);
			text->bind();

			shader->set_uniform("u_colour", text->get_colour());
			shader->set_uniform("u_width", static_cast<float>(text->get_batch_width()));
			shader->set_uniform("u_height", static_cast<float>(text->get_batch_height()));

			glDrawElements(GL_TRIANGLES, text->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::submit_batch(Camera& camera)
		{
			Renderer::m_batch_shader->bind();
			Renderer::m_batch_shader->set_uniform("u_cameraProj", camera.get_proj());
			Renderer::m_batch_shader->set_uniform("u_cameraView", camera.get_transform());
			Renderer::m_batch_shader->set_uniform("u_width", static_cast<float>(Renderer::m_batch->get_width()));
			Renderer::m_batch_shader->set_uniform("u_height", static_cast<float>(Renderer::m_batch->get_height()));

			Renderer::m_batch->bind();
			glDrawElements(GL_TRIANGLES, Renderer::m_batch->get_used_index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::draw_sprite_to_texture(components::Sprite* sprite, components::Transform* transform, Shader* shader, RenderTexture* target)
		{
			sprite->bind();

			shader->bind();
			shader->set_uniform("u_projection", target->get_proj());
			shader->set_uniform("u_transform", transform->get_transform());
			shader->set_uniform("u_width", static_cast<float>(sprite->get_width()));
			shader->set_uniform("u_height", static_cast<float>(sprite->get_height()));

			glDrawElements(GL_TRIANGLES, sprite->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		/*
		void Renderer::draw_animated_sprite(AnimatedSprite& sprite, Shader& shader)
		{
			sprite.bind();

			shader.set_uniform("u_transform", sprite.get_transform());
			shader.set_uniform("u_opacity", sprite.opacity());
			shader.set_uniform<float>("u_width", static_cast<float>(sprite.get_width()));
			shader.set_uniform<float>("u_height", static_cast<float>(sprite.get_height()));

			glDrawElements(GL_TRIANGLES, sprite.index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::draw_particles(ParticleGenerator& particle_gen, Shader& shader)
		{
			if (!particle_gen.finished())
			{
				particle_gen.bind();

				auto* current = particle_gen.get_instance();

				shader.set_uniform<float>("u_width", current->get_width());
				shader.set_uniform<float>("u_height", current->get_height());

				glDrawElementsInstanced(GL_TRIANGLES, particle_gen.gl_index_count(), GL_UNSIGNED_INT, nullptr, particle_gen.amount());
			}
		}

		void Renderer::add_post_effect(PostEffect* effect)
		{
			m_effects.push_back(effect);
		}

		std::span<PostEffect*> Renderer::get_post_effects()
		{
			return m_effects;
		}

		void Renderer::clear_post_effects()
		{
			m_effects.clear();
		}
		*/
	} // namespace graphics
} // namespace galaxy