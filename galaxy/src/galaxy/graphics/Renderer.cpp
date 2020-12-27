///
/// Renderer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/mat4x4.hpp>

#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/particle/ParticleGenerator.hpp"
#include "galaxy/graphics/post/PostEffect.hpp"
#include "galaxy/graphics/shapes/Circle.hpp"
#include "galaxy/graphics/shapes/Line.hpp"
#include "galaxy/graphics/shapes/Point.hpp"
#include "galaxy/graphics/anim/AnimatedSprite.hpp"
#include "galaxy/graphics/sprite/Sprite.hpp"
#include "galaxy/graphics/sprite/SpriteBatch.hpp"
#include "galaxy/graphics/text/Text.hpp"
#include "galaxy/graphics/texture/RenderTexture.hpp"

#include "Renderer.hpp"

namespace galaxy
{
	namespace graphics
	{
		void Renderer::draw_point(Point& point, Shader& shader)
		{
			point.bind();
			shader.set_uniform("u_point_size", point.get_size());
			shader.set_uniform("u_transform", point.get_transform());

			glDrawElements(GL_POINTS, point.index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::draw_line(Line& line, Shader& shader)
		{
			line.bind();
			shader.set_uniform("u_transform", line.get_transform());

			glDrawElements(GL_LINES, line.index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::draw_circle(Circle& circle, Shader& shader)
		{
			circle.bind();
			shader.set_uniform("u_transform", circle.get_transform());

			glDrawElements(GL_LINE_LOOP, circle.index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::draw_sprite(Sprite& sprite, Shader& shader)
		{
			sprite.bind();

			shader.set_uniform("u_transform", sprite.get_transform());
			shader.set_uniform("u_opacity", sprite.opacity());
			shader.set_uniform<float>("u_width", static_cast<float>(sprite.get_width()));
			shader.set_uniform<float>("u_height", static_cast<float>(sprite.get_height()));

			glDrawElements(GL_TRIANGLES, sprite.index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::draw_animated_sprite(AnimatedSprite& sprite, Shader& shader)
		{
			sprite.bind();

			shader.set_uniform("u_transform", sprite.get_transform());
			shader.set_uniform("u_opacity", sprite.opacity());
			shader.set_uniform<float>("u_width", static_cast<float>(sprite.get_width()));
			shader.set_uniform<float>("u_height", static_cast<float>(sprite.get_height()));

			glDrawElements(GL_TRIANGLES, sprite.index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::draw_sprite_batch(SpriteBatch& sprite_batch, Shader& shader)
		{
			sprite_batch.bind();

			shader.set_uniform<float>("u_width", static_cast<float>(sprite_batch.get_width()));
			shader.set_uniform<float>("u_height", static_cast<float>(sprite_batch.get_height()));

			glDrawElements(GL_TRIANGLES, sprite_batch.get_used_index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::draw_sprite_to_texture(Sprite* sprite, RenderTexture& target, Shader& shader)
		{
			shader.set_uniform("u_projection", target.get_proj());

			sprite->bind();
			shader.set_uniform("u_transform", sprite->get_transform());
			shader.set_uniform<float>("u_width", static_cast<float>(sprite->get_width()));
			shader.set_uniform<float>("u_height", static_cast<float>(sprite->get_height()));

			glDrawElements(GL_TRIANGLES, sprite->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::draw_text(Text& text, Shader& shader)
		{
			text.bind();

			shader.set_uniform("u_colour", text.get_colour());
			shader.set_uniform<float>("u_width", static_cast<float>(text.get_batch_width()));
			shader.set_uniform<float>("u_height", static_cast<float>(text.get_batch_height()));

			glDrawElements(GL_TRIANGLES, text.index_count(), GL_UNSIGNED_INT, nullptr);
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
	} // namespace graphics
} // namespace galaxy