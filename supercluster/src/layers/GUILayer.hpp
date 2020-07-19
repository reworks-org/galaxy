///
/// GUILayer.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_GUILAYER_HPP_
#define SUPERCLUSTER_GUILAYER_HPP_

#include <galaxy/layer/Layer.hpp>
#include <galaxy/ui/GUI.hpp>

namespace sc
{
	class GUILayer final : public galaxy::Layer
	{
	public:
		GUILayer(pr::ProtectedDouble* dt) noexcept;
		virtual ~GUILayer() noexcept;

		void events() noexcept override;
		void update(pr::ProtectedDouble* dt) noexcept override;
		void render(qs::Camera& camera) noexcept override;

	private:
		galaxy::GUI m_gui;
	};
}

#endif