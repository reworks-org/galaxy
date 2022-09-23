///
/// AssetPanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_ASSETPANEL_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_ASSETPANEL_HPP_

#include <filesystem>

namespace sc
{
	namespace panel
	{
		class AssetPanel final
		{
		public:
			AssetPanel() noexcept;
			~AssetPanel() noexcept = default;

			void render();

		private:
			std::filesystem::path m_current_path;
		};
	} // namespace panel
} // namespace sc

#endif