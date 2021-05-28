///
/// RenderSystem3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Model.hpp"
#include "galaxy/components/PointLight.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/SpotLight.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
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
			static constexpr const auto SIZEOF_PNT_LIGHT = sizeof(light::Point);
			static constexpr const auto SIZEOF_SPT_LIGHT = sizeof(light::Spot);

			RENDERER_3D().reserve_ssbo(2, world.query_count<components::PointLight>() * SIZEOF_PNT_LIGHT);
			unsigned int counter = 0;
			world.operate<components::PointLight>([&](const ecs::Entity entity, components::PointLight* point_light) {
				if (point_light->is_dirty())
				{
					RENDERER_3D().sub_buffer_ssbo(2, counter * SIZEOF_PNT_LIGHT, 1, &point_light->get_data());
				}

				counter++;
			});

			RENDERER_3D().reserve_ssbo(3, world.query_count<components::SpotLight>() * SIZEOF_SPT_LIGHT);
			counter = 0;
			world.operate<components::SpotLight>([&](const ecs::Entity entity, components::SpotLight* spot_light) {
				if (spot_light->is_dirty())
				{
					RENDERER_3D().sub_buffer_ssbo(3, counter * SIZEOF_SPT_LIGHT, 1, &spot_light->get_data());
				}

				counter++;
			});
		}

		void RenderSystem3D::render_shadows(core::World& world, graphics::Camera3D& camera)
		{
			SL_HANDLE.window()->disable_culling();

			world.operate<components::Model>([&](const ecs::Entity entity, components::Model* model) {
				for (auto& mesh : model->get_meshes())
				{
					RENDERER_3D().draw_mesh_shadows(&mesh);
				}
			});
		}

		void RenderSystem3D::render(core::World& world, graphics::Camera3D& camera)
		{
			SL_HANDLE.window()->enable_back_cull();

			world.operate<components::Model>([&](const ecs::Entity entity, components::Model* model) {
				for (auto& mesh : model->get_meshes())
				{
					RENDERER_3D().draw_mesh_deferred(&mesh, SL_HANDLE.materialbook()->get(mesh.m_material_id));
				}
			});
		}
	} // namespace systems
} // namespace galaxy