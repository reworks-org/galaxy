///
/// Renderer3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "galaxy/graphics/light/Directional.hpp"
#include "galaxy/graphics/light/Spot.hpp"
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

		void Renderer3D::draw_model(Model* model, light::Point* point, light::Directional* dir, light::Spot* spot, Camera3D& camera, Shader* shader)
		{
			SL_HANDLE.window()->enable_back_cull();

			auto transform = glm::mat4 {1.0f};
			transform      = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
			transform      = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));

			glm::mat3 inverse = glm::transpose(glm::inverse(transform));

			shader->bind();

			// Vertex Uniforms.
			shader->set_uniform("u_inverse_matrix", inverse);
			shader->set_uniform("u_transform", transform);
			shader->set_uniform("u_camera_proj", camera.get_proj());
			shader->set_uniform("u_camera_view", camera.get_view());

			// Fragment uniforms.
			shader->set_uniform("u_camera_pos", camera.get_pos());

			shader->set_uniform("point_light.ambient_intensity", point->m_ambient_intensity);
			shader->set_uniform("point_light.diffuse_intensity", point->m_diffuse_intensity);
			shader->set_uniform("point_light.specular_intensity", point->m_specular_intensity);
			shader->set_uniform("point_light.pos", point->m_pos);
			shader->set_uniform("point_light.linear", point->get_linear());
			shader->set_uniform("point_light.quadratic", point->get_quadratic());

			shader->set_uniform("dir_light.ambient_intensity", dir->m_ambient_intensity);
			shader->set_uniform("dir_light.diffuse_intensity", dir->m_diffuse_intensity);
			shader->set_uniform("dir_light.specular_intensity", dir->m_specular_intensity);
			shader->set_uniform("dir_light.dir", dir->m_dir);

			shader->set_uniform("spot_light.ambient_intensity", spot->m_ambient_intensity);
			shader->set_uniform("spot_light.diffuse_intensity", spot->m_diffuse_intensity);
			shader->set_uniform("spot_light.specular_intensity", spot->m_specular_intensity);
			shader->set_uniform("spot_light.pos", spot->m_pos);
			shader->set_uniform("spot_light.dir", spot->m_dir);
			shader->set_uniform("spot_light.linear", spot->get_linear());
			shader->set_uniform("spot_light.quadratic", spot->get_quadratic());
			shader->set_uniform("spot_light.inner_cutoff", spot->m_inner_cutoff);
			shader->set_uniform("spot_light.outer_cutoff", spot->m_outer_cutoff);

			model->draw(shader);
		}

		void Renderer3D::draw_light_object(light::Object* light, Camera3D& camera, Shader* shader)
		{
			SL_HANDLE.window()->disable_culling();

			shader->bind();
			shader->set_uniform("u_transform", light->get_transform());
			shader->set_uniform("u_cameraProj", camera.get_proj());
			shader->set_uniform("u_cameraView", camera.get_view());

			light->bind();
			glDrawArrays(GL_TRIANGLES, 0, light->get_count());
		}
	} // namespace graphics
} // namespace galaxy