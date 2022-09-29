///
/// SelectedAsset.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_SELECTEDASSET_HPP_
#define SUPERCLUSTER_EDITOR_SELECTEDASSET_HPP_

#include <filesystem>

namespace sc
{
	struct SelectedAsset
	{
		std::string m_extension;
		std::filesystem::path m_path;
		bool m_is_hovered = false;
	};
} // namespace sc

#endif