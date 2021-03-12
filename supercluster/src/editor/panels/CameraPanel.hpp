///
/// CameraPanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_CAMERAPANEL_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_CAMERAPANEL_HPP_

#include <galaxy/core/Instance.hpp>

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		class CameraPanel final
		{
		public:
			~CameraPanel() noexcept;

			void render();

			void set_instance(core::Instance* instance);

		private:
			core::Instance* m_cur_instance = nullptr;
		};
	} // namespace panel
} // namespace sc

#endif