///
/// AssetPanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>

#include "AssetPanel.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		AssetPanel::AssetPanel() noexcept
		{
			m_current_path = core::ServiceLocator<fs::VirtualFileSystem>::ref().root_path();
		}

		void AssetPanel::render()
		{
			// https://github.com/TheCherno/Hazel/blob/scripting/Hazelnut/src/Panels/ContentBrowserPanel.h
			// https://github.com/TheCherno/Hazel/blob/scripting/Hazelnut/src/Panels/ContentBrowserPanel.cpp
		}
	} // namespace panel
} // namespace sc