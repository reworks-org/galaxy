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

namespace sc
{
	class ECSLayer final : public galaxy::Layer
	{
	public:
		ECSLayer();
		virtual ~ECSLayer();

		void events() override;
		void update(const double dt) override;
		void render(qs::Camera& camera) override;

	private:
		galaxy::World* m_world;
		qs::Window* m_window;
	};
} // namespace sc

#endif