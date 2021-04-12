///
/// Renderer3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/graphics/light/Point.hpp"
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

			shader->bind();
			shader->set_uniform("u_transform", transform);
			shader->set_uniform("u_cameraProj", camera.get_proj());
			shader->set_uniform("u_cameraView", camera.get_view());
			shader->set_uniform("u_light_colour", light->m_colour);
			shader->set_uniform("u_ambient_strength", 1.0f);

			model->draw(shader);
		}
	} // namespace graphics
} // namespace galaxy