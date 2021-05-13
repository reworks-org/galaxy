///
/// RenderSystem3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <execution>

#include "galaxy/components/DirectionLight.hpp"
#include "galaxy/components/Model.hpp"
#include "galaxy/components/PointLight.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/SpotLight.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer3D.hpp"
#include "galaxy/res/MaterialBook.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "RenderSystem3D.hpp"

namespace galaxy
{
	namespace systems
	{
		void RenderSystem3D::update(core::World& world, const double dt)
		{
			RENDERER_3D().reserve_ssbo(1, world.query_count<components::DirectionLight>() * sizeof(light::Directional));
			unsigned int counter = 0;
			world.operate<components::DirectionLight>([&](const ecs::Entity entity, components::DirectionLight* dir_light) {
				RENDERER_3D().sub_buffer_ssbo(1, counter * sizeof(light::Directional), 1, &dir_light->m_light);
				counter++;
			});

			RENDERER_3D().reserve_ssbo(2, world.query_count<components::PointLight>() * sizeof(light::Point));
			counter = 0;
			world.operate<components::PointLight>([&](const ecs::Entity entity, components::PointLight* point_light) {
				RENDERER_3D().sub_buffer_ssbo(2, counter * sizeof(light::Point), 1, &point_light->m_light);
				counter++;
			});

			RENDERER_3D().reserve_ssbo(3, world.query_count<components::SpotLight>() * sizeof(light::Spot));
			counter = 0;
			world.operate<components::SpotLight>([&](const ecs::Entity entity, components::SpotLight* spot_light) {
				RENDERER_3D().sub_buffer_ssbo(3, counter * sizeof(light::Spot), 1, &spot_light->m_light);
				counter++;
			});
		}

		void RenderSystem3D::render(core::World& world, graphics::Camera3D& camera)
		{
			world.operate<components::Model>([&](const ecs::Entity entity, components::Model* model) {
				for (auto& mesh : model->get_meshes())
				{
					RENDERER_3D().draw_mesh_deferred(&mesh, SL_HANDLE.materialbook()->get(mesh.m_material_id));
				}
			});
		}
	} // namespace systems
} // namespace galaxy