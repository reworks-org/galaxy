///
/// AssetType.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_FS_ASSETTYPE_HPP_
#define GALAXY_FS_ASSETTYPE_HPP_

namespace galaxy
{
	namespace fs
	{
		///
		/// The type of an asset.
		///
		enum class AssetType
		{
			UNKNOWN,
			MUSIC,
			SFX,
			VOICE,
			FONT,
			SCRIPT,
			SHADER,
			TEXTURE,
			ATLAS,
			LANG,
			PREFABS,
			MAPS,
			VIDEO,
			UI,
			UI_FONT
		};
	} // namespace fs
} // namespace galaxy

#endif
