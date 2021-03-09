///
/// Serializer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_SERIALIZER_HPP_
#define GALAXY_FS_SERIALIZER_HPP_

#include <string_view>

#include "galaxy/core/Scene.hpp"

namespace galaxy
{
	namespace fs
	{
		struct Serializer final
		{
			///
			/// \brief Serialize a scene.
			///
			/// Saves to "saves/SCENE_NAME.json".
			///
			/// \param scene Scene to serialize to disk.
			///
			static void serialize(core::Scene* scene);

			///
			/// Deserialize a json file.
			///
			/// \param scene Scene to deserialize.
			/// \param file Defaults to "saves/SCENE_NAME.json", but you can specify a custom filepath.
			///
			static void deserialize(core::Scene* scene, std::string_view file = "");
		};
	} // namespace fs
} // namespace galaxy

#endif