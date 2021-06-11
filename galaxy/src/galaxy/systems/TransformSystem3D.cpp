///
/// TransformSystem3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Model.hpp"
#include "galaxy/components/Transform3D.hpp"
#include "galaxy/core/World.hpp"

#include "TransformSystem3D.hpp"

namespace galaxy
{
	namespace systems
	{
		TransformSystem3D::TransformSystem3D() noexcept
		{
		}

		TransformSystem3D::~TransformSystem3D() noexcept
		{
		}

		void TransformSystem3D::update(core::World& world, const double dt)
		{
			world.operate<components::Transform3D, components::Model>(std::execution::par_unseq, [&](const ecs::Entity entity, components::Transform3D* transform, components::Model* model) {
				if (transform->is_dirty())
				{
					transform->recalculate();
					std::for_each(std::execution::par_unseq, model->get_meshes().begin(), model->get_meshes().end(), [&](auto& mesh) {
						std::for_each(std::execution::par_unseq, mesh.m_verticies.begin(), mesh.m_verticies.end(), [&](auto& vertex) {
							vertex.m_pos      = glm::vec3 {transform->get_transform() * glm::vec4 {vertex.m_pos, 1.0f}};
							vertex.m_normal   = glm::vec3 {transform->get_transform() * glm::vec4 {vertex.m_normal, 0.0f}};
							vertex.m_tangents = glm::vec3 {transform->get_transform() * glm::vec4 {vertex.m_tangents, 0.0f}};
						});
					});

					model->create();
				}
			});
		}
	} // namespace systems
} // namespace galaxy