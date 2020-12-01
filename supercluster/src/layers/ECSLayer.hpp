///
/// ECSLayer.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_ECSLAYER_HPP_
#define SUPERCLUSTER_ECSLAYER_HPP_

#include <qs/core/Window.hpp>
#include <galaxy/core/World.hpp>
#include <galaxy/layer/Layer.hpp>
#include <qs/graphics/Camera.hpp>

namespace sc
{
	class ECSLayer final : public galaxy::Layer
	{
	public:
		ECSLayer();
		virtual ~ECSLayer();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		qs::Camera m_camera;
		galaxy::World* m_world;
		qs::Window* m_window;
	};
} // namespace sc

#endif