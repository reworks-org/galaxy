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
			/// \param file Path to a json file, including the file.
			///
			static void serialize(core::Scene* scene, std::string_view file);

			///
			/// Deserialize a json file.
			///
			/// \param scene Scene to deserialize.
			/// \param file Path to a json file, including the file.
			///
			static void deserialize(core::Scene* scene, std::string_view file);
		};
	} // namespace fs
} // namespace galaxy

#endif