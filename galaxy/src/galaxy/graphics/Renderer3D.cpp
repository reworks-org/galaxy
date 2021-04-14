///
/// Renderer3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/graphics/light/Point.hpp"
#include "galaxy/graphics/light/Object.hpp"
#include "galaxy/graphics/model/Model.hpp"
#include "galaxy/graphics/shader/Shader.hpp"

#include "Renderer3D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Renderer3D::Renderer3D() noexcept
		{
		}

		Renderer3D::~Renderer3D() noexcept
		{
		}

		Renderer3D& Renderer3D::inst() noexcept
		{
			static Renderer3D s_inst;
			return s_inst;
		}

		void Renderer3D::draw_model(Model* model, light::Point* light, Camera3D& camera, Shader* shader)
		{
			auto transform = glm::mat4 {1.0f};
			transform      = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
			transform      = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));

			glm::mat3 inverse = glm::transpose(glm::inverse(transform));

			shader->bind();
			shader->set_uniform("u_inverse_matrix", inverse);
			shader->set_uniform("u_transform", transform);
			shader->set_uniform("u_camera_proj", camera.get_proj());
			shader->set_uniform("u_camera_view", camera.get_view());
			shader->set_uniform("u_camera_pos", camera.get_pos());
			shader->set_uniform("light.ambient_intensity", light->m_ambient_intensity);
			shader->set_uniform("light.diffuse_intensity", light->m_diffuse_intensity);
			shader->set_uniform("light.specular_intensity", light->m_specular_intensity);
			shader->set_uniform("u_light_pos", light->m_pos);

			model->draw(shader);
		}

		void Renderer3D::draw_light_object(light::Object* light, Camera3D& camera, Shader* shader)
		{
			shader->bind();
			shader->set_uniform("u_transform", light->get_transform());
			shader->set_uniform("u_cameraProj", camera.get_proj());
			shader->set_uniform("u_cameraView", camera.get_view());

			light->bind();
			glDrawArrays(GL_TRIANGLES, 0, light->get_count());
		}
	} // namespace graphics
} // namespace galaxy