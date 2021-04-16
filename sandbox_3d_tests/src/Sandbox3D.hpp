///
/// Sandbox3D.hpp
/// Sandbox3D_3d_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOX3DTESTS_SANDBOX3D_HPP_
#define SANDBOX3DTESTS_SANDBOX3D_HPP_

#include <galaxy/core/Instance.hpp>
#include <galaxy/graphics/light/Directional.hpp>
#include <galaxy/graphics/light/Spot.hpp>
#include <galaxy/graphics/light/Object.hpp>
#include <galaxy/graphics/model/Model.hpp>
#include <galaxy/core/Window.hpp>

using namespace galaxy;

namespace s3d
{
	class Sandbox3D final : public core::Instance
	{
	public:
		Sandbox3D();
		virtual ~Sandbox3D();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		core::Window* m_window;
		std::shared_ptr<scene::Scene3D> m_scene;

		graphics::Model m_model;
		light::Point m_point_light;
		light::Directional m_dir_light;
		light::Spot m_spot_light;

		light::Object m_pl_obj;
		light::Object m_spot_obj;
	};
} // namespace s3d

#endif