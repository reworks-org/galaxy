///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_HPP_
#define SUPERCLUSTER_EDITOR_HPP_

#include <protostar/state/State.hpp>
#include <galaxy/layer/LayerStorage.hpp>

namespace sc
{
	class Editor : public pr::State
	{
	public:
		Editor(pr::ProtectedDouble* dt) noexcept;
		~Editor() noexcept override;
		void onPush() noexcept override;
		void onPop() noexcept override;
		void events() noexcept override;
		void update(pr::ProtectedDouble* dt) noexcept override;
		void render() noexcept override;

	private:
		qs::Camera m_camera;
		pr::ProtectedDouble* m_dt;
		galaxy::LayerStorage m_layers;
	};
}

#endif