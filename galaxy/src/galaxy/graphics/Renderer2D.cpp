///
/// Renderer2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Sprite2D.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/shader/Shader.hpp"
#include "galaxy/graphics/texture/RenderTexture.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "Renderer2D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Renderer2D::Renderer2D() noexcept
		    : m_texture {nullptr}, m_batch_shader {nullptr}, m_max_quads {0}
		{
		}

		Renderer2D::~Renderer2D() noexcept
		{
			clean_up();
		}

		Renderer2D& Renderer2D::inst() noexcept
		{
			static Renderer2D s_inst;
			return s_inst;
		}

		void Renderer2D::init(const unsigned int max_quads, std::string_view batch_shader)
		{
			m_max_quads    = max_quads;
			m_batch_shader = SL_HANDLE.shaderbook()->get(batch_shader);
		}

		void Renderer2D::clean_up()
		{
			for (auto& batch : m_batches)
			{
				batch.second->clear();
				batch.second = nullptr;
			}

			m_batches.clear();
			m_batch_shader = nullptr;
			m_texture      = nullptr;
		}

		void Renderer2D::create_default_batches(BaseTexture* texture) noexcept
		{
			m_texture = texture;

			for (auto i = 0; i < 10; i++)
			{
				m_batches[i] = std::make_unique<graphics::SpriteBatch>(m_max_quads);
				m_batches[i]->set_texture(m_texture);
			}
		}

		void Renderer2D::add_batched_sprite(components::BatchSprite* batchsprite, components::Transform2D* transform, int zlevel)
		{
			if (!m_batches.contains(zlevel))
			{
				m_batches[zlevel] = std::make_unique<graphics::SpriteBatch>(m_max_quads);
				m_batches[zlevel]->set_texture(m_texture);
			}

			m_batches[zlevel]->add(batchsprite, transform, zlevel);
		}

		void Renderer2D::calculate_batches()
		{
			for (auto& batch : m_batches)
			{
				if (!batch.second->empty())
				{
					batch.second->calculate();
				}
			}
		}

		void Renderer2D::clear()
		{
			for (auto& batch : m_batches)
			{
				batch.second->clear_sprites();
			}
		}

		void Renderer2D::draw_point(components::Primitive2D* data, components::Transform2D* transform, Shader* shader)
		{
			data->bind();
			shader->set_uniform("u_point_size", *data->get_data().m_pointsize);
			shader->set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_POINTS, data->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_line(components::Primitive2D* data, components::Transform2D* transform, Shader* shader)
		{
			data->bind();
			shader->set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_LINES, data->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_lineloop(components::Primitive2D* data, components::Transform2D* transform, Shader* shader)
		{
			data->bind();
			shader->set_uniform("u_transform", transform->get_transform());

			glDrawElements(GL_LINE_LOOP, data->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_spritebatches(Camera2D& camera)
		{
			m_batch_shader->bind();
			m_batch_shader->set_uniform("u_cameraProj", camera.get_proj());
			m_batch_shader->set_uniform("u_cameraView", camera.get_view());

			for (auto& batch : m_batches)
			{
				m_batch_shader->set_uniform("u_width", static_cast<float>(batch.second->get_width()));
				m_batch_shader->set_uniform("u_height", static_cast<float>(batch.second->get_height()));

				batch.second->bind();
				glDrawElements(GL_TRIANGLES, batch.second->get_used_index_count(), GL_UNSIGNED_INT, nullptr);
			}
		}

		void Renderer2D::draw_text(components::Text* text, components::Transform2D* transform, Shader* shader)
		{
			text->m_batch.calculate(transform);
			text->bind();

			const auto& norm_col = text->get_colour().normalized();
			shader->set_uniform("u_colour", norm_col);
			shader->set_uniform("u_width", static_cast<float>(text->get_batch_width()));
			shader->set_uniform("u_height", static_cast<float>(text->get_batch_height()));

			glDrawElements(GL_TRIANGLES, text->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer2D::draw_sprite(components::Sprite2D* sprite, components::Transform2D* transform, Shader* shader)
		{
			sprite->bind();

			shader->set_uniform("u_transform", transform->get_transform());
			shader->set_uniform("u_opacity", sprite->get_opacity());
			shader->set_uniform("u_width", static_cast<float>(sprite->get_width()));
			shader->set_uniform("u_height", static_cast<float>(sprite->get_height()));

			glDrawElements(GL_TRIANGLES, sprite->index_count(), GL_UNSIGNED_INT, nullptr);
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

		void Renderer2D::draw_sprite_to_target(components::Sprite2D* sprite, components::Transform2D* transform, Shader* shader, RenderTexture* target)
		{
			sprite->bind();

			shader->bind();
			shader->set_uniform("u_projection", target->get_proj());
			shader->set_uniform("u_transform", transform->get_transform());
			shader->set_uniform("u_width", static_cast<float>(sprite->get_width()));
			shader->set_uniform("u_height", static_cast<float>(sprite->get_height()));

			glDrawElements(GL_TRIANGLES, sprite->index_count(), GL_UNSIGNED_INT, nullptr);
		}
	} // namespace graphics
} // namespace galaxy