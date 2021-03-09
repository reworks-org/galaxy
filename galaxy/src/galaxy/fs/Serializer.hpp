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
			/// Serialize a scene.
			///
			/// \param scene Scene to serialize to disk.
			/// \param path Path containing the folder to serialize to, not a file.
			///
			static void serialize(core::Scene* scene, std::string_view path);

			///
			/// Deserialize a json file.
			///
			/// \param scene Scene to deserialize.
			/// \param path Path containing the folder to serialize to, not a file.
			///
			static void deserialize(core::Scene* scene, std::string_view path);
		};
	} // namespace fs
} // namespace galaxy

#endif