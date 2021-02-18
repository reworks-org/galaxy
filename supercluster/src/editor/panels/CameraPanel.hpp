///
/// CameraPanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_CAMERAPANEL_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_CAMERAPANEL_HPP_

#include <galaxy/core/Scene.hpp>

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		class CameraPanel final
		{
		public:
			void render();

			void set_scene(core::Scene* scene);

		private:
			core::Scene* m_cur_scene = nullptr;
		};
	} // namespace panel
} // namespace sc

#endif