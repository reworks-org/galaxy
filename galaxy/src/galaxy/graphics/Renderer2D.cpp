///
/// Renderer2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/particle/ParticleGenerator.hpp"
#include "galaxy/graphics/texture/RenderTexture.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "Renderer2D.hpp"

namespace galaxy
{
	namespace graphics
	{
		void Renderer2D::init(const unsigned int max_quads, std::string_view batch_shader)
		{
			Renderer2D::m_post_shaders.push_back(SL_HANDLE.shaderbook()->get("DefaultFramebuffer"));

			m_batch        = std::make_unique<graphics::SpriteBatch>(max_quads);
			m_batch_shader = SL_HANDLE.shaderbook()->get(batch_shader);
		}

		void Renderer2D::clean_up()
		{
			Renderer2D::m_post_shaders.clear();
			m_batch.reset();
			m_batch        = nullptr;
			m_batch_shader = nullptr;
		}

		void Renderer2D::draw_point(components::Primitive2D* data, components::Transform* transform, Shader* shader)
		{
			data->bind();
			shader->set_uniform("u_point_size", *data->get_data().m_pointsize);
			shader->set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_POINTS, data->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_line(components::Primitive2D* data, components::Transform* transform, Shader* shader)
		{
			data->bind();
			shader->set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_LINES, data->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_lineloop(components::Primitive2D* data, components::Transform* transform, Shader* shader)
		{
			data->bind();
			shader->set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_LINE_LOOP, data->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_spritebatch(Camera& camera)
		{
			Renderer2D::m_batch_shader->bind();
			Renderer2D::m_batch_shader->set_uniform("u_cameraProj", camera.get_proj());
			Renderer2D::m_batch_shader->set_uniform("u_cameraView", camera.get_view());
			Renderer2D::m_batch_shader->set_uniform("u_width", static_cast<float>(Renderer2D::m_batch->get_width()));
			Renderer2D::m_batch_shader->set_uniform("u_height", static_cast<float>(Renderer2D::m_batch->get_height()));

			Renderer2D::m_batch->bind();
			glDrawElements(GL_TRIANGLES, Renderer2D::m_batch->get_used_index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_text(components::Text* text, components::Transform* transform, Shader* shader)
		{
			text->m_batch.calculate(transform);
			text->bind();

			const auto& norm_col = text->get_colour().normalized();
			shader->set_uniform("u_colour", norm_col);
			shader->set_uniform("u_width", static_cast<float>(text->get_batch_width()));
			shader->set_uniform("u_height", static_cast<float>(text->get_batch_height()));

			glDrawElements(GL_TRIANGLES, text->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_batch(graphics::SpriteBatch* sb, Camera& camera)
		{
			Renderer2D::m_batch_shader->bind();
			Renderer2D::m_batch_shader->set_uniform("u_cameraProj", camera.get_proj());
			Renderer2D::m_batch_shader->set_uniform("u_cameraView", camera.get_view());
			Renderer2D::m_batch_shader->set_uniform("u_width", static_cast<float>(sb->get_width()));
			Renderer2D::m_batch_shader->set_uniform("u_height", static_cast<float>(sb->get_height()));

			sb->bind();
			glDrawElements(GL_TRIANGLES, sb->get_used_index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_texture_to_target(graphics::VertexData* vertex_data, graphics::BaseTexture* texture, const glm::mat4& transform, Shader* shader, RenderTexture* target)
		{
			vertex_data->get_vao().bind();
			texture->bind();

			shader->bind();
			shader->set_uniform("u_projection", target->get_proj());
			shader->set_uniform("u_transform", transform);
			shader->set_uniform("u_width", static_cast<float>(texture->get_width()));
			shader->set_uniform("u_height", static_cast<float>(texture->get_height()));

			glDrawElements(GL_TRIANGLES, vertex_data->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_particles(graphics::ParticleGenerator* gen, Camera& camera)
		{
			if (!gen->finished())
			{
				gen->bind();
				auto* current = gen->get_instance();
				auto* shader  = SL_HANDLE.shaderbook()->get("particle");

				shader->bind();
				shader->set_uniform("u_cameraProj", camera.get_proj());
				shader->set_uniform("u_cameraView", camera.get_view());
				shader->set_uniform("u_width", static_cast<float>(current->get_width()));
				shader->set_uniform("u_height", static_cast<float>(current->get_height()));

				glDrawElementsInstanced(GL_TRIANGLES, gen->gl_index_count(), GL_UNSIGNED_INT, nullptr, gen->amount());
			}
		}
	} // namespace graphics
} // namespace galaxy